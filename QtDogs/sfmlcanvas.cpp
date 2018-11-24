#include "sfmlcanvas.h"

SFMLCanvas::SFMLCanvas(QWidget* Parent) : QSFMLWidget(Parent)
{

}

void SFMLCanvas::OnInit()
{
    if(!myTexture.loadFromFile("assets/shiba_test.png"))
    {
        std::cout << "Failed to load image" << std::endl;
    }
    mySprite.setTexture(myTexture);
    mySprite.setPosition(sf::Vector2f(300, 200));

}

void SFMLCanvas::OnUpdate()
{
    RenderWindow::draw(mySprite);
}
