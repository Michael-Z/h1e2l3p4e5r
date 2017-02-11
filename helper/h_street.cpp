#include "h_street.h"

H_StreetId::H_StreetId(StreetId currentAgr)
{
    val = currentAgr;
}

StreetId H_StreetId::getPrevious()
{
    if (val == StreetId::BEGIN) return val;
    return static_cast<StreetId>(int(val) - 1);
}

StreetId H_StreetId::getNext()
{
    if (val == StreetId::END) return val;
    return static_cast<StreetId>(int(val) + 1);
}

StreetId H_StreetId::changeToPrevious()
{
    return val = getPrevious();
}

StreetId H_StreetId::changeToNext()
{
    return val = getNext();
}

QString H_StreetId::toString()
{
    switch (val) {
    case StreetId::BEGIN:
        return "begin";
    case StreetId::PREFLOP:
        return "preflop";
    case StreetId::FLOP:
        return "flop";
    case StreetId::TURN:
        return "turn";
    case StreetId::RIVER:
        return "river";
    case StreetId::END:
        return "end";
    }
    throw "w"; return "error";
}

