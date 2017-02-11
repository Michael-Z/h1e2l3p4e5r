#ifndef MGC_PLAYER_H
#define MGC_PLAYER_H

#include "deps.h"

#include "mgc_element.h"
#include "mgc_card.h"
#include "mgc_recognizer.h"
#include "mgc_stack.h"
#include "mgc_button.h"

class MGC_Card;
class MGC_Recognizer;
class MGC_Element;

enum class PlayerType {HERO, ENEMY, EMPTY};

class MGC_Player : public MGC_Element
{
public:
    PlayerType m_type = PlayerType::EMPTY;
    bool m_typeIsCovered = false;
    QString m_cards = "";
    bool m_cardsIsCovered = false;
    int m_cardsIsCoveredCounter = 0;
    bool m_hasButton = false;

    MGC_Player(QString &id, MGC_Element *parent, QJsonObject &settings);
    virtual bool processOnDown() override;
    virtual void processOnUp() override;

public:
    MGC_Card *m_first_card = nullptr;
    MGC_Card *m_second_card  = nullptr;    
    MGC_Element *m_player_panel = nullptr;
    MGC_Recognizer *m_player_type = nullptr;
    MGC_Stack *m_stack  = nullptr;
    MGC_Button *m_button = nullptr;
signals:

public slots:

};

#endif // MGC_PLAYER_H
