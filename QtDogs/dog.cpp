#include "dog.h"
#include <math.h>
#include "model.h"

///
/// \brief Dog::Dog:
/// Dog constructor.
///
Dog::Dog()
{
    currentBallPositionX = 0;
    hunger = 0;
    bathroom = 0;
    trustLevel = 0;
    currentState = "Idle";
}

b2Vec2 Dog::UpdateDogState(bool isNight){

    bool isHungry = increaseHunger();
    bool needsToGo = false;

    if(hunger >= 50) {
        needsToGo = increaseBathroom();
    }
    if(isNight && !(currentState == "Dead")) {
        currentState = "Sleeping";
    }

    //State behavior
    if(currentState == "Idle")
    {
        currentForce = b2Vec2_zero;
        srand(static_cast <unsigned int> (time(nullptr)));
        unsigned long stateFlagChoice = (unsigned long)((static_cast<float>(rand()) * 3.0f) / static_cast<float>(RAND_MAX));
        std::string stateFlag = currentStateFlag[stateFlagChoice];

        if(isHungry && hunger >= 0)
        {
            currentState = "Bark";
        }
        else if(needsToGo)
        {
            currentState = "Peeing";
        }
        else if(ballExists)
        {
            currentState = "Playing";
        }
        else if(stateFlag == "Sit")
        {
            currentState = "Sitting";
        }
        else if(stateFlag == "Flip")
        {
            currentState = "Flip";
        }
        else if(stateFlag == "Walk")
        {
            currentState = "Walking";
        }
    }
    else if(currentState == "Walking")
    {
        if(getDogDirectionLeft())
        {
            currentForce.x = -walkSpeed;
        }
        else
        {
            currentForce.x = walkSpeed;
        }

        //Random change of behavior to running or idle
        srand(static_cast <unsigned int> (time(nullptr)));
        int behaviorChangeChance = int((static_cast<float>(rand()) * 100.0f) / static_cast<float>(RAND_MAX));
        if(behaviorChangeChance < 33)
        {
            currentState = "Running";
        }
        else if(behaviorChangeChance >= 33)
        {
            currentState = "Idle";
        }
        if(ballExists)
        {
            currentState = "Playing";
        }
    }
    else if(currentState == "Sitting")
    {
        currentForce = b2Vec2_zero;
    }
    else if(currentState == "Running")
    {
        if(getDogDirectionLeft())
        {
            currentForce.x = -runSpeed;
        }
        else
        {
            currentForce.x = runSpeed;
        }
        srand(static_cast <unsigned int> (time(nullptr)));
        int behaviorChangeChance = int((static_cast<float>(rand()) * 100.0f) / static_cast<float>(RAND_MAX));
        if(behaviorChangeChance < 50)
        {
            currentState = "Walking";
        }
        if(ballExists)
        {
            currentState = "Playing";
        }
    }
    else if(currentState == "Peeing")
    {
        currentForce = b2Vec2_zero;
        currentState = "Idle";
    }
    else if(currentState == "Flip")
    {
        currentForce = b2Vec2_zero;
        currentState = "Idle";
    }
    else if(currentState == "Bark")
    {
        currentForce = b2Vec2_zero;
        //Bark at random intervals
        srand(static_cast <unsigned int> (time(nullptr)));
        int barkChance = int((static_cast<float>(rand()) * 100.0f) / static_cast<float>(RAND_MAX));
        decreaseTrustProgress();
        //Bark here
        if(!(barkChance % 100))
        {
            currentState = "Idle";
        }
    }
    else if(currentState == "Sleeping")
    {
        currentForce = b2Vec2_zero;
        if(!isNight)
        {
            currentState = "Idle";
        }
    }
    else if(currentState == "Dead")
    {
        currentForce = b2Vec2_zero;
    }
    else if(currentState == "Playing")
    {
        if(currentBallPositionX > currentDogPositionX)
        {
            currentForce.x = runSpeed;
        }
        else if(currentBallPositionX < currentDogPositionX)
        {
            currentForce.x = -runSpeed;
        }
        else
        {
            currentForce = b2Vec2_zero;
        }

    }

    return currentForce;
}

///
/// \brief Dog::getDogState
/// \return current state of Dog
///
std::string Dog::getDogState()
{
    return currentState;
}

///
/// \brief Dog::getNumOfFrames
/// \return current number of frames for current animation
///
int Dog::getNumOfFrames()
{
    return currentNumOfFrames;
}

///
/// \brief Dog::getDogDirectionLeft
/// \return returns true if dog is heading left. False otherwise.
///
bool Dog::getDogDirectionLeft()
{
    srand(static_cast <unsigned int> (time(nullptr)));
    int dogDirectionChance = int(static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    if(dogDirectionChance == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

///
/// \brief Dog::getHunger
/// Gets the current hunger status of the dog from 0 to 100.
/// \return 0 to 100.
///
float Dog::getHunger()
{
    return hunger;
}

///
/// \brief increaseHunger
/// Increases dogs hunger level
/// \return Boolean value to return if the dog is currently hungry.
///
bool Dog::increaseHunger()
{
    if (hunger < 100)
    {
        hunger-= 0.0055555555555555555;
        if (hunger > 70)
        {
            return false;
        }
        else if(hunger <= 0)
        {
            currentState = "Dead";
        }
        return true;
    }
    return false;
}

///
/// \brief Dog::feedTreat
/// Gives the dog a little loss in hunger and might slightly increases the trust level.
///
void Dog::feedTreat()
{
    if (hunger > 0)
    {
        hunger += 10;
        //Clamp hunger to set interval
        hunger = fmin(fmax(hunger, 0.0f), 100.0f);
        srand(static_cast <unsigned int> (time(nullptr)));
        int rng = rand();
        if (rng % 2 == 0)
        {
            increaseTrustProgress();
        }
    }
}

///
/// \brief Dog::resetHunger
/// Resets the hunger after eating.
///
void Dog::resetHunger()
{
    hunger = 0;
}

///
/// \brief Dog::getBathroom
/// Returns how much the dog has to use the restroom.
/// \return
///
float Dog::getBathroom()
{
    return bathroom;
}

///
/// \brief Dog::increaseBathroom
/// \return Bool value if the dog will immediately use the bathroom.
///
bool Dog::increaseBathroom()
{
    if (bathroom < 90)
    {
        bathroom += 1;
        if ((bathroom > 60 && trustLevel < 6) || (bathroom > 80 && trustLevel < 7))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return true;
}

///
/// \brief Dog::resetBathroom:
/// Resets when dog uses restroom.
///
void Dog::resetBathroom()
{
    bathroom = 0;
}

///
/// \brief Dog::getTrustLevel
/// \return Returns trust level of the dog.
///
int Dog::getTrustLevel()
{
    return trustLevel;
}

///
/// \brief Dog::getTrustProgress
/// \return Returns trust progress of the current trust level
///
int Dog::getTrustProgress()
{
    return trustProgress;
}

///
/// \brief Dog::increaseTrustLevel
/// Increases current trust level between 0 to 10.
///
void Dog::increaseTrustLevel()
{
    if (trustLevel < 10 && (trustProgress == 100))
    {
        trustLevel++;
    }
}

///
/// \brief Dog::increaseTrustProgress
///
void Dog::increaseTrustProgress()
{
    if(trustProgress < 100)
    {
        trustProgress += 10;
    }
    else
    {
        increaseTrustLevel();
        trustProgress = 0;
    }
}

///
/// \brief Dog::decreaseTrustLevel
/// decreases the trust level if the you do something to distrust the dog. 0 to 10 value.
/// \return
/// New value of the trust level.
///
void Dog::decreaseTrustLevel()
{
    if (trustLevel > 0 && (trustProgress == 0))
    {
        trustLevel--;
    }
}

///
/// \brief Dog::decreaseTrustProgress
/// decreases the trust progress
///
void Dog::decreaseTrustProgress()
{
    if(trustProgress > 0)
    {
        trustProgress -= 10;
    }
    else
    {
        decreaseTrustLevel();
        trustProgress = 90;
    }
}

///
/// \brief Dog::resetTrustLevel
/// Resets the Trust level on a new game or major issue.
///
void Dog::resetTrustLevel()
{
    trustLevel = 0;
}

void Dog::doesBallExist(bool exists)
{
    ballExists = exists;
}

void Dog::BallPositionX(float position)
{
    currentBallPositionX = position;
}

void Dog::DogPositionX(float position)
{
    currentDogPositionX = position;
}
