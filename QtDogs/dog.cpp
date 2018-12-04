#include "dog.h"

///
/// \brief Dog::Dog:
/// Dog constructor.
///
Dog::Dog()
{
    hunger = 0;
    bathroom = 0;
    trustLevel = 0;
}

///
/// \brief Dog::getHunger
/// Gets the current hunger status of the dog from 0 to 100.
/// \return 0 to 100.
///
int Dog::getHunger()
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
        hunger+= 10;
        if (hunger > 80)
        {
            return true;
        }
        return false;
    }
    return true;
}

///
/// \brief Dog::feedTreat
/// Gives the dog a little loss in hunger and might slightly increases the trust level.
///
void Dog::feedTreat()
{
    if (hunger > 0)
    {
        hunger -= 10;
        srand(static_cast <unsigned int> (time(nullptr)));
        int rng = rand();
        if (rng % 2 == 0)
        {
            increaseTrustLevel();
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
int Dog::getBathroom()
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
        bathroom+= 10;
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
/// Returns trust level of the dog.
///
int Dog::getTrustLevel()
{
    return trustLevel;
}

///
/// \brief Dog::increaseTrustLevel
/// Increases current trust level between 0 to 10.
/// \return
/// New value of the trust level.
///
void Dog::increaseTrustLevel()
{
    if (trustLevel < 100)
    {
        trustLevel++;
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
    if (trustLevel > 0)
    {
        trustLevel--;
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
