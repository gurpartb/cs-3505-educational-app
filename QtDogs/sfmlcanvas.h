#ifndef SFMLCANVAS_H
#define SFMLCANVAS_H

#include "qsfmlwidget.h"
#include <iostream>

class SFMLCanvas : QSFMLWidget
{
public:
    explicit SFMLCanvas(QWidget *Parent = 0);

    void OnInit() override;
    void OnUpdate() override;

private:
    sf::Texture myTexture;
    sf::Sprite mySprite;


};

#endif // SFMLCANVAS_H
