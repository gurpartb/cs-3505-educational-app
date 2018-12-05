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
    QMap<std::string, std::vector<sf::Texture>> dict;
    SpriteSheetTool();
    void addAnimation(int x, int y, int w, int h, int numFrames, std::string name, std::string path);

private slots:
    void getAnimationFrame(std::string name, int frameCount);

signals:
    void imageSendSignal(sf::Texture);
};

#endif // SPRITESHEETTOOL_H
