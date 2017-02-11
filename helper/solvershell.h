#ifndef SOLVERSHELL_H
#define SOLVERSHELL_H

#include "deps.h"

#include "solver.h"
#include "server.h"
#include "mgc_table.h"
#include "gamestat.h"
#include <QObject>

#include <QDebug>

class MGC_Table;

class SolverShell : public QObject
{
    Q_OBJECT
public:
    explicit SolverShell(MGC_Table *table);
    ~SolverShell();
    void onGameIsOn(GameStat *gs);
    void onNewStreet(GameStat *gs);
    void onHeroIsActing(GameStat *gs);
    void onGameIsOver(GameStat *gs);

    MGC_Table *m_table;
    Server *m_server;

signals:

public slots:

private:
    QString packMessage(GameStat *gs);
};

#endif // SOLVERSHELL_H
