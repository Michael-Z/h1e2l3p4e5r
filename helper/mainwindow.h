#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "deps.h"
#include "app.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    App *m_app;
};

#endif // MAINWINDOW_H
