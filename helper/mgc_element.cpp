#include "mgc_element.h"

MGC_Element::MGC_Element(QString id, MGC_Element *parent, const QJsonObject &settings)
    : QObject(), m_id(id), m_parent(parent), m_settings(settings)
{

    if (m_settings.contains("element_id")) {
        m_id = m_settings["id"].toString();
    }

//    if (m_settings.contains("element_area")) {
        m_elementArea = convertQJsonArray2QRect(m_settings["element_area"].toArray());
//    }

    if (!m_parent) return;
    m_elementWidget = new QLabel(m_parent->m_elementWidget);
    m_elementWidget->setStyleSheet("QLabel {border: 0px solid blue;}");
    m_parent->m_children.insert(m_id, this);
}

MGC_Element::~MGC_Element()
{
    for (QString curChildKey: m_children.keys()) {
        delete m_children[curChildKey];
    }
    m_parent->m_children.remove(m_id);
    delete m_elementWidget;
}

bool MGC_Element::refreshElementMat()
{
    Mat &parentMat = m_parent->m_elementMat;
    if (parentMat.cols < (m_elementArea.x() + m_elementArea.width())
            || parentMat.rows < (m_elementArea.y() + m_elementArea.height())) return false;

    m_elementMat = parentMat(convertQRect2Rect(m_elementArea));
    return true;
}

bool MGC_Element::processOnDown()
{
    refreshElementMat();
    return true;
}

void MGC_Element::processOnUp()
{    
    return;
}

void MGC_Element::processNextScreen()
{
    if (!m_isAvailable)  return;
    if (!processOnDown())  return;
    for (QString curChildKey: m_children.keys()) {
        m_children[curChildKey]->processNextScreen();
    }
    processOnUp();
}

void MGC_Element::render()
{
    if (m_isVisible) {
        show();
        for (QString curChildKey: m_children.keys()) {
            m_children[curChildKey]->render();
        }
    } else {
        hide();
    }
}

void MGC_Element::show()
{    
    m_elementWidget->setGeometry(m_elementArea);
    if (doDrawElementMat) m_elementWidget->setPixmap(convertMat2Pixmap(m_elementMat));
    m_elementWidget->show();
}

void MGC_Element::hide()
{
   m_elementWidget->hide();
}

void MGC_Element::showElementMat() const
{
    showMat(m_elementMat, String("elementMat"));
}

void MGC_Element::showMat(const Mat &mat, String winName) const
{
    namedWindow(winName, 1);
    imshow(winName, mat);
    waitKey(0);
    destroyWindow(winName);
}

Rect MGC_Element::convertQRect2Rect(QRect &area)
{
    return Rect(area.x(), area.y(),area.width(), area.height());
}

QPixmap MGC_Element::convertMat2Pixmap(Mat &mat)
{
    cv::Mat tempMat; // make the same cv::Mat
    cvtColor(mat, tempMat, CV_BGR2RGB); // cvtColor Makes a copt, that what i need
    QImage resImage((const uchar *) tempMat.data, (int) tempMat.cols, (int) tempMat.rows, (int) tempMat.step, QImage::Format_RGB888);
    resImage.bits(); // enforce deep copy, see documentation
    return QPixmap::fromImage(resImage);
}

QRect MGC_Element::convertQJsonArray2QRect(QJsonArray &arr)
{
    return QRect(QPoint(arr[0].toInt(), arr[1].toInt()), QSize(arr[2].toInt(), arr[3].toInt()));
}

bool MGC_Element::saveMatToFile(QString fileName, Mat &mat)
{
    if (!imwrite(fileName.toStdString(), mat)) {
        throw "can't save mat to file";
    }
    return true;
}

void MGC_Element::setBorder(QString width, QString style, QString color)
{
    QString styleStr = "QLabel {border: " + width + "px " + style + " " + color + ";}";
    m_elementWidget->setStyleSheet(styleStr);
}

