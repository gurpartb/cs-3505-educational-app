#include "model.h"

Model::Model(){
    gravity = new b2Vec2(0.0f,9.81f);
    world = new b2World(*gravity);

    srand(uint32_t(time(nullptr)));
    ballExists = false;
    treatExists = false;
    createScene();
    //createBall();
    createDog();

    dog = new Dog();

    bathroomProgress = 0;
    hungerProgress = 0;
    trustProgress = 0;
    level = 1;

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
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(0.1f,1.0f);
    BodyDef.type = b2_dynamicBody;
    BodyDef.linearVelocity = b2Vec2(float(rand()) / float(RAND_MAX)*2.0f+1.0,-float(rand()) / float(RAND_MAX)*2.0-1.0 );
    ball = world->CreateBody(&BodyDef);
    //ball->ApplyAngularImpulse(32.0f, 32.0f);
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
    BodyDef.position = b2Vec2(0.4f,0.4f);
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
    FixtureDef.restitution = 0.1;
    FixtureDef.friction = 0.4f;
    FixtureDef.shape = &rect;

    dogBody->CreateFixture(&FixtureDef);
}

void Model::createTreat()
{
    treatExists = true;
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(1.0f,0.1f);
    BodyDef.type = b2_dynamicBody;
    BodyDef.linearVelocity = b2Vec2(float(rand()) / float(RAND_MAX),0.0f);
    treat = world->CreateBody(&BodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(0.01f,0.02f);

    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.7f;
    FixtureDef.shape = &shape;

    treat->CreateFixture(&FixtureDef);
    emit treatOnScreen(treatExists);

}

void Model::createFood()
{
    foodExists = true;
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(1.0f,0.1f);
    BodyDef.type = b2_dynamicBody;
    BodyDef.linearVelocity = b2Vec2(float(rand()) / float(RAND_MAX),0.0f);
    treat = world->CreateBody(&BodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(0.01f,0.02f);

    b2FixtureDef FixtureDef;
    FixtureDef.density = 2.f;
    FixtureDef.friction = 0.7f;
    FixtureDef.shape = &shape;

    treat->CreateFixture(&FixtureDef);
    emit treatOnScreen(treatExists);
}

void Model::createScene()
{
    //ground
    {
        b2BodyDef BodyDef;
        BodyDef.position = b2Vec2(0.0,2.9);
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
              if (edge->contact->GetFixtureB()->GetUserData() == rightWall->GetUserData())
              {
                  //emit pan right background
              }
              if (edge->contact->GetFixtureB()->GetUserData() == leftWall->GetUserData())
              {
                  //emit pan left background

              }
              if (edge->contact->GetFixtureB()->GetBody() == ball)
              {
                  //emit ball sound
                  world->DestroyBody(ball);
                  ballExists = false;
              }
              if (edge->contact->GetFixtureB()->GetUserData() == treat->GetUserData())
              {
                  //emit eating sound
                  //make treat disappear
                  //change state to idl
              }
              if (edge->contact->GetFixtureB()->GetUserData() == food->GetUserData())
              {
                  //emit eating sound
                  //make food disappear
                  //change state to idle
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
              if (edge->contact->GetFixtureB()->GetUserData() == rightWall->GetUserData())
              {
                  //emit bounce sound
              }
              if (edge->contact->GetFixtureB()->GetUserData() == leftWall->GetUserData())
              {
                  //emit bounce sound
              }
              if (edge->contact->GetFixtureB()->GetUserData() == ground->GetUserData())
              {
                  //emit bounce sound
              }
              if (edge->contact->GetFixtureB()->GetUserData() == ceiling->GetUserData())
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
              if (edge->contact->GetFixtureB()->GetUserData() == ground->GetUserData())
              {
                  //emit falling sound
              }
          }
    }
}

void Model::dogPetted(){

}

void Model::dogFed()
{
    if(ballExists)
    {
        world->DestroyBody(ball);
        ballExists = false;
    }
    if(treatExists)
    {
        world->DestroyBody(treat);
        treatExists = false;
    }
    createTreat();
}

void Model::dogPlayedWithBall()
{
    if(ballExists)
    {
        world->DestroyBody(ball);
        ballExists = false;
    }
    if(treatExists)
    {
        world->DestroyBody(treat);
        treatExists = false;
    }
    createBall();
}

void Model::dogWentToThePark(){

}

void Model::dogLetOut(){

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

    }
}

