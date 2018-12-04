#include "sfmlcanvas.h"

SFMLCanvas::SFMLCanvas()
{
    if(!myTexture.loadFromFile("../QtDogs/assets/shiba_test.png"))
    {
        std::cout << "Failed to load image" << std::endl;
    }
    mySprite.setTexture(myTexture);
    mySprite.setPosition(sf::Vector2f(300, 200));
}

void SFMLCanvas::setSpritePosition(float x, float y)
{
    mySprite.setPosition(sf::Vector2f(x,y));
}

