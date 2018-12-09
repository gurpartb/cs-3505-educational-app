/**
  CS 3505 - A8 Final Project - QT Dogs
  Educational application to teach youth the importance of pet responsibility.
  Designed by:
  Brendan Johnston, Andrew Dron, Caleb Edwards, Colton Lee, Gurpartap Bhatti, Jacob Haydel, Tyler Trombley, Jared Hansen
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "spritesheettool.h"
#include <QTimer>
#include "model.h"
#include <QtDebug>
#include <QMessageBox>
#include<SFML/Audio.hpp>


enum
{
    SPLASH_SCREEN,
    PLAYING_GAME
};

namespace Ui {
class MainWindow;
}

///
/// \brief The MainWindow class
/// Proivdes UI display of the QT Dogs application along with handling button presses and any UI.
///
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Model model;
    sf::Sound musicSound;
    sf::Sound eatSound;
    sf::Sound whistleSound;
    sf::Sound bounceSound;
    sf::Sound barkSound;
    sf::Sound ballSound;

    sf::SoundBuffer musicBuffer;
    sf::SoundBuffer eatBuffer;
    sf::SoundBuffer whistleBuffer;
    sf::SoundBuffer bounceBuffer;
    sf::SoundBuffer barkBuffer;
    sf::SoundBuffer ballBuffer;

    sf::RenderTexture frame;

    sf::Texture ballTex;
    sf::Texture treatTex;
    sf::Texture dogTex;
    sf::Texture backgroundTex;

    sf::Sprite ball;
    sf::Sprite treat;
    sf::Sprite dog;
    sf::Sprite background;
    sf::Sprite food;

    std::string dogAnimation;
    std::string prevDogAnimation;
    std::string backgroundAnimation;

    sf::Texture dogPath;
    sf::Texture splashScreenPath;
    sf::Texture parkPath;
    sf::Texture daytimePath;
    sf::Texture eveningPath;
    sf::Texture nightPath;
    sf::Texture foodTex;

    int width;
    int height;
    int timeOfDay;
    int animationDelayCounter;
    int gameState;
    int dogAnimationLength;
    int dogFrameNumber = 0;
    int backgroundAnimationLength;
    int backgroundFrameNumber = 0;
    int parkPos;
    int barkCounter = 0;
    int ballSoundCounter = 0;
    bool gameStarted;

    Ui::MainWindow *ui;
    QTimer* timer;
    QTimer* poopTimer;
    QTimer* hungerTimer;
    QTimer* timeOfDayChange;
    SpriteSheetTool spriteSheetTool;

    void updateDogAnimation();
    void updateBackgroundAnimation();
    void loadAnimations();
    void enableUi(bool);
    void updateTimeOfDay();

private slots:
    void update();
    void on_trustProgressBar_valueChanged(int);
    void on_hungerProgressBar_valueChanged(int);
    void on_bathroomProgressBar_valueChanged(int);
    void on_trustLevel_valueChanged(int);
    void playMusic();
    void changeTimeOfDay();
    void startGame();
    void goToPark();
    void goHome();

    void on_instructionsButton_clicked();
    void playBounceSound();
    void playWhistleSound();
    void playEatSound();
    void playBarkSound();
    void playBallSound();


signals:
    void dogWalkLeft();
    void dogWalkRight();
    void updateBathroomProgressBar();
    void updateHungerProgressBar();
};

#endif // MAINWINDOW_H
