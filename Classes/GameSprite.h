//
//  GameSprite.h
//  t04
//
//  Created by 野口一也 on 2014/06/12.
//
//

#ifndef __t04__GameSprite__
#define __t04__GameSprite__

#include "cocos2d.h"

USING_NS_CC;

class GameSprite : public cocos2d::Sprite
{
public:
    GameSprite();
    ~GameSprite();
    
    static GameSprite* gameSpriteWinthFile(const std::string& filename);
    CC_SYNTHESIZE(bool, _userFlag, UserFlag);
private:
    Size _winSize;
};

#endif /* defined(__t04__GameSprite__) */
