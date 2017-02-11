#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>

#include "deps.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCommandLineParser parser;
    QCommandLineOption targetDirectoryOption(
                QStringList() << "ip" << "server IP address",
                "", "address", "127.0.0.1:23");
    parser.addOption(targetDirectoryOption);
    parser.process(a);
    glob().IPAddress = parser.value(targetDirectoryOption);

    MainWindow w;
    w.show();

    return a.exec();
}
