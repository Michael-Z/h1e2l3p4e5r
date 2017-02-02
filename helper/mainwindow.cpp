#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Helper");
    m_app = new App(this, ui);
}

MainWindow::~MainWindow()
{
    delete ui;
}
