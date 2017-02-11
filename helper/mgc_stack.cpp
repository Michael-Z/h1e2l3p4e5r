#include "mgc_stack.h"

MGC_Stack::MGC_Stack(QString id, MGC_Element *parent, const QJsonObject &settings)
    : MGC_Recognizer(id, parent, settings)
{
    //QSound::play(glob().PROJECT_PATH + "sounds/alarm1.wav");
}

void MGC_Stack::storeCurrent()
{
    m_mask = m_elementMat;
}

bool MGC_Stack::wasStackChanged()
{
    return !recognizeMask();
}

