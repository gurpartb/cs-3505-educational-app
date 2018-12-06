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
        connect(&spriteSheetTool, SIGNAL(imageSendSignal(sf::Texture&)), this, SLOT(playDogAnimation(sf::Texture&)));

        updateAnimation = 0;
        numFrames = 0;
        dogX = 100;
        dogY = 500;

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

        width = ui->imageLabel->size().width();
        height = ui->imageLabel->size().height();

        frame.create(unsigned(width), unsigned(height));

        backgroundTex.loadFromFile("../QtDogs/assets/pixelartparkfinal.png");
        background.setTexture(backgroundTex);
        background.setScale(2.4,2.4);

        ballTex.loadFromFile("../QtDogs/assets/Beach_Ball.png");
        ball.setTexture(ballTex);
        ball.setOrigin(64,64);

        dog.setTexture(dogTex);
        dog.setScale(4.0,4.0);
        dog.setOrigin(0,0);
        //model connection
        connect(ui->petButton, &QPushButton::pressed, &model, &Model::dogPetted);
        connect(ui->foodButton, &QPushButton::pressed, &model, &Model::dogFed);
        connect(ui->ballButton, &QPushButton::pressed, &model, &Model::dogPlayedWithBall);
        connect(ui->parkButton, &QPushButton::pressed, &model, &Model::dogWentToThePark);
        connect(ui->letOutButton, &QPushButton::pressed, &model, &Model::dogLetOut);
        connect(&model, SIGNAL(updateTrustLevel(int)), this, SLOT(on_trustProgressBar_valueChanged(int)));
        connect(&model, SIGNAL(updateHungerLevel(int)), this, SLOT(on_hungerProgressBar_valueChanged(int)));
        connect(&model, SIGNAL(updateBathroomLevel(int)), this, SLOT(on_bathroomProgressBar_valueChanged(int)));
        connect(this, SIGNAL(dogWalkLeft()),&model,SLOT(dogWalkLeft()));
        connect(this, SIGNAL(dogWalkRight()),&model,SLOT(dogWalkRight()));
}


void MainWindow::playDogAnimation(sf::Texture& texture)
{
    dog.setTextureRect(sf::IntRect(0,0,texture.getSize().x,texture.getSize().y));
    dogTex = texture;
}

void MainWindow::update()
{
   model.update();

   if(updateAnimation >= 4)
   {
       numFrames = spriteSheetTool.getAnimationFrameCount("Dog_Flipping");
       emit getDogAnimationSignal("Dog_Flipping", frameNumber);

       if(frameNumber >= numFrames)
       {
           frameNumber = 0;
       }

       frameNumber++;
       updateAnimation = 1;
   }
   else
   {
       ++updateAnimation;
   }

   //  qDebug() << "x:" << model.ballX();
   //  qDebug() << "y:" << model.ballY();


   dog.setPosition(model.Dogx()*width/2.0f,model.Dogy()*height/2.0f);

   ball.setPosition(model.ballX()*width/2.0f,model.ballY()*height/2.0f);
   ball.setRotation(model.ballR()*180.0f/3.14159f);
   if(model.ballX()*width/2.0f - model.Dogx()*width/2.0f > 0)
   {
       emit dogWalkRight();
         qDebug() << "Walk right";
   }
   else
   {
       emit dogWalkLeft();
       qDebug() << "Walk left";
   }
   frame.clear(sf::Color::White);
   frame.draw(background);
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
