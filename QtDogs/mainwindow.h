#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "spritesheettool.h"
#include <QTimer>
#include "model.h"
#include <QtDebug>
#include<SFML/Audio.hpp>


enum
{
    SPLASH_SCREEN,
    PLAYING_GAME
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Model model;
    sf::Sound sound;
    sf::SoundBuffer buffer;

    sf::RenderTexture frame;

    sf::Texture ballTex;
    sf::Texture treatTex;
    sf::Texture dogTex;
    sf::Texture backgroundTex;

    sf::Sprite ball;
    sf::Sprite treat;
    sf::Sprite dog;
    sf::Sprite background;

    std::string dogAnimation;
    std::string backgroundAnimation;

    int width;
    int height;
    int animationDelayCounter;
    int gameState;
    int dogAnimationLength;
    int dogFrameNumber = 0;
    int backgroundAnimationLength;
    int backgroundFrameNumber = 0;

    Ui::MainWindow *ui;
    QTimer* timer;
    QTimer* poopTimer;
    QTimer* hungerTimer;
    QTimer* timeOfDayChange;
    SpriteSheetTool spriteSheetTool;

    void updateDogAnimation();
    void updateBackgroundAnimation();
    void loadAnimations();
    void startSplashScreen();
    void startGame();

private slots:
    void update();
    void on_trustProgressBar_valueChanged(int);
    void on_hungerProgressBar_valueChanged(int);
    void on_bathroomProgressBar_valueChanged(int);
    void updatePoopBar();
    void updateHungerBar();
    void on_trustLevel_valueChanged(int);
    void playMusic();
    void changeTimeOfDay();

signals:
    void dogWalkLeft();
    void dogWalkRight();
    void updateBathroomProgressBar();
    void updateHungerProgressBar();
};

#endif // MAINWINDOW_H
