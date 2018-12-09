#include "model.h"

Model::Model(){
    gravity = new b2Vec2(0.0f,9.81f);
    world = new b2World(*gravity);

   // world->list

    srand(uint32_t(time(nullptr)));

    ballExists = false;
    treatExists = false;
    foodExists = false;

    createScene();
    createDog();

//    bathroomProgress = 0;
//    hungerProgress = 0;
//    trustProgress = 0;
//    level = 1;

    //connections to dog class
    connect(this, &Model::ballOnScreen, dog, &Dog::doesBallExist);
    connect(this, &Model::foodOnScreen, dog, &Dog::doesFoodExist);
    connect(this, &Model::treatOnScreen, dog, &Dog::doesTreatExist);
    connect(this, &Model::currentBallPosX, dog, &Dog::BallPositionX);
    connect(this, &Model::currentDogPosX, dog, &Dog::DogPositionX);
    connect(this, &Model::currentFoodPosX, dog, &Dog::FoodPositionX);
    connect(this, &Model::currentTreatPosX, dog, &Dog::TreatPositionX);
}

Model::~Model(){

}

void Model::update()
{
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

void Model::createDog()
{
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
    //rect.SetAsBox(SCALE*36*2.0,SCALE*26*2.0);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.restitution = 0.1f;
    FixtureDef.friction = 0.4f;
    FixtureDef.shape = &rect;
    dogBody->CreateFixture(&FixtureDef);

    dog = new Dog();
}

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

void Model::createFood()
{
    foodExists = true;

    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(1.5f,1.7f);
   // BodyDef.position = b2Vec2(.7f,1.0f);
    BodyDef.type = b2_dynamicBody;
    BodyDef.linearVelocity = b2Vec2(float(rand()) / float(RAND_MAX),0.0f);
    BodyDef.fixedRotation = true;
    food = world->CreateBody(&BodyDef);

    b2PolygonShape shape;
    //shape.SetAsBox(SCALE*4.0f,SCALE*4.0f);
    shape.SetAsBox(SCALE*46.0f,SCALE*30.0f);

    b2FixtureDef FixtureDef;
    FixtureDef.density = .80f;
    FixtureDef.friction = 0.2f;

    FixtureDef.shape = &shape;

    food->CreateFixture(&FixtureDef);
    emit treatOnScreen(treatExists);
}

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

void Model::dogCollisions()
{
    for (b2ContactEdge* edge = dogBody->GetContactList() ; edge; edge = edge->next)
    {
        if (edge->contact->IsTouching())
        {
            if (edge->contact->GetFixtureB()->GetBody() == rightWall)
            {
                //emit pan right background
            }
            if (edge->contact->GetFixtureB()->GetBody() == leftWall)
            {
                //emit pan left background
            }
            if (edge->contact->GetFixtureB()->GetBody() == ball)
            {
                //emit ball sound
                if (ballplayCount == 30){

                    ballExists = false;
                    ball->SetActive(false);
                }
                else
                {
                    ballplayCount++;
                }
            }
            if (edge->contact->GetFixtureB()->GetBody() == treat)
            {
                treat->SetActive(false);
                treatExists = false;
                dog->feedTreat();
            }
            if (edge->contact->GetFixtureB()->GetBody() == food)
            {
                //emit eating sound
                //make food disappear
                //change state to idle
                dog->feedFood();
            }
        }
    }
}

void Model::ballCollisions()
{
    for (b2ContactEdge* edge = ball->GetContactList() ; edge; edge = edge->next)
    {
        if (edge->contact->IsTouching())
        {
            if (edge->contact->GetFixtureB()->GetBody() == rightWall)
            {
                //emit bounce sound
            }
            if (edge->contact->GetFixtureB()->GetBody() == leftWall)
            {
                //emit bounce sound
            }
            if (edge->contact->GetFixtureB()->GetBody() == ground)
            {
                //emit bounce sound
            }
            if (edge->contact->GetFixtureB()->GetBody() == ceiling)
            {
                //emit bounce sound
            }
        }
    }
}

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
    //emit dog flip
    emit playWhistleSound();
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

void Model::dogPlayedWithBall()
{
    deactivateAllObjects();
    createBall();
}

void Model::dogWentToThePark(bool wentToPark)
{
    dog->DogInPark(wentToPark);
}

void Model::dogLetOut()
{
    dog->resetBathroom();
}

void Model::dogWalkLeft()
{
    b2Vec2 vec(-0.01f,0.0f);
    dogBody->ApplyLinearImpulse(vec,dogBody->GetWorldCenter(),true);
}

void Model::dogWalkRight()
{
    b2Vec2 vec(0.01f,0.0f);
    dogBody->ApplyLinearImpulse(vec,dogBody->GetWorldCenter(),true);
}

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

