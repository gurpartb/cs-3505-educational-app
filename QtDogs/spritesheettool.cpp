#include "spritesheettool.h"

SpriteSheetTool::SpriteSheetTool()
{

}

///
/// \brief SpriteSheetTool::addAnimation
/// \param x
/// \param y
/// \param w
/// \param h
/// \param numFrames
/// \param name
/// \param path
///
///
void SpriteSheetTool::addAnimation(int x, int y, int w, int h, int numFrames, std::string name, QString path)
{
    QImage image(path);
    std::vector<QImage> frames;
    for(int i = 0; i < numFrames; i++)
    {
        QImage newImage = image.copy(x,y,w,h);
        frames.push_back(newImage);
        x += w;
    }
    dict.insert(name, frames);
}
