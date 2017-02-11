#ifndef MGC_TABLE_H
#define MGC_TABLE_H

#include "deps.h"

#include "mgc_element.h"
#include "mgc_recognizer.h"
#include "mgc_player.h"
#include "mgc_screen.h"
#include "solvershell.h"
#include "game.h"

class MGC_Player;
class Game;

class MGC_Table : public MGC_Element
{
public:

    MGC_Recognizer *m_tableId = nullptr;
    MGC_Recognizer *m_baseOffsetSign = nullptr;
    QPoint m_baseOffsetSignOffset;

    MGC_Table(QString id, MGC_Element *parent,
              const QJsonObject &settings, const QPoint &singPoint);
    virtual ~MGC_Table() override;

    virtual bool processOnDown() override;
    virtual void processOnUp() override;

public:

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

    Game *m_game = nullptr;
    SolverShell *m_solverShell = nullptr;

signals:

public slots:

};

#endif // MGC_TABLE_H
