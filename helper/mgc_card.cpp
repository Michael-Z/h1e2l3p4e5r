#include "mgc_card.h"

MGC_Card::MGC_Card(QString &id, MGC_Element *parent, QJsonObject &settings)
    : MGC_Element(id, parent, settings)
{
    m_suit = new MGC_Recognizer(QString("suit"), this, m_settings["suit"].toObject());
    m_value = new MGC_Recognizer(QString("value"), this, m_settings["value"].toObject());
    m_suit->m_drawMode = DrawMode::MASK;
    m_value->m_drawMode = DrawMode::MASK;

    m_suit->m_misrecMode = MisrecMode::SIMPLE_REQUESTING;
}

void MGC_Card::processOnUp()
{
    m_suit->recognizeMaskInSet();

    if (m_suit->m_recMask == "") {
        m_isVisible = false;
        m_card = "";
        m_isCovered = true;
        m_isCoveredCounter++;
        return;
    }
    m_isCovered = false;
    m_isCoveredCounter = 0;
    if (m_suit->m_recMask.left(5) == QString("empty")) {
        m_isVisible = false;
        m_card = "EMPTY";
        return;
    } else if (m_suit->m_recMask.left(4) == QString("hole")) {
        m_isVisible = true;
        m_card = "HOLE";
        m_value->m_recMask = "";
        m_value->m_isVisible = false;
        return;
    }
    m_value->recognizeMaskInSets(m_suit->m_recMask);
    if (m_value->m_recMask == "") {
        m_isVisible = false;
        m_card = "";
        m_isCovered = true;
        m_isCoveredCounter++;
        return;
    }
    m_card = m_value->m_recMask + m_suit->m_recMask;    
    m_value->m_isVisible = true;
    m_isVisible = true;
}
