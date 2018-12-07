#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include "dog.h"

#define SCALE 2.0f/768.0f

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

    int bathroomProgress;
    int hungerProgress;
    int trustProgress;
    int level;

    bool ballExists;
    bool treatExists;
    bool foodExists;

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



private:
    void createBall();
    void createTreat();
    void createFood();
    void createScene();
    void createDog();

public slots:
    void dogPetted();
    void dogFed();
    void dogPlayedWithBall();
    void dogWentToThePark();
    void dogLetOut();
    void dogWalkLeft();
    void dogWalkRight();

signals:
    void updateLevels(int);
    void updateTrustLevel(float);
    void updateHungerLevel(float);
    void updateBathroomLevel(float);
    void updateTrustProgress(float);
    void ballOnScreen(bool);
    void foodOnScreen(bool);
    void treatOnScreen(bool);
    void currentBallPosX(float);
    void currentDogPosX(float);
    void currentFoodPosX(float);
    void currentTreatPosX(float);

};

#endif // MODEL_H
