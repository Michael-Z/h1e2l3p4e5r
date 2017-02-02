#ifndef MGC_TABLE_H
#define MGC_TABLE_H

#include "deps.h"

#include "mgc_element.h"
#include "mgc_recognizer.h"
#include "mgc_player.h"
#include "mgc_screen.h"

class MGC_Player;

class MGC_Table : public MGC_Element
{
public:            
    MGC_Recognizer *m_tableId = nullptr;
    MGC_Recognizer *m_baseOffsetSign = nullptr;
    QPoint m_baseOffsetSignOffset;

    int m_disappearedCounter = 0;

    MGC_Table(QString id, MGC_Element *parent,
              const QJsonObject &settings, const QPoint &singPoint);    

    virtual bool processOnDown() override;
    virtual void processOnUp() override;

private:

    enum class TableGameSizeType {HU, MAX6, MAX9};
    TableGameSizeType tableGameSize = TableGameSizeType::MAX6;
    QList<MGC_Player*> m_players;
    QList<MGC_Card*> m_board;

    MGC_Element *m_raise_button;
    MGC_Element *m_call_button;
    MGC_Element *m_fold_button;

    MGC_Recognizer *m_call_button_sign;
    MGC_Recognizer *m_fold_button_sign;

    QRect m_baseSingLookupArea;
    int m_destroy_after_disable;
};

#endif // MGC_TABLE_H
