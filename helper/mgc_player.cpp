#include "mgc_player.h"

MGC_Player::MGC_Player(QString &id, MGC_Element *parent, QJsonObject &settings)
    : MGC_Element(id, parent, settings)
{
    m_first_card = new MGC_Card(QString("first_card"), this, m_settings["first_card"].toObject());
    m_second_card = new MGC_Card(QString("second_card"), this, m_settings["second_card"].toObject());

    m_player_panel = new MGC_Element(QString("player_panel"), this, m_settings["player_panel"].toObject());
    m_player_type = new MGC_Recognizer(QString("player_type"), m_player_panel,
                                    m_settings["player_panel"].toObject()["player_type"].toObject());
    m_player_type->m_misrecMode = MisrecMode::SIMPLE_REQUESTING;
}

void MGC_Player::processOnUp()
{

}

bool MGC_Player::processOnDown()
{
    refreshElementMat();
    m_player_panel->refreshElementMat();
    m_player_type->refreshElementMat();
    m_player_type->recognizeMaskInSet();

    if (m_player_type->m_recognizedMaskName.left(4) == "hero") {
       setBorder("5", "solid", "red");
       m_isVisible = true;
       return true;
    } else if (m_player_type->m_recognizedMaskName.left(5) == "enemy") {
        setBorder("5", "solid", "black");
        m_isVisible = true;
        return true;
    } else {
        m_isVisible = false;
        return false;
    }

    return true;
}
