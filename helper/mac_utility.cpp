#include "mac_utility.h"

MAC_Utility::MAC_Utility()
{
}

QJsonDocument MAC_Utility::readQJsonFromFile(const QString &fileName)
{
    QString val;
    QFile file;
    file.setFileName(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    return QJsonDocument::fromJson(val.toUtf8());
}


