#include "mgc_screen.h"

MGC_Screen::MGC_Screen(QString id, QWidget *widgetParent, const QJsonObject &settings, QJsonObject &tableSettings) :
    MGC_Element(id, 0, settings), m_tableSettings(tableSettings)
{
    m_primaryScreen = QGuiApplication::primaryScreen();
    m_elementWidget = new QLabel(widgetParent);
    m_elementWidget->setGeometry(m_elementArea);
    m_elementWidget->move(0, 0);

    m_tableCounter = new MGC_Recognizer(QString("tableCounter"), this, m_tableSettings["base_offset_sign"].toObject());    
    m_tableCounter->setBorder("1", "dashed", "gray");
}

void MGC_Screen::loadScreensFromFolder(QString &path)
{
    QDir masksDir(path); //QDir masksDir(QDir::currentPath() + path);
    QStringList dirEntryList = masksDir.entryList();

    for (auto item: dirEntryList) {
        if (item == "." || item == "..") continue;
        QString fileName = QString("") + path + item;
        QTime bench; bench.start();
        m_screensQueue.enqueue(cv::imread(fileName.toStdString()));
        qWarning() << "load takes : " << bench.elapsed() << "\n";
    }
}

bool MGC_Screen::refreshElementMat()
{
    takeRootScreenshot();
    if (m_screensQueue.isEmpty()) return false;
    m_elementMat = m_screensQueue.dequeue();
    return true;
}

void MGC_Screen::takeRootScreenshot()
{
    //if (screen) qPixMap = screen->grabWindow(0);
    QPixmap qPixMap = m_primaryScreen->grabWindow(0, m_elementArea.x(), m_elementArea.y(), m_elementArea.width(), m_elementArea.height());
    QImage qImage = qPixMap.toImage();
    cv::Mat tmp(qImage.height(),qImage.width(),CV_8UC4,(uchar*)qImage.bits(),qImage.bytesPerLine());
    cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
    cv::cvtColor(tmp, result, CV_RGBA2RGB);
    m_screensQueue.enqueue(result);
}

void MGC_Screen::show()
{
    m_elementWidget->show();
}

bool MGC_Screen::processOnDown() {

    refreshElementMat();

    m_tableCounter->refreshElementMat();
    m_tableCounter->findPoints(4);

    return true;
}

void MGC_Screen::processOnUp()
{    
    for (QPoint curFoundSingPoint: m_tableCounter->m_foundPoints) {
        QString tableId = QString("table_") + QString::number(m_addedTableCounter);
        MGC_Table *newTable = new MGC_Table(tableId, this, m_tableSettings, curFoundSingPoint);
        if (newTable->m_tableId) {
            ++m_addedTableCounter;
        } else {
            delete newTable;
        }
    }    
}
