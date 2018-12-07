#ifndef DOG_H
#define DOG_H

#include "math.h"
#include <ctime>
#include <string>
#include <Box2D/Box2D.h>
#include <vector>
#include <QObject>

class Dog: public QObject
{
private:
    bool ballExists;
    bool foodExists;
    bool treatExists;
    float currentBallPositionX;
    float currentDogPositionX;
    float currentTreatPositionX;
    float currentFoodPositionX;
    int currentNumOfFrames;
    float hunger;
    float bathroom;
    int trustLevel;
    int trustProgress;
    bool dogDirectionLeft;
    float walkSpeed = 5.0f;
    float runSpeed = 7.0f;
    b2Vec2 currentForce;
    std::vector<std::string> currentStateFlag = {"Sit", "Flip", "Walk"};
    std::string currentState;

public:
    Dog();
    b2Vec2 UpdateDogState(bool);
    std::string getDogState();
    void feedTreat();
    float getHunger();
    float getBathroom();
    int getTrustLevel();
    int getTrustProgress();
    void feedFood();

private:
    bool getDogDirectionLeft();

    bool increaseHunger();
    void resetHunger();


    bool increaseBathroom();
    void resetBathroom();


    void increaseTrustLevel();
    void increaseTrustProgress();
    void decreaseTrustProgress();
    void decreaseTrustLevel();
    void resetTrustLevel();

public slots:
    void doesBallExist(bool);
    void doesFoodExist(bool);
    void doesTreatExist(bool);
    void BallPositionX(float);
    void DogPositionX(float);
    void FoodPositionX(float);
    void TreatPositionX(float);
};

#endif // DOG_H
