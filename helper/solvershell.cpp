#include "solvershell.h"

SolverShell::SolverShell(MGC_Table *table) : QObject() , m_table(table)
{
    m_server = glob().m_server;
    //QSound::play(glob().PROJECT_PATH + "sounds/alarm1.wav");
}

SolverShell::~SolverShell()
{

}

void SolverShell::onGameIsOn(GameStat *gs)
{
    //Solver *solver = new Solver();
    QString msg = "hero_got_hand|";
    msg += packMessage(gs);
    m_server->sendMessage(msg);
}

void SolverShell::onNewStreet(GameStat *gs)
{
    QString msg = "new_street|";
    msg += packMessage(gs);
    m_server->sendMessage(msg);
}

void SolverShell::onHeroIsActing(GameStat *gs)
{
    QString msg = "hero_is_acting|";
    msg += packMessage(gs);
    m_server->sendMessage(msg);
}

void SolverShell::onGameIsOver(GameStat *gs)
{
    QString msg = "game_is_over|";
    msg += packMessage(gs);
    m_server->sendMessage(msg);
}

QString SolverShell::packMessage(GameStat *gs)
{
     QString msg;
     msg += gs->heroHand;
     msg += "|";
     if (gs->hasHeroButton) {
         msg += "BUTTON";
     } else {
         msg += "NO_BUTTON";
     }
     msg += "|";
     msg += gs->curStreet.toString();
     msg += "|";
     msg += gs->actionSequence[StreetId::PREFLOP] + "/";
     msg += gs->actionSequence[StreetId::FLOP] + "/";
     msg += gs->actionSequence[StreetId::TURN] + "/";
     msg += gs->actionSequence[StreetId::RIVER];
     msg += "|";
     msg += gs->board[StreetId::FLOP] + "/";
     msg += gs->board[StreetId::TURN] + "/";
     msg += gs->board[StreetId::RIVER];
     return msg;
}
