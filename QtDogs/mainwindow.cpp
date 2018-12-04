#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <bits/stdc++.h>
MainWindow::MainWindow(Model* myModel, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
        ui->setupUi(this);
        timer = new QTimer(this);
        connect(timer,SIGNAL(timeout()),this,SLOT(update()));
        timer->start(1000/60);
        connect(this, SIGNAL(getDogAnimationSignal(std::string, int)), &spriteSheetTool, SLOT(getAnimationFrame(std::string, int)));
        connect(&spriteSheetTool, SIGNAL(imageSendSignal(QImage)), this, SLOT(playDogAnimation(QImage)));

        spriteSheetTool.addAnimation(0, 0, 36, 28, 4, "Idle", "../QtDogs/assets/Dog.png");
        int frameNumber = 1;
        emit getDogAnimationSignal("Idle", frameNumber);

        //model connection
        connect(ui->petButton, &QPushButton::pressed, myModel, &Model::dogPetted);
        connect(ui->foodButton, &QPushButton::pressed, myModel, &Model::dogFed);
        connect(ui->ballButton, &QPushButton::pressed, myModel, &Model::dogPlayedWithBall);
        connect(ui->parkButton, &QPushButton::pressed, myModel, &Model::dogWentToThePark);
        connect(ui->letOutButton, &QPushButton::pressed, myModel, &Model::dogLetOut);
        connect(myModel, SIGNAL(updateTrustLevel(int)), this, SLOT(on_trustProgressBar_valueChanged(int)));
        connect(myModel, SIGNAL(updateHungerLevel(int)), this, SLOT(on_hungerProgressBar_valueChanged(int)));
        connect(myModel, SIGNAL(updateBathroomLevel(int)), this, SLOT(on_bathroomProgressBar_valueChanged(int)));
}

void MainWindow::playDogAnimation(QImage image){
   ui->imageLabel->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::update()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_trustProgressBar_valueChanged(int value)
{
    ui->trustProgressBar->setValue(value);
}

void MainWindow::on_hungerProgressBar_valueChanged(int value)
{
    ui->hungerProgressBar->setValue(value);
}

void MainWindow::on_bathroomProgressBar_valueChanged(int value)
{
    ui->bathroomProgressBar->setValue(value);
}
