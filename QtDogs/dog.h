#ifndef DOG_H
#define DOG_H

#include "math.h"
#include <ctime>
class Dog
{

private:
    int hunger;
    int bathroom;
    int trustLevel;

public:
    Dog();

    int getHunger();
    bool increaseHunger();
    void feedTreat();
    void resetHunger();

    int getBathroom();
    bool increaseBathroom();
    void resetBathroom();

    int getTrustLevel();
    void increaseTrustLevel();
    void decreaseTrustLevel();
    void resetTrustLevel();

};

#endif // DOG_H
