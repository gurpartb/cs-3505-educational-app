/**
  CS 3505 - A8 Final Project - QT Dogs
  Educational application to teach youth the importance of pet responsibility.
  Designed by:
  Brendan Johnston, Andrew Dron, Caleb Edwards, Colton Lee, Gurpartap Bhatti, Jacob Haydel, Tyler Trombley, Jared Hansen
*/

#include "model.h"

///
/// \brief Model::Model
/// Model Constructor.
///
Model::Model()
{

    gravity = new b2Vec2(0.0f,9.81f);
    world = new b2World(*gravity);

    srand(uint32_t(time(nullptr)));

    ballExists = false;
    treatExists = false;
    foodExists = false;

    ballTouchCount = 0;

    bathroomTimer = new QTimer(this);

    createScene();
    createDog();

    //connections to dog class
    connect(this, &Model::ballOnScreen, dog, &Dog::doesBallExist);
    connect(this, &Model::foodOnScreen, dog, &Dog::doesFoodExist);
    connect(this, &Model::treatOnScreen, dog, &Dog::doesTreatExist);
    connect(this, &Model::currentBallPosX, dog, &Dog::BallPositionX);
    connect(this, &Model::currentDogPosX, dog, &Dog::DogPositionX);
    connect(this, &Model::currentFoodPosX, dog, &Dog::FoodPositionX);
    connect(this, &Model::currentTreatPosX, dog, &Dog::TreatPositionX);

}

///
/// \brief Model::~Model
/// Model destructor.
///
Model::~Model(){

}

///
/// \brief Model::update
/// Main update method to handle all model updating functionality.
///
void Model::update()
{
    if (ballTouchCount > 0)
    {
        ballTouchCount--;
    }

    world->Step(1.0f/30.0f,8,3);

    dog->doesBallExist(ballExists);
    dog->doesFoodExist(foodExists);
    dog->doesTreatExist(treatExists);

    if(ballExists)
        dog->BallPositionX(ballX());
    if(foodExists)
        dog->FoodPositionX(foodX());
    if(treatExists)
        dog->TreatPositionX(treatX());

    dog->DogPositionX(dogX());

    dogBody->ApplyLinearImpulse(dog->UpdateDogState(isNight), dogBody->GetWorldCenter(),true);

    if(dog->getDogState() == "Barking")
    {
        emit playBarkSound();
    }

    dog->doesTrickExist(false);

    checkCollisions();

    if (ballExists)
    {
        emit currentBallPosX(ballX());
    }
    if (treatExists)
    {
        emit currentTreatPosX(treatX());
    }
    if (foodExists)
    {
        emit currentFoodPosX(foodX());
    }

    emit currentDogPosX(dogX());
    emit updateTrustLevel(dog->getTrustLevel());
    emit updateHungerLevel(dog->getHunger());
    emit updateBathroomLevel(dog->getBathroom());
    emit updateTrustProgress(dog->getTrustProgress());

}

///
/// \brief Model::createBall
/// Helper to create the ball on the screen.
///
void Model::createBall()
{
    ballExists = true;
    ballplayCount = 0;

    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(0.1f,1.0f);
    BodyDef.type = b2_dynamicBody;
    BodyDef.linearVelocity = b2Vec2(float(rand()) / float(RAND_MAX)*2.0f+1.0f,-float(rand()) / float(RAND_MAX)*2.0f-1.0f );
    BodyDef.angularVelocity = float(rand()) / float(RAND_MAX)*40.0f-20.0f;

    ball = world->CreateBody(&BodyDef);

    b2CircleShape shape;
    shape.m_radius = SCALE * 64.0f;
    b2FixtureDef FixtureDef;
    FixtureDef.density = 0.05f;
    FixtureDef.friction = 0.7f;
    FixtureDef.restitution = 0.7f;
    FixtureDef.shape = &shape;
    ball->CreateFixture(&FixtureDef);
    emit ballOnScreen(ballExists);
}

///
/// \brief Model::createDog
/// creates the dog on the screen using Box-2D.
///
void Model::createDog()
{
    dogExists = true;
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(0.5f,1.75f);
    BodyDef.type = b2_dynamicBody;
    dogBody = world->CreateBody(&BodyDef);
    b2Vec2 vertices[4];
    vertices[0].Set(SCALE*36.0f*2.0f,SCALE*26.0f*2.0f);
    vertices[1].Set(SCALE*36.0f*1.0f,-SCALE*26.0f*2.0f);
    vertices[2].Set(-SCALE*36.0f*1.0f,-SCALE*26.0f*2.0f);
    vertices[3].Set(-SCALE*36.0f*2.0f,SCALE*26.0f*2.0f);
    b2PolygonShape rect;
    rect.Set(vertices,4);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.restitution = 0.1f;
    FixtureDef.friction = 0.4f;
    FixtureDef.shape = &rect;
    dogBody->CreateFixture(&FixtureDef);

    dog = new Dog();
}

///
/// \brief Model::createTreat
/// Creates the treat on the screen using Box-2D.
///
void Model::createTreat()
{
    treatExists = true;

    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(1.0f,0.1f);
    BodyDef.type = b2_dynamicBody;
    BodyDef.linearVelocity = b2Vec2(float(rand()) / float(RAND_MAX)*2.0f-1.0f,0.0f);
    treat = world->CreateBody(&BodyDef);
    b2PolygonShape shape;
    shape.SetAsBox(SCALE*16.0f,SCALE*16.0f);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.7f;
    FixtureDef.shape = &shape;
    FixtureDef.restitution = 0.3f;
    treat->CreateFixture(&FixtureDef);
    emit treatOnScreen(treatExists);
}

///
/// \brief Model::createFood
/// Helper method to create the food using Box-2D.
///
void Model::createFood()
{
    foodExists = true;
    foodEatCount = 0;

    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(1.5f,1.7f);
    BodyDef.type = b2_dynamicBody;
    BodyDef.linearVelocity = b2Vec2(float(rand()) / float(RAND_MAX),0.0f);
    BodyDef.fixedRotation = true;
    food = world->CreateBody(&BodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(SCALE*46.0f,SCALE*30.0f);

    b2FixtureDef FixtureDef;
    FixtureDef.density = .80f;
    FixtureDef.friction = 0.2f;

    FixtureDef.shape = &shape;

    food->CreateFixture(&FixtureDef);
    emit treatOnScreen(treatExists);
}

///
/// \brief Model::createScene
/// Helper method to create the scene using Box-2D.
///
void Model::createScene()
{
    //ground
    {
        b2BodyDef BodyDef;
        BodyDef.position = b2Vec2(0.0f,2.9f);
        BodyDef.type = b2_staticBody;
        ground = world->CreateBody(&BodyDef);

        b2PolygonShape shape;
        shape.SetAsBox(2.0,1.0);

        b2FixtureDef FixtureDef;
        FixtureDef.density = 0.0f;
        FixtureDef.shape = &shape;
        ground->CreateFixture(&FixtureDef);
    }

    //ceiling
    {
        b2BodyDef BodyDef;
        BodyDef.position = b2Vec2(0.0,-1.0);
        BodyDef.type = b2_staticBody;
        ceiling = world->CreateBody(&BodyDef);

        b2PolygonShape shape;
        shape.SetAsBox(2.0,1.0);

        b2FixtureDef FixtureDef;
        FixtureDef.density = 0.f;
        FixtureDef.shape = &shape;
        ceiling->CreateFixture(&FixtureDef);
    }

    //right wall
    {
        b2BodyDef BodyDef;
        BodyDef.position = b2Vec2(3.0,2.0);
        BodyDef.type = b2_staticBody;
        rightWall = world->CreateBody(&BodyDef);

        b2PolygonShape shape;
        shape.SetAsBox(1.0,2.0);

        b2FixtureDef FixtureDef;
        FixtureDef.density = 0.f;
        FixtureDef.shape = &shape;
        rightWall->CreateFixture(&FixtureDef);
    }

    //left wall
    {
        b2BodyDef BodyDef;
        BodyDef.position = b2Vec2(-1.0,0.0);
        BodyDef.type = b2_staticBody;
        leftWall = world->CreateBody(&BodyDef);

        b2PolygonShape shape;
        shape.SetAsBox(1.0,2.0);

        b2FixtureDef FixtureDef;
        FixtureDef.density = 0.f;
        FixtureDef.shape = &shape;
        leftWall->CreateFixture(&FixtureDef);
    }
}

///
/// \brief Model::dogCollisions
/// Helper to handle dog collisions.
///
void Model::dogCollisions()
{
    for (b2ContactEdge* edge = dogBody->GetContactList() ; edge; edge = edge->next)
    {
        if (edge->contact->IsTouching())
        {

            if (edge->contact->GetFixtureB()->GetBody() == ball)
            {
                if (ballplayCount == 30){

                    ballExists = false;
                    ball->SetActive(false);
                }
                else
                {
                    emit playBallSound();
                    ballplayCount++;
                }
            }
            if (edge->contact->GetFixtureB()->GetBody() == treat)
            {
                treat->SetActive(false);
                treatExists = false;
                emit playEatSound();
                dog->feedTreat();
            }
            if (edge->contact->GetFixtureB()->GetBody() == food)
            {

                if (foodEatCount == 20)
                {
                    dog->feedFood();
                    food->SetActive(false);
                    emit playEatSound();
                    foodExists = false;
                }
                else
                {
                    foodEatCount++;
                }

            }
        }
    }
}

///
/// \brief Model::ballCollisions
/// Helper for the
///
void Model::ballCollisions()
{
    for (b2ContactEdge* edge = ball->GetContactList() ; edge; edge = edge->next)
    {
        if (edge->contact->IsTouching())
        {
            if (ballTouchCount < 1)
            {
                if (edge->contact->GetFixtureA()->GetBody() == rightWall)
                {
                    emit playBounceSound();
                }
                if (edge->contact->GetFixtureA()->GetBody() == leftWall)
                {
                    emit playBounceSound();
                }
                if (edge->contact->GetFixtureA()->GetBody() == ground)
                {
                    emit playBounceSound();
                }
                if (edge->contact->GetFixtureA()->GetBody() == ceiling)
                {
                    emit playBounceSound();
                }
                ballTouchCount+=10;
            }

        }
    }
}

///
/// \brief Model::treatCollisions
/// helper to handle the treat collisions.
///
void Model::treatCollisions()
{
    for (b2ContactEdge* edge = treat->GetContactList() ; edge; edge = edge->next)
    {
        if (edge->contact->IsTouching())
        {
            if (edge->contact->GetFixtureB()->GetBody() == ground)
            {
                    //emit falling sound
            }
        }
    }
}


/// \brief Model::dogTrick
/// Allows for the dog to do a flip when user presses the button.
///
void Model::dogTrick()
{
    if(!ballExists && !treatExists)
    {
        dog->doesTrickExist(true);
    }
}

void Model::dogTreat()
{
    deactivateAllObjects();
    createTreat();
}

///
/// \brief Model::dogFed
/// Drops food for the dog and handles eating.
///
void Model::dogFed()
{
    deactivateAllObjects();
    createFood();
}

///
/// \brief Model::deactivateAllObjects
///
void Model::deactivateAllObjects()
{
    if(ballExists)
    {
        ball->SetActive(false);
        ballExists = false;
    }
    if(treatExists)
    {
        treat->SetActive(false);
        treatExists = false;
    }
    if(foodExists)
    {
        food->SetActive(false);
        foodExists = false;
    }
}

///
/// \brief Model::dogPlayedWithBall
/// dog playing with the ball.
///
void Model::dogPlayedWithBall()
{
    deactivateAllObjects();
    createBall();
}

///
/// \brief Model::dogWentToThePark
/// Helper to let the dog go to the park.
///
void Model::dogWentToThePark(bool wentToPark)
{
    dog->DogInPark(wentToPark);
}

///
/// \brief Model::dogLetOut
/// Lets the dog out to the bathroom.
///
void Model::dogLetOut()
{
    dogRestroom();
    QTimer::singleShot(1000, this, SLOT(dogRestroom()));
}

///
/// \brief Model::dogRestroom
/// Flashes the dog using the restroom.
///
void Model::dogRestroom()
{
    if (!atPark)
    {
        if (dogExists)
        {
            dogExists = false;
        }
        else
        {
            dogExists = true;
        }

        dogBody->SetAwake(dogExists);

        if(dogExists)
        {
            dog->resetBathroom();
        }
    }
}

///
/// \brief Model::dogWalkLeft
/// Helper for the dog to walk left.
///
void Model::dogWalkLeft()
{
    b2Vec2 vec(-0.01f,0.0f);
    dogBody->ApplyLinearImpulse(vec,dogBody->GetWorldCenter(),true);
}

///
/// \brief Model::dogWalkRight
/// Helper to help the dog walk right.
///
void Model::dogWalkRight()
{
    b2Vec2 vec(0.01f,0.0f);
    dogBody->ApplyLinearImpulse(vec,dogBody->GetWorldCenter(),true);
}

///
/// \brief Model::checkCollisions
/// helper for the dog to check collisions.
///
void Model::checkCollisions()
{
    if(ballExists)
    {
        dogCollisions();
        ballCollisions();
    }
    if(treatExists)
    {
        dogCollisions();
        treatCollisions();
    }
    if(foodExists)
    {
        dogCollisions();
    }
}

