#ifndef SFMLCANVAS_H
#define SFMLCANVAS_H

#include "qsfmlwidget.h"
#include <iostream>

class SFMLCanvas
{
public:
    explicit SFMLCanvas();

    void setSpritePosition(float x, float y);

private:
    sf::Texture myTexture;
    sf::Sprite mySprite;

};

#endif // SFMLCANVAS_H
