#include "mgc_player.h"

MGC_Player::MGC_Player(QString &id, MGC_Element *parent, QJsonObject &settings)
    : MGC_Element(id, parent, settings)
{
    m_first_card = new MGC_Card(QString("first_card"), this, m_settings["first_card"].toObject());
    m_second_card = new MGC_Card(QString("second_card"), this, m_settings["second_card"].toObject());

    m_button = new MGC_Button(QString("button"), this, m_settings["button"].toObject());
    m_button->m_misrecMode = MisrecMode::SIMPLE_REQUESTING;
    m_button->setBorder("1", "solid", "red");
    m_button->m_drawMode = DrawMode::MASK;

    m_player_panel = new MGC_Element(QString("player_panel"), this, m_settings["player_panel"].toObject());
    m_player_type = new MGC_Recognizer(QString("player_type"), m_player_panel,
        m_settings["player_panel"].toObject()["player_type"].toObject());
    m_player_type->m_misrecMode = MisrecMode::SIMPLE_REQUESTING;

    m_stack = new MGC_Stack(QString("stack"), m_player_panel,
        m_settings["player_panel"].toObject()["stack"].toObject());
    m_stack->m_doDrawElementMat = true;
}

bool MGC_Player::processOnDown()
{
    refreshElementMat();
    m_player_panel->refreshElementMat();
    m_player_type->refreshElementMat();
    m_player_type->recognizeMaskInSet();

    if (m_player_type->m_recMask == "") {
        m_typeIsCovered = true;
        m_player_type->m_isCovered = true;
        m_player_type->m_isCoveredCounter++;
    } else {
        m_typeIsCovered = false;
        m_player_type->m_isCovered = false;
        m_player_type->m_isCoveredCounter = 0;

        if (m_player_type->m_recMask.left(5) == "empty") {
            m_type = PlayerType::EMPTY;
            m_isVisible = false;
            return true;
        }

        if (m_player_type->m_recMask.left(4) == "hero") {
            m_type = PlayerType::HERO;
            setBorder("5", "solid", "red");

        } else if (m_player_type->m_recMask.left(5) == "enemy") {
            m_type = PlayerType::ENEMY;
            setBorder("5", "solid", "black");

        } else {
            if (m_player_type->m_misrecMode != MisrecMode::SIMPLE_REQUESTING) throw "w";
        }
        m_isVisible = true;
    }
    return true;
}

void MGC_Player::processOnUp()
{
    if (!m_isVisible) return;

    m_button->recognizeMaskInSet();
    if (m_button->m_recMask == "") {
        m_button->m_isCovered = true;
        m_button->m_isCoveredCounter++;
    } else {
        m_button->m_isCovered = false;
        m_button->m_isCoveredCounter = 0;
        if (m_button->m_recMask.left(6) == "button") {
            m_hasButton = true;
        } else if (m_button->m_recMask.left(5) == "empty") {
            m_hasButton = false;
        } else {
            if (m_button->m_misrecMode != MisrecMode::SIMPLE_REQUESTING) throw "w";
        }
    }

    if (m_first_card->m_card == "" || m_second_card->m_card == "") {
        m_cards = "";
        m_cardsIsCovered = true;
        m_cardsIsCovered++;
    } else {
        m_cardsIsCovered = false;
        m_cardsIsCovered = 0;
        if (m_first_card->m_card == "EMPTY") {
            m_cards = "EMPTY";
        } else if (m_first_card->m_card == "HOLE") {
            m_cards = "HOLE";
        } else {
            m_cards = m_first_card->m_card + m_second_card->m_card;
        }
    }
}

