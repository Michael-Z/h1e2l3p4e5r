#include "mgc_recognizer.h"

MGC_Recognizer::MGC_Recognizer(QString id, MGC_Element *parent, const QJsonObject &settings)
    : MGC_Element(id, parent, settings)
{
     m_tolerance = m_settings["tolerance"].toDouble();

    if (m_settings.contains("mask")) {
        QString maskPath = m_settings["mask"].toString();
        QString path = glob().ROOM_DATA_PATH + "Masks/" + maskPath;
        m_mask_path = path;
        m_mask = imread(path.toStdString() + ".bmp");
    } else if (m_settings.contains("maskSet")) {
        QString maskSetPath = m_settings["maskSet"].toString();
        QString path = glob().ROOM_DATA_PATH + "Masks/" + maskSetPath;
        m_maskSet_path = path;
        m_maskSet = loadMaskSet(path);
    } else if (m_settings.contains("maskSets")) {
        QJsonObject maskSetsObj = m_settings["maskSets"].toObject();
        QStringList maskSetsKeys = maskSetsObj.keys();
        QString partialPath = glob().ROOM_DATA_PATH + "Masks/";
        m_maskSets_path = partialPath;
        for (QString maskSetKey: maskSetsKeys) {
            QString maskSetPath = maskSetsObj[maskSetKey].toString();
            QString path = partialPath + maskSetPath;
            MaskSet_t maskSet = loadMaskSet(path);
            m_maskSets.insert(maskSetKey, maskSet);
        }
    } else {
        //throw "where is no mask"; for example tableId
    }
    m_elementWidget->setGeometry(m_elementArea);
}

MaskSet_t MGC_Recognizer::loadMaskSet(QString path) const
{
    MaskSet_t maskSet;

    QDir masksDir(path);

    QStringList dirEntryList = masksDir.entryList();
    for (QString maskfileName: dirEntryList) {
        if (maskfileName == "." || maskfileName == "..") continue;
        QString fullMaskfileName = QString("") + path + maskfileName;
        Mask_t curMaskMat = imread(fullMaskfileName.toStdString());
        maskSet.insert(maskfileName.left(maskfileName.size() - 4), curMaskMat);
    }

    return maskSet;
}

int MGC_Recognizer::compare2Masks(Mask_t &mat1, Mask_t &mat2) const
{
    double MSD;
    MSD = norm(mat1, mat2);
    MSD = MSD * MSD / mat1.total();
    return static_cast<int>(MSD);
}

bool MGC_Recognizer::recognizeMask()
{    
    m_recognizedMaskName = "";
    const int difference = compare2Masks(m_elementMat, m_mask);
    bool result = false;
    if (m_tolerance > difference) result = true;
#ifdef DBG
    qDebug() << "recognizeMask"
             << ", defference: " << difference
             << ", result: " << result << "\n";
#endif
    return result;
}

bool MGC_Recognizer::recognizeMaskInSet(QString &maskName)
{    
    m_recognizedMaskName = "";
    const int difference = compare2Masks(m_elementMat, m_maskSet[maskName]);
    bool result = false;
    if (m_tolerance > difference) {
        result = true;
        m_mask = m_maskSet[maskName];
    }
#ifdef DBG
    qDebug() << "recognizeMaskInSet | maskName: " << maskName
             << ", defference: " << difference
             << ", result: " << result << "\n";
#endif
    return true;
}

bool MGC_Recognizer::recognizeMaskInSet()
{    
    m_recognizedMaskName = "";
    QHash<QString, int> masksDifferencies;
    QStringList maskSetNames = m_maskSet.keys();
    for (QString maskName: maskSetNames) {
        Mask_t currentMaskMat = m_maskSet[maskName];
        masksDifferencies.insert(maskName, compare2Masks(m_elementMat, currentMaskMat));
    }
#ifdef DBG
    qDebug() << "recognizeMaskInSet | " << "\n";
    qDebug() << "for parent: " << m_parent->m_id << "\n";
#endif
    QString msg = "";
    double minValue(999999999); QString minName("");
    for (QString maskName: maskSetNames) {
        int defference = masksDifferencies[maskName];
#ifdef DBG
        qDebug() << "maskName: " << maskName
                 << ", defference: " << defference << "\n";        
#endif
        if (m_tolerance < defference) continue;        
        if (minValue < defference) continue;
        minValue = defference;
        minName = maskName;
    }

    if (minName != "") {
        m_recognizedMaskName = minName;
        m_mask = m_maskSet[m_recognizedMaskName];
#ifdef DBG
        qDebug() << "m_resultMask was found | m_recognizedMaskName: " << m_recognizedMaskName
                 << " with m_tolerance: " << m_tolerance << "\n";
#endif
        return true;
    } else {
#ifdef DBG
        qDebug() << "m_recognizedMaskName was't found | with m_tolerance: " << m_tolerance << "\n";
#endif
        switch (m_misrecMode) {
        case MisrecMode::IGNORE: {
            break;
        }
        case MisrecMode::SIMPLE_REQUESTING: {            
            QString tmpStyleSheet = m_elementWidget->styleSheet();
            m_elementWidget->setStyleSheet("QLabel {border: 1px solid yellow;}");            
            bool wasVisible;
            if (wasVisible = m_elementWidget->isVisible()) m_elementWidget->show();

            AddNewMaskHelper *popUp = new AddNewMaskHelper(masksDifferencies, m_maskSet_path,
                m_elementMat, m_elementWidget->mapToGlobal(QPoint(0, 0)));
            popUp->exec();
            if (popUp->m_hasMaskAdded) {
                addMaskToMaskSetFromElementMat(popUp->m_newMaskName);
            }
            delete popUp;

            m_elementWidget->setStyleSheet(tmpStyleSheet);
            if (wasVisible) {
                m_elementWidget->show();
            } else {
                m_elementWidget->hide();
            }
    //        resolveMisrecRecogMaskBySimpleRequest(m_mask_path);
            break;
        }
        default:
            throw "WHY?";
            break;
        }

        return false;
    }
}

bool MGC_Recognizer::recognizeMaskInSets(QString &maskSetName)
{    
    m_recognizedMaskName = "";
    QHash<QString, int> masksDifferencies;
    QStringList maskSetNames = m_maskSets[maskSetName].keys();
    for (QString maskName: maskSetNames) {
        Mask_t currentMaskMat = m_maskSets[maskSetName][maskName];
        masksDifferencies.insert(maskName, compare2Masks(m_elementMat, currentMaskMat));
    }    
#ifdef DBG
    qDebug() << "recognizeMaskSet | maskSetName: " << maskSetName << "\n";
    qDebug() << "for parent: " << m_parent->m_id << "\n";
#endif
    double minValue(999999999); QString minName("");
    for (QString maskName: maskSetNames) {
        int defference = masksDifferencies[maskName];
#ifdef DBG
        qDebug() << "maskName: " << maskName
                 << ", defference: " << defference << "\n";
#endif
        if (m_tolerance < defference) continue;
        if (minValue < defference) continue;
        minValue = defference;
        minName = maskName;
    }

    if (minName != "") {
        m_recognizedMaskName = minName;
        m_mask = m_maskSets[maskSetName][m_recognizedMaskName];
#ifdef DBG
        qDebug() << "m_resultMask was found | m_recognizedMaskName: " << m_recognizedMaskName
                 << " with m_tolerance: " << m_tolerance << "\n";
#endif
        return true;
    } else {
#ifdef DBG
        qDebug() << "m_recognizedMaskName was't found | with m_tolerance: " << m_tolerance << "\n";
#endif
        return false;
    }
}

int MGC_Recognizer::findPoints(int matchAmount, QString &maskName)
{    
#ifdef DBG
    qDebug() << "findPoints | for maskName: " << maskName << ", matchAmount: " << matchAmount << "\n";
#endif
    m_foundPoints.clear();

    if (maskName != QStringLiteral("")) m_mask = m_maskSet[maskName];

    /// Create the result matrix
    int result_cols =  m_elementMat.cols - m_mask.cols + 1;
    int result_rows =  m_elementMat.rows - m_mask.rows + 1;
    Mat findResult;
    findResult.create(result_rows, result_cols, CV_32FC1);

    /// Do the Matching and Normalize
    matchTemplate(m_elementMat, m_mask, findResult, CV_TM_CCOEFF_NORMED);
    normalize(findResult, findResult, 0, 1, NORM_MINMAX, -1, Mat() );

    QRect curFoundArea; Mat curFoundMat;
    int resCouter = 0;
    for (int i = 0; i < matchAmount; ++i) {

        /// Localizing the best match with minMaxLoc
        /// /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        minMaxLoc(findResult, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

        curFoundArea.setX(maxLoc.x);
        curFoundArea.setY(maxLoc.y);
        curFoundArea.setWidth(m_mask.cols);
        curFoundArea.setHeight(m_mask.rows);
#ifdef DBG
        qDebug() << "maxVal: " << maxVal << "\n";
#endif
        if (curFoundArea.x() < 0 || curFoundArea.x() + curFoundArea.width() > m_elementArea.width()
                || curFoundArea.y() < 0 || curFoundArea.y() + curFoundArea.height() > m_elementArea.height()) {
            break;
        }

        curFoundMat = m_elementMat(convertQRect2Rect(curFoundArea));

        double MSD;
        MSD = norm(curFoundMat, m_mask);
        MSD = MSD * MSD / curFoundMat.total();
#ifdef DBG
        qDebug() << "defference(MSD): " << MSD << "\n";
#endif
        if (m_tolerance < MSD) break;

        QPoint curPoint (curFoundArea.x(), curFoundArea.y());
        m_foundPoints.append(curPoint);
        ++resCouter;
        findResult.at<float>(curPoint.y(),curPoint.x()) = 0;
    }
    return resCouter;
}

void MGC_Recognizer::addMaskFromElementMat()
{    
    m_mask = m_elementMat;
}

void MGC_Recognizer::addMaskToMaskSetFromElementMat(QString &maskName)
{    
    m_maskSet.insert(maskName, m_elementMat);
}

void MGC_Recognizer::show()
{
    switch(m_drawMode) {

    case DrawMode::ELEMENT_MAT:
        MGC_Element::show();
        break;

    case DrawMode::MASK:
        if (m_recognizedMaskName != QStringLiteral("")) {
            m_elementWidget->setGeometry(m_elementArea);
            m_elementWidget->setPixmap(convertMat2Pixmap(m_mask));
            m_elementWidget->show();
        } else {
            hide();
        }
        break;

    default:
        throw "WHY? which mode?";
        break;
    }
}

void MGC_Recognizer::addAndSaveMask(QString &folder)
{
    if (m_recognizedMaskName != QString("")) return;
    addMaskToMaskSetFromElementMat(QString::number(m_nameCounter));
    QString path = glob().ROOM_DATA_PATH + "Masks/" + folder + QString("/")
            + QString::number(m_nameCounter) + QString(".bmp");
    ++m_nameCounter;
    saveMatToFile(path, m_elementMat);
}

//void MGC_Recognizer::resolveMisrecRecogMaskBySimpleRequest(m_mask_path)
//{


//}
