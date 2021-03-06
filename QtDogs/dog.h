/**
  CS 3505 - A8 Final Project - QT Dogs
  Educational application to teach youth the importance of pet responsibility.
  Designed by:
  Brendan Johnston, Andrew Dron, Caleb Edwards, Colton Lee, Gurpartap Bhatti, Jacob Haydel, Tyler Trombley, Jared Hansen
*/

#ifndef DOG_H
#define DOG_H

#include "math.h"
#include <ctime>
#include <string>
#include <Box2D/Box2D.h>
#include <vector>
#include <QObject>
#include <math.h>
#include <QDebug>

///
/// \brief The Dog class
/// Provides all implementation of the dog, holds dog's AI and positioning.
///
class Dog: public QObject
{

    Q_OBJECT

private:
    bool isDogInPark;
    bool ballExists;
    bool foodExists;
    bool treatExists;
    bool trickExists;
    float currentBallPositionX;
    float currentDogPositionX;
    float currentTreatPositionX;
    float currentFoodPositionX;
    float hunger;
    float bathroom;
    int trustLevel;
    int trustProgress;
    int currentAnimationFrame;
    bool dogDirectionLeft;
    float walkSpeed = 0.008f;
    float runSpeed = 0.017f;
    b2Vec2 currentForce;
    std::vector<std::string> currentStateFlag = {"Idle","Walk", "Sit", "Flip"};
    std::string currentState;

public:
    Dog();
    virtual ~Dog();

    b2Vec2 UpdateDogState(bool);
    int updateDogAnimation();
    std::string getDogState();

    float getHunger();
    float getBathroom();
    int getTrustLevel();
    int getTrustProgress();
    bool getDogDirectionLeft();
    bool isInPark(){return isDogInPark;}

    void feedFood();
    void feedTreat();
    void resetBathroom();
    void DogInPark(bool);
    void doesBallExist(bool);
    void doesFoodExist(bool);
    void doesTrickExist(bool);
    void doesTreatExist(bool);
    void BallPositionX(float);
    void DogPositionX(float);
    void FoodPositionX(float);
    void TreatPositionX(float);

private:
    bool getRandomDogDirectionLeft();
    bool increaseHunger();
    void resetHunger();
    bool increaseBathroom();

    void increaseTrustLevel();
    void increaseTrustProgress();
    void decreaseTrustProgress();
    void decreaseTrustLevel();
    void resetTrustLevel();

};

#endif // DOG_H
