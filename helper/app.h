#ifndef APP_H
#define APP_H

#include "deps.h"
#include "mac_utility.h"

#include "mgc_screen.h"

#include "ui_mainwindow.h"
#include <QMainWindow>

#include <QObject>

class App : public QObject
{
    Q_OBJECT
public:
    explicit App(QMainWindow *parent = 0, Ui_MainWindow *ui = 0);

signals:

public slots:
    void processNextScreen();

private:
    QMainWindow *m_parent;
    Ui_MainWindow *m_ui;

    MGC_Screen *m_screen;

    QJsonObject m_settings;
    QJsonObject m_roomSettings;
};

#endif // APP_H
