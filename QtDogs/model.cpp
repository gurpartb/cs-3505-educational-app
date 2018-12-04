#include "model.h"

Model::Model(){
    updateLevels();
}

Model::~Model(){

}

void Model::dogPetted(){
    myDog.increaseTrustLevel();
    updateLevels();
}

void Model::dogFed(){
    myDog.increaseTrustLevel();
    myDog.increaseTrustLevel();
    myDog.increaseTrustLevel();
    myDog.resetHunger();
    updateLevels();
}

void Model::dogPlayedWithBall(){
    myDog.increaseHunger();
    updateLevels();
}

void Model::dogWentToThePark(){
    myDog.increaseTrustLevel();
    myDog.increaseTrustLevel();
    myDog.increaseHunger();
    updateLevels();
}

void Model::dogLetOut(){
    myDog.increaseTrustLevel();
    myDog.resetBathroom();
    updateLevels();
}

void Model::updateLevels(){
    emit updateTrustLevel(myDog.getTrustLevel());
    emit updateHungerLevel(myDog.getHunger());
    emit updateBathroomLevel(myDog.getBathroom());
}


