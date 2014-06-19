//
//  GameSprite.cpp
//  t04
//
//  Created by 野口一也 on 2014/06/12.
//
//

#include "GameSprite.h"

GameSprite::GameSprite()
{
    
}

GameSprite::~GameSprite()
{
    
}

GameSprite* GameSprite::gameSpriteWinthFile(const std::string &filename)
{
    GameSprite* sprite = new GameSprite();
//    if (sprite && sprite->initWithFile(filename)) {
    if (sprite && sprite->initWithSpriteFrameName(filename)) {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}