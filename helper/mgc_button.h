#ifndef MGC_BUTTON_H
#define MGC_BUTTON_H

#include "mgc_recognizerexternalarea.h"

class MGC_Button : public MGC_RecognizerExternalArea
{
public:
    MGC_Button(QString id, MGC_Element *parent, const QJsonObject &settings);

};

#endif // MGC_BUTTON_H
