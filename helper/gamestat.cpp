#include "gamestat.h"

GameStat::GameStat()// : QObject()
{
    reset();
}

void GameStat::reset()
{
    actionSequence.insert(StreetId::PREFLOP, "");
    actionSequence.insert(StreetId::FLOP, "");
    actionSequence.insert(StreetId::TURN, "");
    actionSequence.insert(StreetId::RIVER, "");

    curStreet.val = StreetId::PREFLOP;
}
