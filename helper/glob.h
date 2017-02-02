#ifndef GLOB_H
#define GLOB_H

#include <QString>

class Glob
{
public:
    Glob();
    QString PROJECT_PATH = "d:/___fenix/";
    QString ROOM_DATA_PATH;
};

Glob& glob();

#endif // GLOB_H
