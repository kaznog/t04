#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "GameSprite.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    void tryPopMoles(float dt);
    void popMole(GameSprite* mole);
    Animation* animationFromPlist_delay(const std::string& filename, Rect rect, float delay);

    /** Touch listener */
    void enableTouchEvent(bool enabled);
    CC_SYNTHESIZE(EventListenerTouchOneByOne*, _touchListener, TouchListener);
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchMoved(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    virtual void onTouchCancelled(Touch *touch, Event *event);
    
    void setTappable(cocos2d::Ref* pSender);
    void unTappable(cocos2d::Ref* pSender);
private:
    Size _winSize;
    float _scaleFactor;
    std::vector<GameSprite*> _moles;
    Animation* _laughAnim;
    Animation* _hitAnim;
    
    LabelTTF* _label;
    int _score;
    int _totalSpawns;
    bool _gameOver;
};

#endif // __HELLOWORLD_SCENE_H__
