#ifndef GAMESTAT_H
#define GAMESTAT_H

#include <QHash>
#include "h_street.h"
#include <QHash>
//#include <QObject>

class GameStat// : public QObject
{
    //Q_OBJECT
public:
    H_StreetId curStreet = StreetId::PREFLOP;
    QString heroHand = "";
    bool hasHeroButton = false;
    QHash<StreetId, QString> board;
    QHash<StreetId, QString> actionSequence;

    explicit GameStat();
    void reset();

//signals:

//public slots:
};

#endif // GAMESTAT_H
