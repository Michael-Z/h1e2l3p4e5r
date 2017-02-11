#ifndef GAME_H
#define GAME_H

#include "mgc_table.h"

#include "h_street.h"
#include "gamestat.h"
#include "solvershell.h"

#include <QListWidget>

#include <QObject>

class SolverShell;

enum class GameEventType {GAME_IS_ON, GAME_IS_OVER, NEW_STREET, HERO_IS_ACTING};

enum class WaitingForStat {GAME_END, HERO_GETS_CARDS, SOME_ACTION, SOME_ACTION_EXT};

class MGC_Table;

class Game : public QObject
{
    Q_OBJECT
public:

    struct GameIndicators {
        int heroActCounter;
        bool heroIsChoosing;
        bool enemyWasStackChanged;
        bool heroWasStackChanged;
        bool heroActionsIncludeFold;
    } gi;

    GameStat m_gs;

    WaitingForStat m_waitingForStat = WaitingForStat::GAME_END;

    explicit Game(MGC_Table *table, SolverShell *solverShell);    
    void process();

    SolverShell *m_solverShell;

signals:

public slots:

private:
    MGC_Table *m_table  = nullptr;
    MGC_Player *m_hero = nullptr;
    MGC_Player *m_enemy = nullptr;

    QListWidget *m_widget; //QTextEdit *m_widget;
    QListWidgetItem *m_rec;


    void buildActionString(GameEventType gameEventType);
    bool actionBuildingCase(StreetId streetId);

    bool checkPlayers();
};




#endif // GAME_H
