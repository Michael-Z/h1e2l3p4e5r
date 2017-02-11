#ifndef GLOB_H
#define GLOB_H

#include "server.h"
#include <QString>

class Glob
{
public:
    Glob();
    QString PROJECT_PATH = "d:/___fenix/";
    QString ROOM_DATA_PATH;
    Server *m_server;
    QString IPAddress;
};

Glob& glob();

#endif // GLOB_H
