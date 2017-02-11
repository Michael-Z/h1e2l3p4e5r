#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QByteArray>
#include <QDataStream>

class Server : public QObject

{
    Q_OBJECT

public:

    explicit Server(QObject *parent = 0); // конструктор

signals:

public slots:

    void incommingConnection(); // обработчик входящего подключения
    void readyRead(); // обработчик входящих данных
    void stateChanged(QAbstractSocket::SocketState stat); // обработчик изменения состояния вещающего сокета (он нам важен, дабы у нас всегда был кто-то, кто будет вещать
    void sendMessage(QString &message);

private:

    QTcpServer *server; // указатель на сервер
    QList<QTcpSocket *> sockets; // получатели данных
    QTcpSocket *firstSocket; // вещатель

};

#endif // SERVER_H
