#include "spritesheettool.h"
#include <iostream>

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
    sf::IntRect rectSourceSprite = sf::IntRect(x, y, w, h);

    texture.loadFromFile(path, rectSourceSprite);
    std::vector<sf::Texture> frames;

    //Add as many frames as numFrames, changing the initial x coordinate by adding the width.
    for(int i = 0; i < numFrames; i++)
    {
        frames.push_back(texture);
        x += w;
        sf::IntRect rectSourceSprite = sf::IntRect(x, y, w, h);
        //std::cout << "X: " << x << " Y: " << y << std::endl;
        texture.loadFromFile(path, rectSourceSprite);
    }
    //If the given item exists in the dictionary, get the frames vector and append the frames to the vector.
    //Otherwise insert it into our dictionary.
    if(dict.count(name) > 0)
    {
        std::vector<sf::Texture> framesMappedToDictionary = dict[name];
        framesMappedToDictionary.reserve(frames.size() + framesMappedToDictionary.size());
        framesMappedToDictionary.insert(framesMappedToDictionary.end(), frames.begin(), frames.end());
        dict[name] = framesMappedToDictionary;
    }
    else
    {
        dict.insert(name, frames);
    }
}

///
/// \brief SpriteSheetTool::getAnimationFrame
/// \param name
/// \param frameCount
///
void SpriteSheetTool::getAnimationFrame(std::string name, int frameCount){
    //sf::Texture texture = ;
    emit imageSendSignal(dict[name][frameCount]);
}
