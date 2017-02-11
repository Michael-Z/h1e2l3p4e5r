#ifndef MGC_CARD_H
#define MGC_CARD_H

#include "deps.h"

#include "mgc_player.h"
#include "mgc_element.h"
#include "mgc_recognizer.h"

//class MGC_Card;
//class MGC_Recognizer;

class MGC_Card : public MGC_Element
{
public:
    QString m_card = "";
    MGC_Card(QString &id, MGC_Element *parent, QJsonObject &settings);

    MGC_Recognizer *m_suit = nullptr;
    MGC_Recognizer *m_value = nullptr;

    virtual void processOnUp() override;
};

#endif // MGC_CARD_H
