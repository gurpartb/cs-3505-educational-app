#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <bits/stdc++.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
        ui->setupUi(this);

        //Model connections
        connect(ui->petButton, &QPushButton::pressed, &model, &Model::dogPetted);
        connect(ui->foodButton, &QPushButton::pressed, &model, &Model::dogFed);
        connect(ui->ballButton, &QPushButton::pressed, &model, &Model::dogPlayedWithBall);
        connect(ui->parkButton, &QPushButton::pressed, &model, &Model::dogWentToThePark);
        connect(ui->letOutButton, &QPushButton::pressed, &model, &Model::dogLetOut);
        connect(ui->ballButton,&QPushButton::pressed,this,&MainWindow::playMusic);
        connect(&model, &Model::updateTrustLevel, this, &MainWindow::on_trustProgressBar_valueChanged);
        connect(&model, &Model::updateHungerLevel, this, &MainWindow::on_hungerProgressBar_valueChanged);
        connect(&model, &Model::updateBathroomLevel, this, &MainWindow::on_bathroomProgressBar_valueChanged);

        connect(this, &MainWindow::dogWalkLeft,&model, &Model::dogWalkLeft);
        connect(this, &MainWindow::dogWalkRight,&model, &Model::dogWalkRight);

        connect(&model, &Model::updateBathroomLevel, this, &MainWindow::on_bathroomProgressBar_valueChanged);
        connect(&model, &Model::updateTrustProgress, this, &MainWindow::on_trustProgressBar_valueChanged);


        width = ui->imageLabel->size().width();
        height = ui->imageLabel->size().height();


        ui->hungerProgressBar->setValue(0);
        ui->bathroomProgressBar->setValue(0);
        ui->trustProgressBar->setValue(0);
        ui->levelNumber->display(1);

        animationDelayCounter = 0;

        //setup sfml stuff
        loadAnimations();
        frame.create(unsigned(width), unsigned(height));

        backgroundTex.loadFromFile("../QtDogs/assets/Splash_Screen.png");
        background.setTexture(backgroundTex);
        background.setScale(1.0f,1.0f);
        backgroundAnimation = "Splash_Screen";

        ballTex.loadFromFile("../QtDogs/assets/Beach_Ball.png");
        ball.setTexture(ballTex);
        ball.setOrigin(64,64);

        treatTex.loadFromFile("../QtDogs/assets/Dog_biscuit.png");
        treat.setTexture(treatTex);
        treat.setOrigin(64,64);
        treat.setScale(0.2f,0.2f);

        dog.setTexture(dogTex);
        dog.setScale(4.0,4.0);
        dog.setOrigin(18,0);
        dogAnimation = "Dog_Idle";


        //create timers
        timeOfDayChange = new QTimer(this);
        connect(timeOfDayChange, SIGNAL(timeout()), this, SLOT(changeTimeOfDay()));
        timeOfDayChange->start(180000);

        timer = new QTimer(this);
        connect(timer,SIGNAL(timeout()),this,SLOT(update()));
        timer->start(1000/30);


        //start
        startSplashScreen();
}

void MainWindow::loadAnimations()
{
    //adding animation frames
    sf::Image dogPath;
    dogPath.loadFromFile("../QtDogs/assets/DogSpriteSheetFinal.png");
    spriteSheetTool.addAnimation(0,   0,    36, 26,  3, "Dog_Idle",       dogPath);
    spriteSheetTool.addAnimation(0,   28,   36, 26, 14, "Dog_Sitting",    dogPath);
    spriteSheetTool.addAnimation(0,   59,   36, 26, 12, "Dog_Barking",    dogPath);
    spriteSheetTool.addAnimation(0,   138,  36, 26, 22, "Dog_Peeing",     dogPath);
    spriteSheetTool.addAnimation(15,  166,  36, 26, 4,  "Dog_Peeing",     dogPath);
    spriteSheetTool.addAnimation(0,   189,  36, 26, 9, "Dog_BeginSleep", dogPath);
    spriteSheetTool.addAnimation(360, 189,  36, 26, 13, "Dog_Sleeping",   dogPath);
    spriteSheetTool.addAnimation(122, 215,  36, 24, 9, "Dog_WakeUp",     dogPath);
    spriteSheetTool.addAnimation(0, 240, 36, 26, 11,    "Dog_Walking",    dogPath);
    spriteSheetTool.addAnimation(0, 263, 36, 26, 4,     "Dog_Running",    dogPath);
    spriteSheetTool.addAnimation(0, 265, 36, 26, 4,     "Dog_Walking",    dogPath);
    spriteSheetTool.addAnimation(0, 316, 36, 55, 10,    "Dog_Flipping",   dogPath);


    sf::Image splashScreenPath;
    splashScreenPath.loadFromFile("../QtDogs/assets/Splash_Screen.png");
    for(int i = 0; i < 7; ++i)
           spriteSheetTool.addAnimation(0, i*768, 768, 768,  8, "Splash_Screen", splashScreenPath);
    spriteSheetTool.addAnimation(0,   5376,  768, 768,  7, "Splash_Screen",    splashScreenPath);


    sf::Image daytimePath;
    daytimePath.loadFromFile("../QtDogs/assets/Daytime.png");
    for(int i = 0; i < 5; ++i)
         spriteSheetTool.addAnimation(0,   768*i,   768, 768,  5, "Daytime",    daytimePath);
    spriteSheetTool.addAnimation(0,   3840,  768, 768,  1, "Daytime",    daytimePath);


    sf::Image eveningPath;
    eveningPath.loadFromFile("../QtDogs/assets/Evening.png");
    for(int i = 0; i < 5; ++i)
        spriteSheetTool.addAnimation(0,   768*i,   768, 768,  5, "Evening",    eveningPath);
    spriteSheetTool.addAnimation(0,   3840,  768, 768,  1, "Evening",    eveningPath);


    sf::Image nightPath;
    nightPath.loadFromFile("../QtDogs/assets/Night.png");
    for(int i = 0; i < 4; ++i)
        spriteSheetTool.addAnimation(0,   768*i,   768, 768,  3, "Night",    nightPath);
}


void MainWindow::updateDogAnimation()
{
    dogAnimationLength = spriteSheetTool.getAnimationFrameCount(dogAnimation);
    sf::Texture texture = spriteSheetTool.getAnimationFrame(dogAnimation, dogFrameNumber);

    dog.setOrigin(texture.getSize().x/2,texture.getSize().y/2);
    dog.setTextureRect(sf::IntRect(0,0,texture.getSize().x,texture.getSize().y));
    dogTex = texture;

    if(dogFrameNumber >= dogAnimationLength)
        dogFrameNumber = 0;

    ++dogFrameNumber;
}

void MainWindow::updateBackgroundAnimation()
{
    backgroundAnimationLength = spriteSheetTool.getAnimationFrameCount(backgroundAnimation);
    sf::Texture texture = spriteSheetTool.getAnimationFrame(backgroundAnimation, backgroundFrameNumber);

    background.setTextureRect(sf::IntRect(0,0,texture.getSize().x,texture.getSize().y));
    backgroundTex = texture;

    if(backgroundFrameNumber >= backgroundAnimationLength)
        backgroundFrameNumber = 0;

    ++backgroundFrameNumber;
}

void MainWindow::startSplashScreen()
{

}

void MainWindow::startGame()
{

}

void MainWindow::update()
{
    model.update();



    //TODO: move this somewhere else
    if(model.getBallExists())
    {

        if (model.ballX()*width/2.0f - model.dogX()*width/2.0f > 0)
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
    else if(model.getTreatExists())
    {

        if(model.treatX()*width/2.0f - model.dogX()*width/2.0f > 0)
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


    if(animationDelayCounter <= 1)
    {
        updateDogAnimation();
        updateBackgroundAnimation();
        animationDelayCounter = 4;
    }
    else --animationDelayCounter;


    frame.clear(sf::Color::White);
    frame.draw(background);

    if(model.getBallExists())
    {
        ball.setPosition(model.ballX()*width/2.0f,model.ballY()*height/2.0f);
        ball.setRotation(model.ballR()*180.0f/3.14159f);
        frame.draw(ball);
    }
    else if(model.getTreatExists())
    {
        treat.setPosition(model.treatX()*width/2.0f,model.treatY()*height/2.0f);
        treat.setRotation(model.treatR()*180.0f/3.14159f);
        frame.draw(treat);
    }

    dog.setPosition(model.dogX()*width/2.0f, model.dogY()*height/2.0f);
    frame.draw(dog);

    frame.display();


    //draw the sfml canvas
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

void MainWindow::playMusic()
{

    if(!buffer.loadFromFile("../QtDogs/assets/who_let_dogs_out.ogg"))
    {
        std::cout << "ERRRRROR";
    }

    sound.setBuffer(buffer);
    sound.setVolume(100.f);
    sound.play();

    //qDebug() << sound.getStatus();
}

void MainWindow::changeTimeOfDay()
{

}
