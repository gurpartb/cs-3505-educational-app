#include "model.h"

Model::Model(){
    gravity = new b2Vec2(0.0f,9.81f);
    world = new b2World(*gravity);

    srand(uint32_t(time(nullptr)));

    createScene();
    createBall();

    dog = new Dog();

    bathroomProgress = 0;
    hungerProgress = 0;
    trustProgress = 0;
    level = 1;
}

Model::~Model(){

}

void Model::update()
{
    world->Step(1.0f/30.0f,8,3);
}

void Model::createBall()
{



    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(0.1f,1.0f);
    BodyDef.type = b2_dynamicBody;
    BodyDef.linearVelocity = b2Vec2(float(rand()) / float(RAND_MAX)*2.0f+1.0,-float(rand()) / float(RAND_MAX)*2.0-1.0 );
    ball = world->CreateBody(&BodyDef);

    b2CircleShape shape;
    shape.m_radius = SCALE * 64.0f;

    b2FixtureDef FixtureDef;
    FixtureDef.density = 0.5f;
    FixtureDef.friction = 0.7f;
    FixtureDef.restitution = 0.4f;
    FixtureDef.shape = &shape;

    ball->CreateFixture(&FixtureDef);
}

void Model::createTreat()
{
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(1.0f,0.1f);
    BodyDef.type = b2_dynamicBody;
    BodyDef.linearVelocity = b2Vec2(float(rand()) / float(RAND_MAX),0.0f);
    treat = world->CreateBody(&BodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(0.1f,0.2f);

    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.7f;
    FixtureDef.shape = &shape;

    treat->CreateFixture(&FixtureDef);
}

void Model::createScene()
{
    //ground
    {
        b2BodyDef BodyDef;
        BodyDef.position = b2Vec2(0.0,2.9);
        BodyDef.type = b2_staticBody;
        b2Body* Body = world->CreateBody(&BodyDef);

        b2PolygonShape shape;
        shape.SetAsBox(2.0,1.0);
        b2FixtureDef FixtureDef;
        FixtureDef.density = 0.0f;
        FixtureDef.shape = &shape;
        Body->CreateFixture(&FixtureDef);
    }

    //ceiling
    {
        b2BodyDef BodyDef;
        BodyDef.position = b2Vec2(0.0,-1.0);
        BodyDef.type = b2_staticBody;
        b2Body* Body = world->CreateBody(&BodyDef);

        b2PolygonShape shape;
        shape.SetAsBox(2.0,1.0);
        b2FixtureDef FixtureDef;
        FixtureDef.density = 0.f;
        FixtureDef.shape = &shape;
        Body->CreateFixture(&FixtureDef);
    }

    //right wall
    {
        b2BodyDef BodyDef;
        BodyDef.position = b2Vec2(3.0,2.0);
        BodyDef.type = b2_staticBody;
        b2Body* Body = world->CreateBody(&BodyDef);

        b2PolygonShape shape;
        shape.SetAsBox(1.0,2.0);
        b2FixtureDef FixtureDef;
        FixtureDef.density = 0.f;
        FixtureDef.shape = &shape;
        Body->CreateFixture(&FixtureDef);
    }

    //left wall
    {
        b2BodyDef BodyDef;
        BodyDef.position = b2Vec2(-1.0,0.0);
        BodyDef.type = b2_staticBody;
        b2Body* Body = world->CreateBody(&BodyDef);

        b2PolygonShape shape;
        shape.SetAsBox(1.0,2.0);
        b2FixtureDef FixtureDef;
        FixtureDef.density = 0.f;
        FixtureDef.shape = &shape;
        Body->CreateFixture(&FixtureDef);
    }
}

void Model::dogPetted(){

}

void Model::dogFed(){

}

void Model::dogPlayedWithBall()
{
    world->DestroyBody(ball);
    createBall();
}

void Model::dogWentToThePark(){

}

void Model::dogLetOut(){

}


///
/// \brief Model::updateBathroomProgress
///
void Model::updateBathroomProgress()
{
    bathroomProgress++;
    if(bathroomProgress == 100)
    {
        bathroomProgress = 0;
        if(trustProgress >= 10)
            trustProgress -= 10;
        else
            trustProgress = 0;
        emit updateTrustProgress(trustProgress);
    }
    emit updateBathroomLevel(bathroomProgress);
}

///
/// \brief Model::resetBathroomProgress
///
void Model::resetBathroomProgress()
{
    bathroomProgress = 0;
    trustProgress += 10;
    if(trustProgress == 100)
    {
        trustProgress = 0;
        level++;
        emit updateLevels(level);
    }
    emit updateTrustProgress(trustProgress);
    emit updateBathroomLevel(bathroomProgress);
}

///
/// \brief Model::updateHungerProgress
///
void Model::updateHungerProgress()
{
    hungerProgress++;
    if(hungerProgress == 100)
    {
        hungerProgress = 0;
        if(trustProgress >= 5)
            trustProgress -= 5;
        else
            trustProgress = 0;
        emit updateTrustProgress(trustProgress);
    }
    emit updateHungerLevel(hungerProgress);
}

///
/// \brief Model::resetHungerProgress
///
void Model::resetHungerProgress()
{
    hungerProgress = 0;
    trustProgress += 5;
    if(trustProgress == 100)
    {
        trustProgress = 0;
        level++;
        emit updateLevels(level);
    }
    emit updateTrustProgress(trustProgress);
    emit updateHungerLevel(hungerProgress);
}


