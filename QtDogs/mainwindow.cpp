#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <bits/stdc++.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
        ui->setupUi(this);

        timer = new QTimer(this);
        connect(timer,SIGNAL(timeout()),this,SLOT(update()));
        timer->start(1000/30);

        connect(this, SIGNAL(getDogAnimationSignal(std::string, int)), &spriteSheetTool, SLOT(getAnimationFrame(std::string, int)));
        connect(&spriteSheetTool, SIGNAL(imageSendSignal(sf::Texture)), this, SLOT(playDogAnimation(sf::Texture)));
        spriteSheetTool.addAnimation(0,0, 36, 28, 4, "Idle", "../QtDogs/assets/Dog.png");
        int frameNumber = 1;
        emit getDogAnimationSignal("Idle", frameNumber);


        width = ui->imageLabel->size().width();
        height = ui->imageLabel->size().height();

        frame.create(unsigned(width), unsigned(height));

        ballTex.loadFromFile("../QtDogs/assets/Beach_Ball.png");
        ball.setTexture(ballTex);
        ball.setOrigin(64,64);


        //model connection
        connect(ui->petButton, &QPushButton::pressed, &model, &Model::dogPetted);
        connect(ui->foodButton, &QPushButton::pressed, &model, &Model::dogFed);
        connect(ui->ballButton, &QPushButton::pressed, &model, &Model::dogPlayedWithBall);
        connect(ui->parkButton, &QPushButton::pressed, &model, &Model::dogWentToThePark);
        connect(ui->letOutButton, &QPushButton::pressed, &model, &Model::dogLetOut);
        connect(&model, SIGNAL(updateTrustLevel(int)), this, SLOT(on_trustProgressBar_valueChanged(int)));
        connect(&model, SIGNAL(updateHungerLevel(int)), this, SLOT(on_hungerProgressBar_valueChanged(int)));
        connect(&model, SIGNAL(updateBathroomLevel(int)), this, SLOT(on_bathroomProgressBar_valueChanged(int)));
}


void MainWindow::playDogAnimation(sf::Texture texture)
{
    dog.setTexture(texture);
   //QPixmap convertedImage = QPixmap::fromImage(image);
   //convertedImage = convertedImage.scaled(144, 104);
   //ui->imageLabel->setPixmap(convertedImage);
}

void MainWindow::update()
{
   model.update();

   ball.setPosition(model.ballX()*width/2.0f,model.ballY()*height/2.0f);
   ball.setRotation(model.ballR()*180.0f/3.14159f);

   frame.clear(sf::Color::White);
   frame.draw(ball);
   frame.draw(dog);
   frame.display();


   sf::Texture rt = frame.getTexture();
   sf::Image irt = rt.copyToImage();
   const uint8_t *pp = irt.getPixelsPtr();

   QImage qi(pp,width,height,QImage::Format_ARGB32);
   qi = qi.rgbSwapped();

   ui->imageLabel->setPixmap(QPixmap::fromImage(qi));
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
