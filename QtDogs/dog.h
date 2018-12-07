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
    float currentBallPositionX;
    float currentDogPositionX;
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
    int getNumOfFrames();


private:
    bool getDogDirectionLeft();

    float getHunger();
    bool increaseHunger();
    void feedTreat();
    void resetHunger();

    float getBathroom();
    bool increaseBathroom();
    void resetBathroom();

    int getTrustLevel();
    int getTrustProgress();
    void increaseTrustLevel();
    void increaseTrustProgress();
    void decreaseTrustProgress();
    void decreaseTrustLevel();
    void resetTrustLevel();

public slots:
    void doesBallExist(bool);
    void BallPositionX(float);
    void DogPositionX(float);
};

#endif // DOG_H
