#include "mgc_table.h"

MGC_Table::MGC_Table(QString id, MGC_Element *parent, const QJsonObject &settings, const QPoint &singPoint)
    : MGC_Element(id, parent, settings)
{    
    m_isVisible = false;

    m_destroy_after_disable = m_settings["destroy_after_disable"].toInt();

    QJsonObject baseSingObj = m_settings["base_offset_sign"].toObject();    
    m_baseSingLookupArea = convertQJsonArray2QRect(baseSingObj["element_area"].toArray());

    m_elementArea.moveTo(singPoint.x(), singPoint.y());

    if (!refreshElementMat()) return;

    MGC_Recognizer *tableIdSignElement = new MGC_Recognizer(QString("table_id_sign"), this, m_settings["table_id_sign"].toObject());
    tableIdSignElement->refreshElementMat();
    tableIdSignElement->findPoints();    
    if (tableIdSignElement->m_foundPoints.size() != 1) {
        delete tableIdSignElement;
        return;
    }
    m_tableId = new MGC_Recognizer(QString("table_id"), this, m_settings["table_id"].toObject());
    m_tableId->m_elementArea.moveLeft(m_tableId->m_elementArea.x() + tableIdSignElement->m_foundPoints[0].x() + tableIdSignElement->m_elementArea.x());
    m_tableId->m_elementArea.moveTop(m_tableId->m_elementArea.y() + tableIdSignElement->m_foundPoints[0].y() + tableIdSignElement->m_elementArea.y());
    m_tableId->refreshElementMat();
    m_tableId->addMaskFromElementMat();    
    m_tableId->doDrawElementMat = true;
    delete tableIdSignElement;

    m_baseOffsetSign = new MGC_Recognizer(QString("base_offset_sign"), this, m_settings["base_offset_sign"].toObject());
    m_baseOffsetSignOffset.setX(m_baseOffsetSign->m_elementArea.x());
    m_baseOffsetSignOffset.setY(m_baseOffsetSign->m_elementArea.y());
    m_baseOffsetSign->m_elementArea.moveTo(m_baseOffsetSignOffset);
    m_baseOffsetSign->m_elementArea.setWidth(m_baseOffsetSign->m_mask.cols);
    m_baseOffsetSign->m_elementArea.setHeight(m_baseOffsetSign->m_mask.rows);
    m_baseOffsetSign->doDrawElementMat = true;

    QJsonObject tableMAX6Settings = m_settings["MAX6"].toObject();

    QJsonObject playersSettings = tableMAX6Settings["players"].toObject();
    MGC_Player *playerElement;
    for (int i = 0; i < 6; ++i) {
        QString playerIdString = QString("player_") + QString::number(i);
        playerElement = new MGC_Player(playerIdString, this, playersSettings[playerIdString].toObject());        
        m_players.append(playerElement);
    }

    QJsonObject boardSettings = m_settings["board"].toObject();
    MGC_Card *boardCardElement;
    for (int i = 0; i < 5; ++i) {
        QString boardCardIdString = QString("board_") + QString::number(i);
        boardCardElement = new MGC_Card(boardCardIdString, this, boardSettings[boardCardIdString].toObject());        
        m_board.append(boardCardElement);
    }
    m_raise_button = new MGC_Element("raise_button", this, m_settings["raise_button"].toObject());
    m_call_button = new MGC_Element("call_button", this, m_settings["call_button"].toObject());
    m_fold_button = new MGC_Element("fold_button", this, m_settings["fold_button"].toObject());

    m_call_button_sign = new MGC_Recognizer("call_button_sign", m_call_button,
        m_settings["call_button"].toObject()["call_button_sign"].toObject());
    m_fold_button_sign = new MGC_Recognizer("fold_button_sign", m_fold_button,
        m_settings["fold_button"].toObject()["fold_button_sing"].toObject());

    setBorder("7", "solid", "black");
}

bool MGC_Table::processOnDown()
{    
    MGC_Recognizer *tableCounter = static_cast<MGC_Recognizer*> (m_parent->m_children[QString("tableCounter")]);
    QList<QPoint> &foundSignPoints = tableCounter->m_foundPoints;
    if (!m_tableId) throw "WHY? Trying to process screen with table that isn't identified";
    m_isVisible = false;
    QList<QPoint>::iterator it = foundSignPoints.begin();
    QPoint singPoint;
    while (it != foundSignPoints.end()) {
        singPoint = *it;
        m_elementArea.moveTo(singPoint.x(), singPoint.y());
        if (!refreshElementMat()) {
            ++it;
            continue;
        }
        m_tableId->refreshElementMat();
        if (!m_tableId->recognizeMask()) {
            ++it;
            continue;
        }
        m_isVisible = true;
        m_disappearedCounter = 0;
        m_baseOffsetSign->m_elementArea.moveTo(m_baseOffsetSignOffset.x(), m_baseOffsetSignOffset.y());
        it = foundSignPoints.erase(it);
        break;
    }

    if (!m_isVisible) {
        ++m_disappearedCounter;
        if (m_disappearedCounter > m_destroy_after_disable) {
            this->deleteLater();
        }
        return false;
    }    

    m_call_button->refreshElementMat();
    m_fold_button->refreshElementMat();

    m_call_button_sign->refreshElementMat();
    m_fold_button_sign->refreshElementMat();

    m_call_button_sign->recognizeMaskInSet();
    m_fold_button_sign->recognizeMaskInSet();

    if (m_call_button_sign->m_recognizedMaskName == "action" &&
            m_fold_button_sign->m_recognizedMaskName == "action") {
        m_raise_button->setBorder("5", "solid", "black");
        m_call_button->setBorder("5", "solid", "black");
        m_fold_button->setBorder("5", "solid", "black");
    } else {
        m_raise_button->setBorder("1", "solid", "black");
        m_call_button->setBorder("1", "solid", "black");
        m_fold_button->setBorder("1", "solid", "black");
    }

    return true;
}

void MGC_Table::processOnUp()
{

}

