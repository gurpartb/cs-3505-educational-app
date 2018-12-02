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
void SpriteSheetTool::addAnimation(int x, int y, int w, int h, int numFrames, std::string name, std::string path)
{
    //Load up the textures using the rectangle coordinates
    sf::Texture texture;
    sf::IntRect rectSourceSprite(x, y, w, h);
    texture.loadFromFile(path, rectSourceSprite);
    std::vector<sf::Texture> frames;
    //Add as many frames as numFrames, changing the initial x coordinate by adding the width.
    for(int i = 0; i < numFrames; i++)
    {
        frames.push_back(texture);
        x += w;
        texture.loadFromFile(path, rectSourceSprite);
    }
    dict.insert(name, frames);
}

///
/// \brief SpriteSheetTool::getAnimationFrame
/// \param name
/// \param frameCount
///
void SpriteSheetTool::getAnimationFrame(std::string name, int frameCount){
    std::vector<sf::Texture> frames = dict[name];
    QImage imageTextureCopy(frames[frameCount].copyToImage().getPixelsPtr(), frames[frameCount].getSize().x, frames[0].getSize().y, QImage::Format_ARGB32);
    emit imageSendSignal(imageTextureCopy);
}
