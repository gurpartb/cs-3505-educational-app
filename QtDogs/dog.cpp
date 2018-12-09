/**
  CS 3505 - A8 Final Project - QT Dogs
  Educational application to teach youth the importance of pet responsibility.
  Designed by:
  Brendan Johnston, Andrew Dron, Caleb Edwards, Colton Lee, Gurpartap Bhatti, Jacob Haydel, Tyler Trombley, Jared Hansen
*/

#include "dog.h"

///
/// \brief Dog::Dog:
/// Dog constructor.
///
Dog::Dog()
{
    currentBallPositionX = 0;
    currentDogPositionX = 0;
    currentFoodPositionX = 0;
    currentTreatPositionX = 0;
    hunger = 99;
    bathroom = 0;
    trustLevel = 0;
    currentState = "Idle";
    isDogInPark = false;
}

///
/// \brief Dog::~Dog
/// Dog Destructor
///
Dog::~Dog()
{

}

///
/// \brief Dog::UpdateDogState
/// Updates the current dog state, main AI for the entire project.
/// \param isNight - Determines if the dog should be sleeping or not.
/// \return Vector of moving positions.
///
b2Vec2 Dog::UpdateDogState(bool isNight){

    bool isHungry = increaseHunger();
    bool needsToGo = false;

    if(hunger >= 50) {
        needsToGo = increaseBathroom();
    }

    if(!isDogInPark)
    {
        if(isNight && !(currentState == "Dead")) {
            currentState = "BeginSleeping";
        }

        //State behavior
        if(currentState == "Idle")
        {
            currentForce = b2Vec2_zero;
            float stateFlagChoice = ((static_cast<float>(rand()) * 1.0f) / static_cast<float>(RAND_MAX));

            std::string stateFlag = currentStateFlag[3];
            if(stateFlagChoice < 0.50f)
                stateFlag = currentStateFlag[0];
            else if(stateFlagChoice < 0.85f)
                stateFlag = currentStateFlag[1];
            else if(stateFlagChoice < 0.95f)
                stateFlag = currentStateFlag[2];


            if(isHungry && hunger >= 0 && stateFlagChoice < 0.05)
            {
                currentState = "Barking";
            }
            else if(needsToGo)
            {
                currentState = "Peeing";
            }
            else if(ballExists)
            {
                currentState = "Playing";
            }
            else if((foodExists || treatExists) && isHungry)
            {
                currentState = "Eating";
            }
            else if(stateFlag == "Sit")
            {
                currentState = "Sitting";
            }
            else if(stateFlag == "Flip")
            {
                currentState = "Flipping";
            }
            else if(stateFlag == "Walk")
            {
                currentState = "Walking";
            }
        }
        else if(currentState == "Walking")
        {
            if(getRandomDogDirectionLeft())
            {
                dogDirectionLeft = (dogDirectionLeft) ? false : true;
            }
            if(dogDirectionLeft)
                currentForce.x = -walkSpeed;
            else
                currentForce.x = walkSpeed;

            //Random change of behavior to running or idle
            int behaviorChangeChance = int((static_cast<float>(rand()) * 100.0f) / static_cast<float>(RAND_MAX));

            if(behaviorChangeChance < 10)
            {
                currentState = "Idle";
            }

            if(ballExists)
            {
                currentState = "Playing";
            }
            else if((foodExists || treatExists) && isHungry)
            {
                currentState = "Eating";
            }
        }
        else if(currentState == "Sitting")
        {
            currentForce = b2Vec2_zero;
            //Random change of behavior to idle
            int behaviorChangeChance = int((static_cast<float>(rand()) * 100.0f) / static_cast<float>(RAND_MAX));
            if(behaviorChangeChance < 20)
            {
                currentState = "Idle";
            }

        }
        else if(currentState == "Running")
        {
            if(getDogDirectionLeft())
            {
                dogDirectionLeft = true;
                currentForce.x = -runSpeed;
            }
            else
            {
                dogDirectionLeft = false;
                currentForce.x = runSpeed;
            }
            int behaviorChangeChance = int((static_cast<float>(rand()) * 100.0f) / static_cast<float>(RAND_MAX));
            if(behaviorChangeChance < 50)
            {
                currentState = "Walking";
            }
            if(ballExists)
            {
                currentState = "Playing";
            }
            else if(foodExists && isHungry)
            {
                currentState = "Eating";
            }
            else if(treatExists)
            {
                currentState = "Eating";
            }
        }
        else if(currentState == "Peeing")
        {
            currentForce = b2Vec2_zero;
            resetBathroom();
            currentState = "Idle";
        }
        else if(currentState == "Flipping")
        {
            currentForce = b2Vec2_zero;
            //Random change of behavior to idle
            srand(static_cast <unsigned int> (time(nullptr)));
            int behaviorChangeChance = int((static_cast<float>(rand()) * 100.0f) / static_cast<float>(RAND_MAX));
            if(behaviorChangeChance < 80)
            {
                currentState = "Idle";
            }
        }
        else if(currentState == "Barking")
        {
            currentForce = b2Vec2_zero;
            //Bark at random intervals
            int barkChance = int((static_cast<float>(rand()) * 100.0f) / static_cast<float>(RAND_MAX));
            decreaseTrustProgress();
            if(barkChance < 5)
            {
                currentState = "Idle";
            }
        }
        else if(currentState == "BeginSleeping")
        {
            currentForce = b2Vec2_zero;

            currentState = "Sleeping";
        }
        else if(currentState == "Sleeping")
        {
            currentForce = b2Vec2_zero;
            if(!isNight)
            {
                currentState = "EndSleeping";
            }
        }
        else if(currentState == "EndSleeping")
        {
            currentForce = b2Vec2_zero;
            currentState = "Idle";
        }
        else if(currentState == "BeginDeath")
        {
            currentForce = b2Vec2_zero;
            currentState = "Death";
        }
        else if(currentState == "Death")
        {
            currentForce = b2Vec2_zero;
        }
        else if(currentState == "Playing")
        {
            if(ballExists)
            {
                if(currentBallPositionX > currentDogPositionX)
                {
                    dogDirectionLeft = false;
                    currentForce.x = runSpeed;
                }
                else if(currentBallPositionX < currentDogPositionX)
                {
                    dogDirectionLeft = true;
                    currentForce.x = -runSpeed;
                }
                else
                {
                    currentForce = b2Vec2_zero;
                }
            }
            else
            {
                currentState = "Idle";
            }

        }
        else if(currentState == "Eating") {
            if(foodExists)
            {
                if(currentFoodPositionX > currentDogPositionX)
                {
                    dogDirectionLeft = false;
                    currentForce.x = runSpeed;
                }
                else if(currentFoodPositionX < currentDogPositionX)
                {
                    dogDirectionLeft = true;
                    currentForce.x = -runSpeed;
                }
            }
            else if(treatExists)
            {
                if(currentTreatPositionX > currentDogPositionX)
                {
                    dogDirectionLeft = false;
                    currentForce.x = runSpeed;
                }
                else if(currentTreatPositionX < currentDogPositionX)
                {
                    dogDirectionLeft = true;
                    currentForce.x = -runSpeed;
                }
            }
            else
            {
                currentState = "Idle";
            }
        }
    }
    else
    {
        if(currentState == "Idle")
        {
            currentForce = b2Vec2_zero;
            currentState = "Walking";
        }
        else if(currentState == "Walking")
        {
            if(getDogDirectionLeft())
            {
                dogDirectionLeft = true;
                currentForce.x = -walkSpeed;
            }
            else
            {
                dogDirectionLeft = false;
                currentForce.x = walkSpeed;
            }

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
            else if(foodExists || treatExists)
            {
                currentState = "Eating";
            }

        }
        else if(currentState == "Eating")
        {
            if(foodExists)
            {
                if(currentFoodPositionX > currentDogPositionX)
                {
                    dogDirectionLeft = false;
                    currentForce.x = runSpeed;
                }
                else if(currentFoodPositionX < currentDogPositionX)
                {
                    dogDirectionLeft = true;
                    currentForce.x = -runSpeed;
                }
            }
            else if(treatExists)
            {
                if(currentTreatPositionX > currentDogPositionX)
                {
                    dogDirectionLeft = false;
                    currentForce.x = runSpeed;
                }
                else if(currentTreatPositionX < currentDogPositionX)
                {
                    dogDirectionLeft = true;
                    currentForce.x = -runSpeed;
                }
            }
            else
            {
                currentState = "Idle";
            }
        }
        else if(currentState == "Playing")
        {
            if(ballExists)
            {
                if(currentBallPositionX > currentDogPositionX)
                {
                    dogDirectionLeft = false;
                    currentForce.x = runSpeed;
                }
                else if(currentBallPositionX < currentDogPositionX)
                {
                    dogDirectionLeft = true;
                    currentForce.x = -runSpeed;
                }
                else
                {
                    currentForce = b2Vec2_zero;
                }
            }
            else
            {
                currentState = "Idle";
            }
        }
        else if(currentState == "Bark")
        {
            currentForce = b2Vec2_zero;
            //Bark at random intervals
            int barkChance = int((static_cast<float>(rand()) * 100.0f) / static_cast<float>(RAND_MAX));
            decreaseTrustProgress();
            if(!(barkChance % 100))
            {
                currentState = "Idle";
            }
        }
        else if(currentState == "Dead")
        {
            currentForce = b2Vec2_zero;
        }
        else if(currentState == "Peeing")
        {
            currentForce = b2Vec2_zero;
            resetBathroom();
            currentState = "Idle";
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
/// \brief Dog::getRandomDogDirectionLeft
/// \return returns true if dog is heading left. False otherwise.
///
bool Dog::getRandomDogDirectionLeft()
{
    float dogDirectionChance = (static_cast<float>(rand())*1.0f / static_cast<float>(RAND_MAX));
    //qDebug() << (dogDirectionLeft);
    return (dogDirectionChance < 0.05f);
}

///
/// \brief Dog::getDogDirectionLeft
/// \return returns true if dog is heading left. False otherwise.
///
bool Dog::getDogDirectionLeft()
{
    return dogDirectionLeft;
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
    hunger-= 0.0555555555555555555;
    if (hunger < 100)
    {
        if (hunger > 70)
        {
            return false;
        }
        else if(hunger <= 0)
        {
            currentState = "BeginDeath";
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
        int rng = rand();
        if (rng % 2 == 0)
        {
            increaseTrustProgress();
        }
    }
}

///
/// \brief Dog::feedFood
/// Gives the dog a full hunger bar
///
void Dog::feedFood()
{
    if (hunger > 0)
    {
        hunger += 100;
        //Clamp hunger to set interval
        hunger = fmin(fmax(hunger, 0.0f), 100.0f);
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
        bathroom += 0.0055555f;
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
/// \returnEnabled
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

///
/// \brief Dog::DogInPark
/// Sets if the dog is currently in the park.
///
void Dog::DogInPark(bool inPark)
{
    isDogInPark = inPark;
}

///
/// \brief Dog::doesBallExist
/// Setter for showing that the ball is existing on screen.
///
void Dog::doesBallExist(bool exists)
{
    ballExists = exists;
}

///
/// \brief Dog::doesFoodExist
/// Setter for showing if the food is existing on screen.
///
void Dog::doesFoodExist(bool exists)
{
    foodExists = exists;
}

///
/// \brief Dog::doesTreatExist
/// Setter for showing if the treat is existing on screen.
///
void Dog::doesTreatExist(bool exists)
{
    treatExists = exists;
}

///
/// \brief Dog::BallPositionX
/// Setter for setting the Ball position X direction.
///
void Dog::BallPositionX(float position)
{
    currentBallPositionX = position;
}

///
/// \brief Dog::DogPositionX
/// Setter for setting the Dog position X direction.
///
void Dog::DogPositionX(float position)
{
    currentDogPositionX = position;
}

///
/// \brief Dog::FoodPositionX
///  Setter for setting the Foods position X direction.
///
void Dog::FoodPositionX(float position)
{
    currentFoodPositionX = position;
}

///
/// \brief Dog::TreatPositionX
///  Setter for setting the Treat position X direction.
///
void Dog::TreatPositionX(float position)
{
    currentTreatPositionX = position;
}
