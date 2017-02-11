#include "server.h"

Server::Server(QObject *parent) : QObject(parent), firstSocket(NULL)
{
    server = new QTcpServer(this);
    server->listen(QHostAddress::LocalHost, 666);
    connect(server, &QTcpServer::newConnection, this, &Server::incommingConnection); // подключаем сигнал "новое подключение" к нашему обработчику подключений    
}

void Server::incommingConnection() // обработчик подключений
{
    QTcpSocket * socket = server->nextPendingConnection(); // получаем сокет нового входящего подключения
    connect(socket, &QTcpSocket::stateChanged, this, &Server::stateChanged); // делаем обработчик изменения статуса сокета //QAbstractSocket::SocketState
    connect(socket, &QTcpSocket::readyRead, this, &Server::readyRead); // подключаем входящие сообщения от вещающего на наш обработчик
    //socket->write("wellcome to helper!\n");
    sockets << socket;
}

void Server::readyRead() // обработчик входящих сообщений от "вещающего"
{
    QObject * object = QObject::sender(); // далее и ниже до цикла идет преобразования "отправителя сигнала" в сокет, дабы извлечь данные
    if (!object)
        return;
    QTcpSocket * socket = static_cast<QTcpSocket *>(object);
    QByteArray arr =  socket->readAll();
}

void Server::stateChanged(QAbstractSocket::SocketState state) // обработчик статуса, нужен для контроля за "вещающим"
{
//    QObject * object = QObject::sender();
//    if (!object)
//        return;
//    QTcpSocket * socket = static_cast<QTcpSocket *>(object);
    qDebug() << state;
    //if (state == QAbstractSocket::UnconnectedState) delete socket;
}

void Server::sendMessage(QString &message)
{
    message += ">";
    QByteArray &arr = message.toUtf8();
    // на самом деле весь верхний код можно было заменить на firstSocket, но я выдирал код из другого проекта, и переписывать мне лень :)
    for(QTcpSocket *socket: sockets) { // пишем входящие данные от "вещающего" получателям
        if (socket->state() == QTcpSocket::ConnectedState) {
            socket->write(arr);
            socket->flush();
            socket->waitForBytesWritten();
        }
    }
}
