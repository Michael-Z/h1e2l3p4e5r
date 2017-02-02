#ifndef MAC_UTILITY_H
#define MAC_UTILITY_H

#include "deps.h"

#include <QString>
#include <QIODevice>
#include <QFile>
#include <QJsonDocument>

class MAC_Utility
{
public:
    MAC_Utility();

    static QJsonDocument readQJsonFromFile(const QString &fileName);    
};

#endif // MAC_UTILITY_H
