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
        timer->start(1000 / 3);
        connect(this, SIGNAL(getDogAnimationSignal(std::string, int)), &spriteSheetTool, SLOT(getAnimationFrame(std::string, int)));
        //connect(&spriteSheetTool, SIGNAL(imageSendSignal(sf::Texture)), this, SLOT(playDogAnimation(sf::Texture)));

        //model connection
        connect(ui->petButton, &QPushButton::pressed, myModel, &Model::dogPetted);
        connect(ui->foodButton, &QPushButton::pressed, myModel, &Model::dogFed);
        connect(ui->ballButton, &QPushButton::pressed, myModel, &Model::dogPlayedWithBall);
        connect(ui->parkButton, &QPushButton::pressed, myModel, &Model::dogWentToThePark);
        connect(ui->letOutButton, &QPushButton::pressed, myModel, &Model::dogLetOut);
        connect(myModel, SIGNAL(updateTrustLevel(int)), this, SLOT(on_trustProgressBar_valueChanged(int)));
        connect(myModel, SIGNAL(updateHungerLevel(int)), this, SLOT(on_hungerProgressBar_valueChanged(int)));
        connect(myModel, SIGNAL(updateBathroomLevel(int)), this, SLOT(on_bathroomProgressBar_valueChanged(int)));

        std::string dogPath = "../QtDogs/assets/Dog.png";

        //adding animation frames
        spriteSheetTool.addAnimation(0, 0, 36, 28, 4,      "Dog_Idle",       dogPath);
        spriteSheetTool.addAnimation(4, 28, 38, 27, 15,    "Dog_Sitting",    dogPath);
        spriteSheetTool.addAnimation(4, 59, 38, 26, 13,    "Dog_Barking",    dogPath);
        spriteSheetTool.addAnimation(4, 138, 38, 26, 23,   "Dog_Peeing",     dogPath);
        spriteSheetTool.addAnimation(19, 166, 38, 26, 5,   "Dog_Peeing",     dogPath);
        spriteSheetTool.addAnimation(4, 189, 38, 26, 10,   "Dog_BeginSleep", dogPath);
        spriteSheetTool.addAnimation(375, 189, 38, 26, 14, "Dog_Sleeping",   dogPath);
        spriteSheetTool.addAnimation(122, 215, 38, 24, 10, "Dog_WakeUp",     dogPath);
        spriteSheetTool.addAnimation(4, 239, 36, 29, 12,   "Dog_Walking",    dogPath);
        spriteSheetTool.addAnimation(4, 265, 39, 26, 5,    "Dog_Running",    dogPath);
        spriteSheetTool.addAnimation(4, 265, 39, 26, 5,    "Dog_Walking",    dogPath);
        spriteSheetTool.addAnimation(2, 316, 38, 55, 11,   "Dog_Flipping",   dogPath);

}

void MainWindow::playDogAnimation(QImage image){
   //QPixmap convertedImage = QPixmap::fromImage(image);
   //convertedImage = convertedImage.scaled(144, 104);
   //ui->imageLabel->setPixmap(convertedImage);
}

void MainWindow::update()
{
    if(dogFrameNumber > 10){
        dogFrameNumber = 0;
    }
    //emit getDogAnimationSignal("Dog_Flipping", dogFrameNumber);
    dogFrameNumber += 1;
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
