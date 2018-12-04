#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(1000/60);
}

void MainWindow::update()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}
