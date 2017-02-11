#ifndef MGC_SCREEN_H
#define MGC_SCREEN_H

#include "deps.h"

#include "mgc_element.h"
#include "mgc_recognizer.h"
#include "mgc_table.h"

#include <QLabel>
#include <QQueue>
#include <QScreen>
#include <QGuiApplication>

typedef QQueue<Mat> QueueOfMat_t;

class MGC_Screen : public MGC_Element
{
public:
    MGC_Recognizer *m_tableCounter;
    int m_addedTableCounter = 0;

    explicit MGC_Screen(QString id, QWidget *widgetParent, const QJsonObject &settings, QJsonObject &tableSettings);

    virtual bool processOnDown() override;
    virtual void processOnUp() override;

    virtual bool refreshElementMat() override;
    void takeRootScreenshot();
    void loadScreensFromFolder(QString &path);
    virtual void show() override;

private:
    QueueOfMat_t m_screensQueue;
    QScreen *m_primaryScreen;
    QJsonObject m_tableSettings;

signals:

public slots:

};

#endif // MGC_SCREEN_H
