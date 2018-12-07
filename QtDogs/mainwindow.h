#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "spritesheettool.h"
#include <QTimer>
#include "model.h"
#include <QtDebug>
#include<SFML/Audio.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void update();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int dogX, dogY;

    Model model;
    sf::Sound sound;
    sf::SoundBuffer buffer;

    sf::RenderTexture frame;

    sf::Texture ballTex;
    sf::Texture treatTex;
    sf::Texture dogTex;
    sf::Texture backgroundTex;

    int updateAnimation;
    int numFrames;

    sf::Sprite ball;
    sf::Sprite treat;
    sf::Sprite dog;
    sf::Sprite background;

    int width;
    int height;

    bool atHouse;

private slots:
    void on_trustProgressBar_valueChanged(int value);
    void on_hungerProgressBar_valueChanged(int value);
    void on_bathroomProgressBar_valueChanged(int value);
    void updatePoopBar();
    void updateHungerBar();
    void on_trustLevel_valueChanged(int value);
    void playDogAnimation(sf::Texture&);
    void playMusic();
    void changeTimeOfDay();

private:
    QTimer* timer;
    QTimer* poopTimer;
    QTimer* hungerTimer;
    QTimer* timeOfDayChange;
    Ui::MainWindow *ui;
    SpriteSheetTool spriteSheetTool;

    int frameNumber = 0;
signals:
    void getDogAnimationSignal(std::string, int);
    void dogWalkLeft();
    void dogWalkRight();
    void updateBathroomProgressBar();
    void updateHungerProgressBar();


};

#endif // MAINWINDOW_H
