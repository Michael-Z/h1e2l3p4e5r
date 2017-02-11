#ifndef H_STREET_H
#define H_STREET_H

#include <QHash>

enum class StreetId {BEGIN, PREFLOP, FLOP, TURN, RIVER, END};

class H_StreetId
{
public:    
    StreetId val;

    H_StreetId(StreetId currentAgr = StreetId::PREFLOP);

    StreetId changeToPrevious();
    StreetId changeToNext();

    StreetId getPrevious();
    StreetId getNext();

    QString toString();

private:


};

//inline bool operator==(const StreetId& mc1, const StreetId& mc2)
//{
//     return (mc1 == mc2);
//}

inline uint qHash(const StreetId& mc)
{
     return qHash(int(mc));
}

#endif // H_STREET_H
