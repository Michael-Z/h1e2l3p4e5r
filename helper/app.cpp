#include "app.h"

App::App(QMainWindow *parent, Ui_MainWindow *ui) : QObject(parent), m_parent(parent), m_ui(ui)
{
    m_settings = MAC_Utility::readQJsonFromFile(glob().PROJECT_PATH + "settings.json").object();
    glob().ROOM_DATA_PATH = glob().PROJECT_PATH + m_settings["room_data_path"].toString();
    m_roomSettings = MAC_Utility::readQJsonFromFile(glob().ROOM_DATA_PATH + "settings.json").object();

    m_screen = new MGC_Screen(QString("screen"), ui->mainTab, m_settings["screen"].toObject(), m_roomSettings["table"].toObject());

    int width = m_screen->m_elementArea.width(); int height = m_screen->m_elementArea.height();

    ui->mainTab->resize(width, height);
    ui->tabWidget->resize(ui->mainTab->width(), ui->mainTab->height() + 25);
    ui->centralWidget->resize(ui->tabWidget->width(), ui->tabWidget->height() + 0);
    m_parent->setGeometry(QRect(0, 30, ui->centralWidget->width(), ui->centralWidget->height() + 50));

    m_server = new Server();
    glob().m_server = m_server;



    QString program = "node";
    QStringList arguments;
    arguments << "d:/___fenix/extractor/_app.js"; //qDebug() << QDir::currentPath();
    arguments << glob().IPAddress;
    m_process = new QProcess(parent);
    QString dir = QString("d:/___fenix/extractor/");
    m_process->setWorkingDirectory(dir);
    m_process->start(program, arguments);
    m_process->waitForStarted();
    connect (m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(onConsolProcessOutput()));
    connect (m_process, SIGNAL(readyReadStandardError()), this, SLOT(onConsolProcessOutput()));

    QMetaObject::invokeMethod(this, "processNextScreen", Qt::QueuedConnection);
}

void App::processNextScreen()
{
//    QTime bench; bench.start();

//    QTableWidgetItem *detectedItem = m_ui->tableWidget->item(1, 0);
//    detectedItem->setText(QString::number(m_screen->m_tableCounter->m_foundPoints.size()));

    m_screen->processNextScreen();

    //Item->setText(QString::number(m_screen->m_children.size() - 1));

//    int elapsed = bench.restart();
//    QString benchQString = QString::number(1000 / elapsed) + QString(" fps, ") + QString::number(elapsed)  + QString(" ms");
//    benchItem->setText(benchQString);

    m_screen->render();
    QMetaObject::invokeMethod(this, "processNextScreen", Qt::QueuedConnection);
}

void App::onConsolProcessOutput()
{
    qDebug() << "node script: " << m_process->readAll() << "\n";
}
