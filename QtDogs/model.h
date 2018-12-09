/**
  CS 3505 - A8 Final Project - QT Dogs
  Educational application to teach youth the importance of pet responsibility.
  Designed by:
  Brendan Johnston, Andrew Dron, Caleb Edwards, Colton Lee, Gurpartap Bhatti, Jacob Haydel, Tyler Trombley, Jared Hansen
*/

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include "dog.h"

#define SCALE 2.0f/768.0f

///
/// \brief The Model class
/// Model for handling connections between the dog class and UI. Handles most Box-2D information.
///
class Model: public QObject
{
    Q_OBJECT

private:
    b2Vec2* gravity;
    b2World* world;

    Dog* dog;
    b2Body* ball;
    b2Body* treat;
    b2Body* food;
    b2Body* dogBody;
    b2Body* ground;
    b2Body* ceiling;
    b2Body* rightWall;
    b2Body* leftWall;

    int ballplayCount;

    int bathroomProgress;
    int hungerProgress;
    int trustProgress;
    int level;

    bool ballExists;
    bool treatExists;
    bool foodExists;
    bool isNight;
    bool atPark;


public:
    Model();
    virtual ~Model();

    void update();

    float ballX(){return ball->GetPosition().x;}
    float ballY(){return ball->GetPosition().y;}
    float ballR(){return ball->GetAngle();}
    bool getBallExists(){return ballExists;}

    float treatX(){return treat->GetPosition().x;}
    float treatY(){return treat->GetPosition().y;}
    float treatR(){return treat->GetAngle();}
    bool getTreatExists(){return treatExists;}

    float foodX(){return food->GetPosition().x;}
    float foodY(){return food->GetPosition().y;}
    float foodR(){return food->GetAngle();}
    bool getFoodExists(){return foodExists;}

    float dogX(){return dogBody->GetPosition().x;}
    float dogY(){return dogBody->GetPosition().y;}
    std::string getDogState(){return dog->getDogState();}

    float getDogHunger(){return dog->getHunger();}
    float getDogBathroom(){return dog->getBathroom();}
    int getDogTrustLevel(){return dog->getTrustLevel();}
    int getDogTrustProgress(){return dog->getTrustProgress();}
    bool getDogDirectionLeft(){return dog->getDogDirectionLeft();}

private:
    void createBall();
    void createTreat();
    void createFood();
    void createScene();
    void createDog();

    void dogCollisions();
    void ballCollisions();
    void treatCollisions();
    void checkCollisions();

public slots:
    void dogTrick();
    void dogFed();
    void dogPlayedWithBall();
    void dogWentToThePark();
    void dogLetOut();
    void dogWalkLeft();
    void dogWalkRight();
    void dogTreat();
signals:
    void updateLevels(int);
    void updateTrustLevel(int);
    void updateHungerLevel(float);
    void updateBathroomLevel(float);
    void updateTrustProgress(int);
    void ballOnScreen(bool);
    void foodOnScreen(bool);
    void treatOnScreen(bool);
    void currentBallPosX(float);
    void currentDogPosX(float);
    void currentFoodPosX(float);
    void currentTreatPosX(float);
    void playBounceSound();
    void playWhistleSound();
    void playEatSound();

};

#endif // MODEL_H
