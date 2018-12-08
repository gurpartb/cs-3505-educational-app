/**
  CS 3505 - A8 Final Project - QT Dogs
  Educational application to teach youth the importance of pet responsibility.
  Designed by:
  Brendan Johnston, Andrew Dron, Caleb Edwards, Colton Lee, Gurpartap Bhatti, Jacob Haydel, Tyler Trombley, Jared Hansen
*/

#include "spritesheettool.h"
#include <iostream>

///
/// \brief SpriteSheetTool::SpriteSheetTool
/// SpriteSheet tool constructor.
SpriteSheetTool::SpriteSheetTool()
{

}

///
/// \brief SpriteSheetTool::addAnimation
/// Generic method to add animations to the spritesheet.
///
void SpriteSheetTool::addAnimation(int x, int y, int w, int h, int numFrames, std::string name, sf::Texture& img)
{

    std::vector<sf::IntRect> frames;

    //Add as many frames as numFrames, changing the initial x coordinate by adding the width.
    for(int i = 0; i < numFrames; ++i)
    {
        sf::IntRect rectSourceSprite = sf::IntRect(x, y, w, h);
        frames.push_back(rectSourceSprite);
        x += w;
    }

    //If the given item exists in the dictionary, get the frames vector and append the frames to the vector.
    //Otherwise insert it into our dictionary.
    if(dict.contains(name))
    {
        std::vector<sf::IntRect>* framesMappedToDictionary = &dict[name];
        for(auto& frame : frames)
            framesMappedToDictionary->push_back(frame);
    }
    else
    {
        dict.insert(name, frames);
        sheetDict.insert(name,&img);
    }
}
