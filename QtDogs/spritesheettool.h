#ifndef SPRITESHEETTOOL_H
#define SPRITESHEETTOOL_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <string>
#include <QMap>
#include <vector>
#include <SFML/Graphics.hpp>

class SpriteSheetTool : public QLabel
{
    Q_OBJECT
public:
    SpriteSheetTool();
    void addAnimation(int x, int y, int w, int h, int numFrames, std::string name, std::string path);
    void addAnimation(int x, int y, int w, int h, int numFrames, std::string name, sf::Image& img);
    int getAnimationFrameCount(std::string name){return dict[name].size()-1;}
    sf::Texture* getAnimationFrame(std::string name, int frameCount){return &dict[name][frameCount];}

private:
    QMap<std::string, std::vector<sf::Texture>> dict;
};

#endif // SPRITESHEETTOOL_H
