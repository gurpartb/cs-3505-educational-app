#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <bits/stdc++.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this, SIGNAL(getDogAnimationSignal(std::string, int)), &spriteSheetTool, SLOT(getAnimationFrame(std::string, int)));
    connect(&spriteSheetTool, SIGNAL(imageSendSignal(QImage)), this, SLOT(playDogAnimation(QImage)));

    spriteSheetTool.addAnimation(0, 0, 36, 28, 4, "Idle", "/home/sunsun/Desktop/CS_3505/A8/a8-an-educational-app-f18-gurpartb/QtDogs/assets/Dog.png");
    int frameNumber = 1;
    emit getDogAnimationSignal("Idle", frameNumber);

}

void MainWindow::playDogAnimation(QImage image){
    ui->DogLabel->setPixmap(QPixmap::fromImage(image));
}

MainWindow::~MainWindow()
{
    delete ui;
}
