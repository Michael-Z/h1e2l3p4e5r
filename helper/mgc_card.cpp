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

    if (m_suit->m_recognizedMaskName.left(5) == QString("empty")) {
        m_isVisible = false;
        return;
    } else {
        //throw "Need new mask?";
    }
    m_value->recognizeMaskInSets(m_suit->m_recognizedMaskName);
    m_isVisible = true;
}
