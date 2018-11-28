#ifndef SFMLCANVAS_H
#define SFMLCANVAS_H

#include "qsfmlwidget.h"
#include <iostream>

class SFMLCanvas : public QSFMLWidget
{
public:
    explicit SFMLCanvas(QWidget *Parent = 0);

    void OnInit() override;
    void OnUpdate() override;
    void setSpritePosition(float x, float y);

private:
    sf::Texture myTexture;
    sf::Sprite mySprite;

};

#endif // SFMLCANVAS_H
