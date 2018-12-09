/**
  CS 3505 - A8 Final Project - QT Dogs
  Educational application to teach youth the importance of pet responsibility.
  Designed by:
  Brendan Johnston, Andrew Dron, Caleb Edwards, Colton Lee, Gurpartap Bhatti, Jacob Haydel, Tyler Trombley, Jared Hansen
*/

#ifndef SPRITESHEETTOOL_H
#define SPRITESHEETTOOL_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <string>
#include <QMap>
#include <vector>
#include <SFML/Graphics.hpp>

///
/// \brief The SpriteSheetTool class
/// Sprite Sheet Tool designed to be able to use sprite sheet references to open any type of sprite sheet for a game.
///
class SpriteSheetTool : public QLabel
{
    Q_OBJECT



public:
    SpriteSheetTool();
    void addAnimation(int x, int y, int w, int h, int numFrames, std::string name, sf::Texture& img);
    int getAnimationFrameCount(std::string name){return static_cast<int>(dict[name].size());}
    sf::IntRect* getAnimationFrame(std::string name, int frameCount){return &dict[name][static_cast<unsigned int>(frameCount)];}
    sf::Texture* getTexture(std::string name){return sheetDict[name];}

private:
    QMap<std::string, sf::Texture*> sheetDict;
    QMap<std::string, std::vector<sf::IntRect>> dict;
};

#endif // SPRITESHEETTOOL_H
