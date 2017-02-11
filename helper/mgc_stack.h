#ifndef MGC_STACK_H
#define MGC_STACK_H

#include "mgc_recognizer.h"

class MGC_Stack : public MGC_Recognizer
{
public:    
    explicit MGC_Stack(QString id, MGC_Element *parent, const QJsonObject &settings);

    void storeCurrent();
    bool wasStackChanged();

private:

signals:

public slots:

};

#endif // MGC_STACK_H
