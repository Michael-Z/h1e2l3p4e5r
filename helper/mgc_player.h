#ifndef MGC_PLAYER_H
#define MGC_PLAYER_H

#include "deps.h"

#include "mgc_element.h"
#include "mgc_card.h"
#include "mgc_recognizer.h"

class MGC_Card;
class MGC_Recognizer;
class MGC_Element;

class MGC_Player : public MGC_Element
{
public:
    MGC_Player(QString &id, MGC_Element *parent, QJsonObject &settings);
    virtual bool processOnDown() override;
    virtual void processOnUp() override;

private:
    MGC_Recognizer *m_place_type;
    bool m_isHero = false;

    MGC_Card *m_first_card = nullptr;
    MGC_Card *m_second_card  = nullptr;
    MGC_Element *m_player_panel = nullptr;
    MGC_Recognizer *m_player_type = nullptr;
};

#endif // MGC_PLAYER_H
