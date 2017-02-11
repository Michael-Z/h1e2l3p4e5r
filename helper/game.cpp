#include "game.h"

Game::Game(MGC_Table *table, SolverShell *solverShell) : QObject(), m_table(table), m_solverShell(solverShell)
{
    //    QSound::play(glob().PROJECT_PATH + "sounds/alarm1.wav");

//    m_widget = new QListWidget (parent->m_elementWidget);
//    m_widget->resize(QSize(300, 200));
//    m_widget->move(QPoint(10, m_table->m_elementArea.height() - m_widget->height() - 5));
//    m_widget->show();
//    m_rec = new QListWidgetItem(m_widget);
}


void Game::process() {

    backToSwitch: switch(m_waitingForStat) {

    case WaitingForStat::GAME_END:
    {
        int activePlayerAmount = 0;
        m_hero = nullptr;
        m_enemy = nullptr;
        for(MGC_Player *curPlayer: m_table->m_players) {
            if (curPlayer->m_type == PlayerType::EMPTY || curPlayer->m_typeIsCovered || curPlayer->m_cards == "EMPTY") continue;
            ++activePlayerAmount;
            if (curPlayer->m_cardsIsCovered) {

                return;
            }
            if (curPlayer->m_cardsIsCovered) { // || curPlayer->m_cards != "EMPTY"

                return;
            }
            if (curPlayer->m_type == PlayerType::HERO) {
                m_hero = curPlayer;
            } else if (curPlayer->m_type == PlayerType::ENEMY) {
                m_enemy = curPlayer;
            } else throw "w?";
        }
        if (activePlayerAmount < 2) {

            return;
        } else if (activePlayerAmount > 2) {

            return;
        }
        m_waitingForStat = WaitingForStat::HERO_GETS_CARDS;
        goto backToSwitch;
    }


    case WaitingForStat::HERO_GETS_CARDS:
    {
        if(m_hero == nullptr) return;
        if (m_hero->m_cards == "" || m_hero->m_cards == "EMPTY") {

            return;
        }
        if (m_hero->m_cards == "HOLE") throw "w";

        gi.heroActCounter = 0;
        gi.heroIsChoosing = false;
        gi.enemyWasStackChanged = false;
        gi.heroWasStackChanged = false;
        gi.heroActionsIncludeFold = false;

        m_gs.reset();

        m_gs.hasHeroButton = m_hero->m_hasButton;
        m_gs.heroHand = m_hero->m_cards;

        m_solverShell->onGameIsOn(&m_gs);

        m_waitingForStat = WaitingForStat::SOME_ACTION;
        goto backToSwitch;
    }

    case WaitingForStat::SOME_ACTION:
    {
        if (gi.heroActCounter != 0) {
            bool wasNewStreetOpened;
            if (m_gs.curStreet.getNext() == StreetId::END) {
                wasNewStreetOpened = false;
            } else {
                MGC_Card *card = m_table->m_board.at(int(m_gs.curStreet.getNext()));
                if (card->m_isCovered || card->m_card == "EMPTY") {
                    wasNewStreetOpened = false;
                } else {
                    m_gs.curStreet.changeToNext();
                    if (m_gs.curStreet.val == StreetId::FLOP) {
                        MGC_Card *firstCard = m_table->m_board.at(0);
                        MGC_Card *secondCard = m_table->m_board.at(1);
                        m_gs.board[m_gs.curStreet.val] = firstCard->m_card + secondCard->m_card + card->m_card;
                    } else {
                        m_gs.board[m_gs.curStreet.val] = card->m_card;
                    }
                    wasNewStreetOpened = true;
                }
            }
            if (wasNewStreetOpened) {

                gi.enemyWasStackChanged = m_enemy->m_stack->wasStackChanged();
                if (gi.enemyWasStackChanged) {
                    if (gi.heroActCounter > 0) gi.heroWasStackChanged = m_hero->m_stack->wasStackChanged();
                    gi.heroIsChoosing = false;
                }

                buildActionString(GameEventType::NEW_STREET);
                m_solverShell->onNewStreet(&m_gs);

                gi.heroActCounter = 0;
                gi.heroIsChoosing = false;
                gi.enemyWasStackChanged = false;
                gi.heroWasStackChanged = false;
            }
        }

        if (!gi.heroIsChoosing) {
            if (m_table->m_call_button->m_isVisible) {
                gi.heroActionsIncludeFold = m_table->m_fold_button->m_isVisible;
                buildActionString(GameEventType::HERO_IS_ACTING);
                m_solverShell->onHeroIsActing(&m_gs);
                gi.heroActCounter++;
                gi.heroIsChoosing = true;
                gi.enemyWasStackChanged = false;
                gi.heroWasStackChanged = false;
                m_enemy->m_stack->storeCurrent();
                m_hero->m_stack->storeCurrent();
            }

        } else {
            gi.enemyWasStackChanged = m_enemy->m_stack->wasStackChanged();
            if (gi.enemyWasStackChanged) {
                if (gi.heroActCounter > 0) gi.heroWasStackChanged = m_hero->m_stack->wasStackChanged();
                gi.heroIsChoosing = false;
            }

        }
        m_waitingForStat = WaitingForStat::SOME_ACTION_EXT;
        return; //go for next screen
    }

    case WaitingForStat::SOME_ACTION_EXT:
    {
        if (m_hero->m_cards == "EMPTY") {
            gi.heroIsChoosing = false;
            gi.enemyWasStackChanged = false;
            gi.heroWasStackChanged = false;
            gi.heroActionsIncludeFold = false;

            m_gs.curStreet.val = StreetId::BEGIN;
            m_solverShell->onGameIsOver(&m_gs);

//            m_rec = new QListWidgetItem(m_widget);

            m_waitingForStat = WaitingForStat::GAME_END;
            goto backToSwitch;
        }

        m_waitingForStat = WaitingForStat::SOME_ACTION;
        goto backToSwitch;
    }
        default: throw "w";
    }
}

void Game::buildActionString(GameEventType gameEventType) {

    StreetId previousStreet = m_gs.curStreet.getPrevious();

    switch (gameEventType) {

        case GameEventType::HERO_IS_ACTING:

            if (actionBuildingCase(m_gs.curStreet.val)) {

                if (gi.heroActCounter == 0) {
                    //nothing
                } else if (gi.heroActCounter == 1) {
                        if (gi.heroWasStackChanged) {
                            m_gs.actionSequence[m_gs.curStreet.val] += "rr";
                        } else {
                            m_gs.actionSequence[m_gs.curStreet.val] += "cr";
                        }
                } else {
                    m_gs.actionSequence[m_gs.curStreet.val] += "rr";
                }

            } else {

                if (gi.heroActCounter == 0) {
                    if (gi.heroActionsIncludeFold) {
                        m_gs.actionSequence[m_gs.curStreet.val] += "r";
                    } else {
                        m_gs.actionSequence[m_gs.curStreet.val] += "c";
                    }
                } else {
                    m_gs.actionSequence[m_gs.curStreet.val] += "rr";
                }

            }

            break;

        case GameEventType::NEW_STREET:

            if (actionBuildingCase(previousStreet)) {
                if (gi.heroActCounter == 0) {
                    throw "ERROR: how a new street was appeared if hero does zero action";
                } else if (gi.heroActCounter == 1) {
                    if (gi.heroWasStackChanged) {
                        m_gs.actionSequence[previousStreet] += "rc";
                    } else {
                        m_gs.actionSequence[previousStreet] += "cc";
                    }
                } else {
                    if (gi.heroIsChoosing) {
                        m_gs.actionSequence[previousStreet] += "c";
                    } else {
                        m_gs.actionSequence[previousStreet] += "rc";
                    }
                }

            } else {

                if (gi.heroIsChoosing) {
                    m_gs.actionSequence[previousStreet] += "c";
                } else {
                    m_gs.actionSequence[previousStreet] += "rc";
                    if (! (gi.enemyWasStackChanged && gi.heroWasStackChanged) ) throw "ERROR: why stacks did change?";
                }

            }

            break;

        default: throw "w";
    }

}

bool Game::actionBuildingCase(StreetId streetId) {
    return ( (streetId == StreetId::PREFLOP) && (m_gs.hasHeroButton == true) )
        || ( (streetId != StreetId::PREFLOP) && (m_gs.hasHeroButton == false) );
}
