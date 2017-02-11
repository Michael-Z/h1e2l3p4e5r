#include "mgc_recognizerexternalarea.h"

MGC_RecognizerExternalArea::MGC_RecognizerExternalArea(QString id, MGC_Element *parent,
        const QJsonObject &settings) : MGC_Recognizer(id, parent, settings)
{
    m_elementExternalArea = convertQJsonArray2QRect(m_settings["element_external_area"].toArray());
}

bool MGC_RecognizerExternalArea::refreshElementMat()
{
    Mat &parentMat = m_parent->m_parent->m_elementMat;
    if (parentMat.cols < (m_elementExternalArea.x() + m_elementExternalArea.width())
            || parentMat.rows < (m_elementExternalArea.y() + m_elementExternalArea.height())) return false;

    m_elementMat = parentMat(convertQRect2Rect(m_elementExternalArea));
    return true;
}

