/**
  CS 3505 - A8 Final Project - QT Dogs
  Educational application to teach youth the importance of pet responsibility.
  Designed by:
  Brendan Johnston, Andrew Dron, Caleb Edwards, Colton Lee, Gurpartap Bhatti, Jacob Haydel, Tyler Trombley, Jared Hansen
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <bits/stdc++.h>
#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Model connections
    //connectios for buttons
    connect(ui->trickButton, &QPushButton::pressed, &model, &Model::dogTrick);
    connect(ui->foodButton, &QPushButton::pressed, &model, &Model::dogFed);
    connect(ui->treatButton, &QPushButton::pressed,&model, &Model::dogTreat);
    connect(ui->ballButton, &QPushButton::pressed, &model, &Model::dogPlayedWithBall);
    connect(ui->parkButton, &QPushButton::pressed, this, &MainWindow::goToPark);
    connect(ui->letOutButton, &QPushButton::pressed, &model, &Model::dogLetOut);
    connect(ui->playButton, &QPushButton::pressed, this, &MainWindow::startGame);
    connect(ui->homeButton, &QPushButton::pressed, this, &MainWindow::goHome);

    //connections for updating status bars
    connect(&model, &Model::updateTrustLevel, this, &MainWindow::on_trustProgressBar_valueChanged);
    connect(&model, &Model::updateHungerLevel, this, &MainWindow::on_hungerProgressBar_valueChanged);
    connect(&model, &Model::updateBathroomLevel, this, &MainWindow::on_bathroomProgressBar_valueChanged);
    connect(&model, &Model::updateBathroomLevel, this, &MainWindow::on_bathroomProgressBar_valueChanged);
    connect(&model, &Model::updateTrustProgress, this, &MainWindow::on_trustProgressBar_valueChanged);

    //connections for dog movement
    connect(this, &MainWindow::dogWalkLeft,&model, &Model::dogWalkLeft);
    connect(this, &MainWindow::dogWalkRight,&model, &Model::dogWalkRight);
    //connections for sound effects
    connect(&model, &Model::playBounceSound, this, &MainWindow::playBounceSound);
    connect(&model, &Model::playBarkSound, this, &MainWindow::playBarkSound);
    connect(&model, &Model::playWhistleSound, this, &MainWindow::playWhistleSound);
    connect(&model, &Model::playEatSound, this, &MainWindow::playEatSound);

    width = ui->imageLabel->size().width();
    height = ui->imageLabel->size().height();

    ui->hungerProgressBar->setValue(0);
    ui->bathroomProgressBar->setValue(0);
    ui->trustProgressBar->setValue(0);
    ui->levelNumber->display(0);
    ui->homeButton->setVisible(false);
    parkPos = 0;


    animationDelayCounter = 0;

    //setup sfml stuff
    loadAnimations();
    frame.create(unsigned(width), unsigned(height));

    //backgroundTex.loadFromFile("../QtDogs/assets/Splash_Screen.png");
    background.setTexture(backgroundTex);
    background.setScale(1.0f,1.0f);
    backgroundAnimation = "Splash_Screen";

    ballTex.loadFromFile("../QtDogs/assets/Beach_Ball.png");
    ball.setTexture(ballTex);
    ball.setOrigin(64,64);

    treatTex.loadFromFile("../QtDogs/assets/Dog_biscuit.png");
    treat.setTexture(treatTex);
    treat.setScale(0.25f,0.25f);
    treat.setOrigin(128,128);

    foodTex.loadFromFile("../QtDogs/assets/DogBowl.png");
    food.setTexture(foodTex);
    food.setScale(1.0f,1.0f);
    //food.setScale(.8f,.8f);
    //food.setOrigin(128,128);
    food.setOrigin(46,30);

    //dogTex.loadFromFile("../QtDogs/assets/DogSpriteSheetFinal.png");
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

    enableUi(false);
   // playMusic();//this will play the music when the game is started
    musicBuffer.loadFromFile("../QtDogs/assets/who_let_dogs_out.ogg");
    //all of these are awufl but they were the best i could find on soundbible
    whistleBuffer.loadFromFile("../QtDogs/assets/Whistle.wav");
    eatBuffer.loadFromFile("../QtDogs/assets/Eat.wav");
    bounceBuffer.loadFromFile("../QtDogs/assets/Bounce2.wav");
    barkBuffer.loadFromFile(("../QtDogs/assets/bark_1.ogg"));

    musicSound.setBuffer(musicBuffer);
    whistleSound.setBuffer(whistleBuffer);
    eatSound.setBuffer(eatBuffer);
    bounceSound.setBuffer(bounceBuffer);
    barkSound.setBuffer(barkBuffer);
}

///
/// \brief MainWindow::~MainWindow
/// Main Window destructor.
///
MainWindow::~MainWindow()
{
    delete ui;
}

///
/// \brief MainWindow::loadAnimations
/// Loads all of the sprite sheets to provide animations for background, splash screen, and dog.
///
void MainWindow::loadAnimations()
{
    //adding animation frames
    dogPath.loadFromFile("../QtDogs/assets/DogSpriteSheetFinal.png");
    spriteSheetTool.addAnimation(0,   7,    36, 26,  3, "Dog_Idle",       dogPath);
    spriteSheetTool.addAnimation(410,   339, 72, 40,  7,"Dog_Dying",   dogPath);
    spriteSheetTool.addAnimation(770, 304, 72, 34,  1,  "Dog_Dead",   dogPath);
    spriteSheetTool.addAnimation(0,   35,   36, 28, 14, "Dog_Sitting",    dogPath);
    spriteSheetTool.addAnimation(0,   68,   36, 26, 12, "Dog_Barking",    dogPath);
    spriteSheetTool.addAnimation(0,   147,  36, 26, 22, "Dog_Peeing",     dogPath);
    spriteSheetTool.addAnimation(15,  175,  36, 26, 4,  "Dog_Peeing",     dogPath);
    spriteSheetTool.addAnimation(0,   195,  36, 26, 9,  "Dog_Sleeping", dogPath);
    spriteSheetTool.addAnimation(360, 195,  36, 26, 13, "Dog_Sleeping",   dogPath);
    spriteSheetTool.addAnimation(0, 221,  36, 26, 3,    "Dog_Sleeping",   dogPath);
    spriteSheetTool.addAnimation(108, 223,  36, 24, 9,  "Dog_WakeUp",     dogPath);
    spriteSheetTool.addAnimation(0, 247, 36, 24, 11,    "Dog_Walking",    dogPath);
    spriteSheetTool.addAnimation(0, 272, 36, 25, 4,     "Dog_Running",    dogPath);
    spriteSheetTool.addAnimation(0, 325, 36, 55, 10,    "Dog_Flipping",   dogPath);

    parkPath.loadFromFile("../QtDogs/assets/pixelartparkfinal.png");
    spriteSheetTool.addAnimation(0, 0, 7680, 768, 1, "Park_Screen", parkPath);

    splashScreenPath.loadFromFile("../QtDogs/assets/Splash_Screen.png");
    for(int i = 0; i < 10; ++i)
        spriteSheetTool.addAnimation(0, i*768, 768, 768,  8, "Splash_Screen", splashScreenPath);
   /// spriteSheetTool.addAnimation(0,   5376,  768, 768,  7, "Splash_Screen",    splashScreenPath);

    daytimePath.loadFromFile("../QtDogs/assets/Daytime.png");
    for(int i = 0; i < 5; ++i)
        spriteSheetTool.addAnimation(0,   768*i,   768, 768,  5, "Daytime",    daytimePath);
    spriteSheetTool.addAnimation(0,   3840,  768, 768,  1, "Daytime",    daytimePath);

    eveningPath.loadFromFile("../QtDogs/assets/Evening.png");
    for(int i = 0; i < 5; ++i)
        spriteSheetTool.addAnimation(0,   768*i,   768, 768,  5, "Evening",    eveningPath);
    spriteSheetTool.addAnimation(0,   3840,  768, 768,  1, "Evening",    eveningPath);

    nightPath.loadFromFile("../QtDogs/assets/Night.png");
    for(int i = 0; i < 4; ++i)
        spriteSheetTool.addAnimation(0,   768*i,   768, 768,  3, "Night",    nightPath);
}

///
/// \brief MainWindow::updateDogAnimation
/// Updates the current displaying animation with the new dog animation at 30FPS.
///
void MainWindow::updateDogAnimation()
{
    if(dogAnimation != "Dog_Dead" && dogAnimation != "Dog_Sleeping" && dogAnimation != "Dog_Sitting" && dogAnimation != "Dog_Flipping" )
    {
        ++dogFrameNumber;
    }
    else if(dogFrameNumber < dogAnimationLength)
    {
        ++dogFrameNumber;
        if(dogFrameNumber >= dogAnimationLength)
        {
            if(dogAnimation == "Dog_Sitting")
            {
                dogFrameNumber = 4;
            }
            else if(dogAnimation == "Dog_Sleeping")
            {
                dogFrameNumber = 9;
            }
            else
            {
                dogFrameNumber--;
            }
        }
    }

    if(dogFrameNumber >= dogAnimationLength)
        dogFrameNumber = 0;

    sf::IntRect* rect = spriteSheetTool.getAnimationFrame(dogAnimation, dogFrameNumber);
    sf::Texture* texture = spriteSheetTool.getTexture(dogAnimation);

    dogAnimationLength = spriteSheetTool.getAnimationFrameCount(dogAnimation);
    dog.setOrigin(static_cast<float>(rect->width / 2.0), static_cast<float>(rect->height - 12));

    dog.setTextureRect(*rect);
    dog.setTexture(*texture);
}

///
/// \brief MainWindow::updateBackgroundAnimation
/// Updates the background animation for day / evening / night cycles when at the house.
///
void MainWindow::updateBackgroundAnimation()
{
    if(backgroundAnimation != "Park_Screen")
    {
        ++backgroundFrameNumber;
        background.setOrigin(0,0);
    }
    else
    {
        parkPos = (parkPos + 24) % 3840;
        background.setOrigin(parkPos,0);
    }

    if(backgroundFrameNumber >= backgroundAnimationLength)
        backgroundFrameNumber = 0;

    sf::IntRect* rect = spriteSheetTool.getAnimationFrame(backgroundAnimation, backgroundFrameNumber);
    sf::Texture* texture = spriteSheetTool.getTexture(backgroundAnimation);

    backgroundAnimationLength = spriteSheetTool.getAnimationFrameCount(backgroundAnimation);

    background.setTextureRect(*rect);
    background.setTexture(*texture);
}

///
/// \brief MainWindow::enableUi
/// Allows for the enabling or disabling of the UI. Sets visibilities and enabling buttons / labels.
/// \param enabled - True if game is active, false if otherwise.
///
void MainWindow::enableUi(bool enabled)
{
    gameStarted = enabled;
    ui->trickButton->setEnabled(enabled);
    ui->ballButton->setEnabled(enabled);
    ui->foodButton->setEnabled(enabled);
    ui->treatButton->setEnabled(enabled);
    ui->ballButton->setEnabled(enabled);
    ui->parkButton->setEnabled(enabled);
    ui->letOutButton->setEnabled(enabled);
    ui->playButton->setEnabled(!enabled); //Reverse so Play button is disabled.

    ui->trustProgressBar->setEnabled(enabled);
    ui->hungerProgressBar->setEnabled(enabled);
    ui->bathroomProgressBar->setEnabled(enabled);

    ui->levelLabel->setVisible(enabled);
    ui->levelNumber->setVisible(enabled);

    ui->trickButton->setVisible(enabled);
    ui->ballButton->setVisible(enabled);
    ui->foodButton->setVisible(enabled);
    ui->treatButton->setVisible(enabled);
    ui->ballButton->setVisible(enabled);
    ui->parkButton->setVisible(enabled);
    ui->letOutButton->setVisible(enabled);
    ui->playButton->setVisible(!enabled); //Reverse so Play button is disabled.
}

///
/// \brief MainWindow::startGame
/// Starts the game after a user presses play from the splash screen.
///
void MainWindow::startGame()
{
    enableUi(true);
    //sound.stop();
}


void MainWindow::updateTimeOfDay()
{
    timeOfDay++;
    if(timeOfDay < 30*60)
    {
        backgroundAnimation = "Daytime";
        model.isNight = false;
    }
    else if(timeOfDay < 30*120)
    {
         backgroundAnimation = "Evening";
         model.isNight = false;
    }
    else if(timeOfDay < 30*180)
    {
        backgroundAnimation = "Night";
        model.isNight = true;
    }
    else
    {
        timeOfDay = 0;
    }
}

///
/// \brief MainWindow::goToPark
/// Change background and emit signals to display the park
///
void MainWindow::goToPark()
{
    backgroundAnimation = "Park_Screen";
    model.dogWentToThePark(true);
}

///
/// \brief MainWindow::goHome
/// sets the scene for the dog to go home.
///
void MainWindow::goHome()
{
    model.dogWentToThePark(false);
}

///
/// \brief MainWindow::update
/// Main update function to display SFML objects and call an update to the model if the game is started.
///
void MainWindow::update()
{
    if (gameStarted)
    {

        model.update();

        if(model.isDogInPark())
        {
           backgroundAnimation = "Park_Screen";
           ui->parkButton->setVisible(false);
           ui->homeButton->setVisible(true);
        }
        else
        {
            updateTimeOfDay();
            ui->parkButton->setVisible(true);
            ui->homeButton->setVisible(false);
        }

        if (model.getDogTrustLevel() >= 3)
        {
            if (!model.isNight){
                ui->parkButton->setEnabled(true);
                ui->parkButton->setText("Go To Park");
            }
            else
            {
                ui->parkButton->setEnabled(true);
                ui->parkButton->setText("Park Is Closed");
            }

        }
        else
        {
            ui->parkButton->setEnabled(false);
            ui->parkButton->setText("Unlock Lv 3");

        }

    }

    std::string tmp =  model.getDogState();
    if(tmp == "Playing" || tmp == "Eating")tmp = "Running";
    if(tmp == "BeginDeath")tmp = "Death";
    if(tmp == "BeginSleeping" || tmp == "EndSleeping")tmp = "Sleeping";
    dogAnimation = "Dog_" + tmp;

    if(model.getDogDirectionLeft())
        dog.setScale(-4.0,4.0);
    else
        dog.setScale(4.0,4.0);

    //update animations
    if(animationDelayCounter <= 1)
    {
        if(prevDogAnimation!= dogAnimation)
            dogFrameNumber = 0;
        prevDogAnimation = dogAnimation;
        updateDogAnimation();
        updateBackgroundAnimation();
        animationDelayCounter = 6;
    }
    else --animationDelayCounter;


    frame.clear(sf::Color::Transparent);
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
    else if(model.getFoodExists())
    {
        food.setPosition(model.foodX()*width/2.0f,model.foodY()*height/2.0f);
        food.setRotation(model.foodR()*180.0f/3.1459f);
        frame.draw(food);
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

///
/// \brief MainWindow::on_trustProgressBar_valueChanged
/// Sets the value of the trust progression bar.
///
void MainWindow::on_trustProgressBar_valueChanged(int value)
{
    ui->trustProgressBar->setValue(value);
}

///
/// \brief MainWindow::on_hungerProgressBar_valueChanged
/// Sets the value of the hunger progression bar.
///
void MainWindow::on_hungerProgressBar_valueChanged(int value)
{
    ui->hungerProgressBar->setValue(value);
}

///
/// \brief MainWindow::on_bathroomProgressBar_valueChanged
///  Sets the value of the bathroom progression bar.
///
void MainWindow::on_bathroomProgressBar_valueChanged(int value)
{
    ui->bathroomProgressBar->setValue(value);
}

///
/// \brief MainWindow::on_trustLevel_valueChanged
///  Sets the value of the trust level.
///
void MainWindow::on_trustLevel_valueChanged(int value)
{
    ui->levelNumber->display(value);
}

///
/// \brief MainWindow::updatePoopBar
/// Updates the value of the bathroom bar.
///
void MainWindow::updatePoopBar()
{

}

///
/// \brief MainWindow::updateHungerBar
/// Updates the value of the hunger bar.
///
void MainWindow::updateHungerBar()
{

}

///
/// \brief MainWindow::playMusic
///  Plays the intro music on the splash screen.
///
void MainWindow::playMusic()
{
    musicSound.play();
}

///
/// \brief MainWindow::changeTimeOfDay
/// Changes the background depending on the time of day.
///
void MainWindow::changeTimeOfDay()
{

}

///
/// \brief MainWindow::on_instructionsButton_clicked
/// Displays a QMessageBox of the instructions and authors of the game. And what the game is about.
///
void MainWindow::on_instructionsButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Instructions"));
    msgBox.setInformativeText(tr("About:\nQtDogs in an educational app, intended for kids aged 7 to 14. Our goal is to teach young people the importance of taking care of their own pet. Many children want pets of their own but need to understand responsibility before getting a pet of their own.\n\nPlaying the Game:\nTo start the game press the play button. You will then have a dog of your own!\nProgressing in the Game:\n- Hunger: As time passes, your dog will become hungry, you can use the feed button to fully feed your dog. You always want to have close to 100% hunger so you can keep your dog happy! The longer your dog is happy, the faster your trust level progresses.\n\n- Trust Level: The more things you to do keep your dog happy, the more you progress your Trust Level. Once you reach higher levels you will be able to take your dog out, as you can trust your dog to be good in public places.\n\n- Trick & Ball & Treats: Playing with your dog can help increase your trust level quicker! Giving him treats also help, and he may do cool tricks!\n\n- Bathroom Level & Let Out: Your dog over time will need to use the restroom. Make sure to let him out before he ruins your carpet! The longer you wait to take him out the less your dog can trust you, and he may do something you don’t like. Make sure to take him out often!\n\nTake care of your pet and have fun in QT Dogs!\n\nDeveloped by:\nAndrew Dron\nBrendan Johnston\nCaleb Edwards\nColton Lee\nGurpartap Bhatti\nJared Hansen\nJacob Haydel\nTyler Trombley\n"));
    msgBox.setStandardButtons(QMessageBox::Cancel);
    msgBox.exec();
}

void MainWindow::playBounceSound()
{
    bounceSound.play();
}

void MainWindow::playWhistleSound()
{
  whistleSound.play();
}

void MainWindow::playEatSound()
{
    eatSound.play();
}

void MainWindow::playBarkSound()
{
    if(barkCounter == 30) {
        barkCounter = 0;
        unsigned int numOfSound = static_cast<unsigned int>((static_cast<float>(rand()) * 5.0f) / static_cast<float>(RAND_MAX)) + 1;
        barkBuffer.loadFromFile("../QtDogs/assets/bark_" + std::to_string(numOfSound) + ".ogg");
        barkSound.setBuffer(barkBuffer);
        barkSound.play();
    }
    else {
        barkCounter++;
    }

}

