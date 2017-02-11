#ifndef MGC_RECOGNIZEREXTERNALAREA_H
#define MGC_RECOGNIZEREXTERNALAREA_H

#include "mgc_recognizer.h"

class MGC_RecognizerExternalArea : public MGC_Recognizer
{
public:
    QRect m_elementExternalArea;

    MGC_RecognizerExternalArea(QString id, MGC_Element *parent, const QJsonObject &settings);
    virtual bool refreshElementMat() override;

signals:

public slots:

};



#endif // MGC_RECOGNIZEREXTERNALAREA_H
