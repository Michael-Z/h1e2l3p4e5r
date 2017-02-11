#include "solver.h"

Solver::Solver(QObject *parent) : QObject(parent)
{

    QString program = "d:/___fenix/piosolver/piosolver_pro_1032.exe";
    QStringList arguments;
//    //arguments << "-style" << "fusion";

    QProcess *myProcess = new QProcess(parent);
    QString dir = QString("D:/___fenix/piosolver/");
    myProcess->setWorkingDirectory(dir);
    myProcess->start(program, arguments);
    myProcess->waitForStarted();

    myProcess->write("is_ready\n");
    myProcess->waitForBytesWritten();
    myProcess->waitForReadyRead();
    auto asd = myProcess->readAllStandardOutput();
    qDebug() << asd << "\n";
    myProcess->kill();
    myProcess->waitForFinished();
    //myProcess->readLine();

//    QProcess *myProcess = new QProcess(parent);
//    myProcess->start(program, arguments);
//    myProcess->waitForStarted();
//    myProcess->write("is_ready\n");
//    myProcess->waitForBytesWritten();
//    myProcess->readLine();

}
