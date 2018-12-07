#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <bits/stdc++.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
        ui->setupUi(this);

        ui->hungerProgressBar->setValue(0);
        ui->bathroomProgressBar->setValue(0);
        ui->trustProgressBar->setValue(0);
        ui->levelNumber->display(1);

        timer = new QTimer(this);
        connect(timer,SIGNAL(timeout()),this,SLOT(update()));
        timer->start(1000/30);

        poopTimer = new QTimer(this);
        connect(poopTimer, SIGNAL(timeout()), this, SLOT(updatePoopBar()));
        poopTimer->start(1000);

        hungerTimer = new QTimer(this);
        connect(hungerTimer, SIGNAL(timeout()), this, SLOT(updateHungerBar()));
        hungerTimer->start(2000);

        connect(this, SIGNAL(getDogAnimationSignal(std::string, int)), &spriteSheetTool, SLOT(getAnimationFrame(std::string, int)));
        connect(&spriteSheetTool, SIGNAL(imageSendSignal(sf::Texture&)), this, SLOT(playDogAnimation(sf::Texture&)));

        updateAnimation = 0;
        numFrames = 0;
        dogX = 100;
        dogY = 500;

        std::string dogPath = "../QtDogs/assets/DogSpriteSheetFinal.png";
         //adding animation frames
        spriteSheetTool.addAnimation(0,   0,    36, 26,  4, "Dog_Idle",       dogPath);
        spriteSheetTool.addAnimation(0,   28,   36, 26, 15, "Dog_Sitting",    dogPath);
        spriteSheetTool.addAnimation(0,   59,   36, 26, 13, "Dog_Barking",    dogPath);
        spriteSheetTool.addAnimation(0,   138,  36, 26, 23, "Dog_Peeing",     dogPath);
        spriteSheetTool.addAnimation(15,  166,  36, 26, 5,  "Dog_Peeing",     dogPath);
        spriteSheetTool.addAnimation(0,   189,  36, 26, 10, "Dog_BeginSleep", dogPath);
        spriteSheetTool.addAnimation(360, 189,  36, 26, 14, "Dog_Sleeping",   dogPath);
        spriteSheetTool.addAnimation(122, 215,  36, 24, 10, "Dog_WakeUp",     dogPath);
        spriteSheetTool.addAnimation(0, 240, 36, 26, 12,    "Dog_Walking",    dogPath);
        spriteSheetTool.addAnimation(0, 263, 36, 26, 5,     "Dog_Running",    dogPath);
        spriteSheetTool.addAnimation(0, 265, 36, 26, 5,     "Dog_Walking",    dogPath);
        spriteSheetTool.addAnimation(0, 316, 36, 55, 11,    "Dog_Flipping",   dogPath);

        width = ui->imageLabel->size().width();
        height = ui->imageLabel->size().height();

        frame.create(unsigned(width), unsigned(height));

        backgroundTex.loadFromFile("../QtDogs/assets/pixelartparkfinal.png");
        background.setTexture(backgroundTex);
        background.setScale(2.4f,2.4f);

        ballTex.loadFromFile("../QtDogs/assets/Beach_Ball.png");
        ball.setTexture(ballTex);
        ball.setOrigin(64,64);

        treatTex.loadFromFile("../QtDogs/assets/Dog_biscuit.png");
        treat.setTexture(treatTex);
        treat.setOrigin(64,64);
        treat.setScale(0.2f,0.2f);

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

        connect(this, &MainWindow::updateBathroomProgressBar, &model, &Model::updateBathroomProgress);
        connect(&model, &Model::updateBathroomLevel, this, &MainWindow::on_bathroomProgressBar_valueChanged);
        connect(ui->letOutButton, &QPushButton::pressed, &model, &Model::resetBathroomProgress);
        connect(this, &MainWindow::updateHungerProgressBar, &model, &Model::updateHungerProgress);
        connect(ui->foodButton, &QPushButton::pressed, &model, &Model::resetHungerProgress);
        connect(&model, SIGNAL(updateTrustProgress(int)), this, SLOT(on_trustProgressBar_valueChanged(int)));
        //connect(&model, SIGNAL(updateLevels(int)), this, SLOT(on_trustLevel_valueChanged(int)));

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
       numFrames = spriteSheetTool.getAnimationFrameCount("Dog_Walking");
       emit getDogAnimationSignal("Dog_Walking", frameNumber);

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

    dog.setPosition(model.Dogx()*width/2.0f, model.Dogy()*height/2.0f);
   if(model.ballExists)
   {

       ball.setPosition(model.ballX()*width/2.0f,model.ballY()*height/2.0f);
       ball.setRotation(model.ballR()*180.0f/3.14159f);
       if(model.ballX()*width/2.0f - model.Dogx()*width/2.0f > 0)
       {
           dog.setTextureRect(sf::IntRect(0, 0, dogTex.getSize().x, dogTex.getSize().y));
           emit dogWalkRight();
       }
       else
       {
           dog.setTextureRect(sf::IntRect(dogTex.getSize().x, 0, -dogTex.getSize().x, dogTex.getSize().y));
           emit dogWalkLeft();
       }
   }
   else if(model.treatExists)
   {

       treat.setPosition(model.treatX()*width/2.0f,model.treatY()*height/2.0f);
       treat.setRotation(model.treatR()*180.0f/3.14159f);
       if(model.treatX()*width/2.0f - model.Dogx()*width/2.0f > 0)
       {
           dog.setTextureRect(sf::IntRect(0, 0, dogTex.getSize().x, dogTex.getSize().y));
           emit dogWalkRight();
       }
       else
       {
           dog.setTextureRect(sf::IntRect(dogTex.getSize().x, 0, -dogTex.getSize().x, dogTex.getSize().y));
           emit dogWalkLeft();
       }
   }
   frame.clear(sf::Color::White);
   frame.draw(background);
   if(model.ballExists)
   {
       frame.draw(ball);
   }
   else if(model.treatExists)
   {
       frame.draw(treat);
   }
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

///
/// \brief MainWindow::on_trustProgressBar_valueChanged
/// \param value
///
void MainWindow::on_trustProgressBar_valueChanged(int value)
{
    ui->trustProgressBar->setValue(value);
}

///
/// \brief MainWindow::on_hungerProgressBar_valueChanged
/// \param value
///
void MainWindow::on_hungerProgressBar_valueChanged(int value)
{
    ui->hungerProgressBar->setValue(value);
}

///
/// \brief MainWindow::on_bathroomProgressBar_valueChanged
/// \param value
///
void MainWindow::on_bathroomProgressBar_valueChanged(int value)
{
    ui->bathroomProgressBar->setValue(value);
}

void MainWindow::on_trustLevel_valueChanged(int value)
{
    ui->levelNumber->display(value);
}

///
/// \brief MainWindow::updatePoopBar
///
void MainWindow::updatePoopBar()
{
    emit updateBathroomProgressBar();
}

///
/// \brief MainWindow::updateHungerBar
///
void MainWindow::updateHungerBar()
{
    emit updateHungerProgressBar();
}
