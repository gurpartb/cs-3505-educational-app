#include "mainwindow.h"
#include <QApplication>
#include "gravity.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>


#define SCALE 10.f

void CreateBox(b2World& World, int MouseX, int MouseY)
{
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(MouseX/SCALE, MouseY/SCALE);
    BodyDef.type = b2_dynamicBody;
    b2Body* Body = World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((32.0f/2)/SCALE, (32.0f/2)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.0f;
    FixtureDef.friction = 0.7f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
}

void CreateGround(b2World& World, float X, float Y)
{
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(X/SCALE, Y/SCALE);
    BodyDef.type = b2_staticBody;
    b2Body* Body = World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((512.0f/2)/SCALE, (16.0f/2)/SCALE); // Creates a box shape. Divide your desired width and height by 2.
    b2FixtureDef FixtureDef;
    FixtureDef.density = 0.0f;  // Sets the density of the body
    FixtureDef.shape = &Shape; // Sets the shape
    Body->CreateFixture(&FixtureDef); // Apply the fixture definition
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //gravity g;

   sf::RenderWindow Window(sf::VideoMode(512,512,32),"Falling Object");
   Window.setFramerateLimit(60);

    b2Vec2 Gravity(0.0f,9.81f);
    b2World World(Gravity);

    // CreateBox(World,0.0,0.0);
     CreateGround(World,256.0f,256.0f);


    sf::Texture BoxTexture;
    BoxTexture.loadFromFile("../QtDogs/assets/shiba_test.png");

    sf::Texture GroundTexture;
    GroundTexture.loadFromFile("../QtDogs/assets/shiba_test.png");


    while(Window.isOpen())
    {
        sf::Event event;
        while(Window.pollEvent(event))
        {
           if(event.type == sf::Event::Closed)Window.close();
        }


        World.Step(1.0f/60.0f,8,3);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            int MouseX = sf::Mouse::getPosition(Window).x;
            int MouseY = sf::Mouse::getPosition(Window).y;
            CreateBox(World, MouseX, MouseY);
        }

        Window.clear(sf::Color::White);

        for (b2Body* BodyIterator = World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
        {
            if (BodyIterator->GetType() == b2_dynamicBody)
            {
                sf::Sprite Sprite;
                Sprite.setTexture(BoxTexture);
                Sprite.setScale(0.25,0.25);
                Sprite.setOrigin(16.f, 16.f);
                Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
                Sprite.setRotation(BodyIterator->GetAngle() * 180/b2_pi);
                Window.draw(Sprite);
            }
            else
            {
                sf::Sprite GroundSprite;
                GroundSprite.setTexture(GroundTexture);
                GroundSprite.setOrigin(400.f, 8.f);
                GroundSprite.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
                GroundSprite.setRotation(180/b2_pi * BodyIterator->GetAngle());
                Window.draw(GroundSprite);

            }
        }
        Window.display();

    }

    return 0;
}
