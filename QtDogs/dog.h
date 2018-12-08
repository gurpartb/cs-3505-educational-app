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

    Q_OBJECT

private:
    bool isDogInPark;
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
    int animationFrameNumber;
    bool dogDirectionLeft;
    float walkSpeed = 0.015f;
    float runSpeed = 0.017f;
    b2Vec2 currentForce;
    std::vector<std::string> currentStateFlag = {"Sit", "Flip", "Walk"};
    std::string currentState;

public:
    Dog();
    virtual ~Dog();

    b2Vec2 UpdateDogState(bool);
    std::string getDogState();

    float getHunger();
    float getBathroom();
    int getTrustLevel();
    int getTrustProgress();
    bool getDogDirectionLeft();

    void feedFood();
    void feedTreat();
    void resetBathroom();
    void DogInPark(bool);
    void doesBallExist(bool);
    void doesFoodExist(bool);
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
