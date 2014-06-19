#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto director = Director::getInstance();
    director->getOpenGLView()->setDesignResolutionSize(480, 320, ResolutionPolicy::SHOW_ALL);
    _scaleFactor = 480.0f / 1024.0f;
    _winSize = director->getWinSize();
    Size visibleSize = director->getVisibleSize();
    Vec2 origin = director->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto spriteCache = SpriteFrameCache::getInstance();
    spriteCache->addSpriteFramesWithFile("mole_default.plist");

    auto cache = AnimationCache::getInstance();
    cache->addAnimationsWithFile("moleAnim.plist");

    auto dirt = Sprite::create("bg_dirt.png");
    dirt->setScale(480/ dirt->getContentSize().width);
    dirt->setPosition(Vec2(_winSize.width/2, _winSize.height/2));
    this->addChild(dirt, -2);
    
    auto lower = Sprite::create("grass_lower.png");
    lower->setScale(_scaleFactor);
    lower->setAnchorPoint(Vec2(0.5, 1));
    lower->setPosition(Vec2(_winSize.width/2, _winSize.height/2));
    lower->getTexture()->setAliasTexParameters();
    this->addChild(lower, 1);
    
    auto upper = Sprite::create("grass_upper.png");
    upper->setScale(_scaleFactor);
    upper->setAnchorPoint(Vec2(0.5, 0));
    upper->setPosition(Vec2(_winSize.width/2, _winSize.height/2));
    upper->getTexture()->setAliasTexParameters();
    this->addChild(upper, -1);
    
    GameSprite* mole2 = GameSprite::gameSpriteWinthFile("mole_1.png");
    mole2->setScale(_scaleFactor);
    mole2->setPosition(Vec2(512 * _scaleFactor, _winSize.height/2 - mole2->getContentSize().height/3));
    this->addChild(mole2);
    
    GameSprite* mole1 = GameSprite::gameSpriteWinthFile("mole_1.png");
    mole1->setScale(_scaleFactor);
    mole1->setPosition(Vec2(200 * _scaleFactor, _winSize.height/2 - mole2->getContentSize().height/3));
    this->addChild(mole1);

    GameSprite* mole3 = GameSprite::gameSpriteWinthFile("mole_1.png");
    mole3->setScale(_scaleFactor);
    mole3->setPosition(Vec2(820 * _scaleFactor, _winSize.height/2 - mole2->getContentSize().height/3));
    this->addChild(mole3);
    _moles.push_back(mole1);
    _moles.push_back(mole2);
    _moles.push_back(mole3);
    
    float _margin = 10.0f;
    _label = LabelTTF::create("score: 0", "Verdana", 48);
    _label->setAnchorPoint(Vec2(1, 0));
    _label->setPosition(Vec2(_winSize.width - _margin, _margin));
    this->addChild(_label, 10);
    
    this->enableTouchEvent(true);
    this->schedule(schedule_selector(HelloWorld::tryPopMoles), 0.5f);
    return true;
}

void HelloWorld::setTappable(cocos2d::Ref *pSender)
{
    auto mole = dynamic_cast<GameSprite*>(pSender);
    mole->setUserFlag(true);
}

void HelloWorld::unTappable(cocos2d::Ref *pSender)
{
    auto mole = dynamic_cast<GameSprite*>(pSender);
    mole->setUserFlag(false);
}

void HelloWorld::tryPopMoles(float dt)
{
    if (_gameOver) {
        return;
    }
    _label->setString("score: " + std::to_string(_score));
    if (_totalSpawns >= 50) {
        auto golabel = LabelTTF::create("Level Complete", "Verdana", 48);
        golabel->setPosition(Vec2(_winSize.width/2, _winSize.height/2));
        golabel->setScale(0.1f);
        this->addChild(golabel, 10);
        golabel->runAction(ScaleTo::create(0.5f, 1.0f));
        _gameOver = true;
    }
    for (std::vector<GameSprite*>::iterator it = _moles.begin(); it != _moles.end(); it++) {
        auto mole = dynamic_cast<GameSprite*>(*it);
        if (arc4random() % 3 == 0) {
            if (mole->getNumberOfRunningActions() == 0) {
                this->popMole(mole);
            }
        }
    }
}

void HelloWorld::popMole(GameSprite *mole)
{
    if (_totalSpawns > 50) {
        return;
    }
    _totalSpawns++;
    
    mole->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mole_1.png"));
    auto moveUp = MoveBy::create(0.2f, Vec2(0, _winSize.height * 0.25f));
    auto setTappable = CallFuncN::create(CC_CALLBACK_1(HelloWorld::setTappable, this));
    
    auto easeMoveUp = EaseInOut::create(moveUp, 3.0f);
    auto laughN = AnimationCache::getInstance()->getAnimation("laugh");
    laughN->setDelayPerUnit(0.1);
    auto laugh = Animate::create(laughN);

    auto unTappable = CallFuncN::create(CC_CALLBACK_1(HelloWorld::unTappable, this));
    
    auto delay = DelayTime::create(0.5f);
    auto easeMoveDown = easeMoveUp->reverse();
//    mole->runAction(Sequence::create(easeMoveUp, laugh, delay, easeMoveDown, NULL));
    mole->runAction(Sequence::create(easeMoveUp, setTappable, laugh, delay, unTappable, easeMoveDown, NULL));
}

Animation* HelloWorld::animationFromPlist_delay(const std::string &filename, Rect rect, float delay)
{
    auto array = cocos2d::Array::createWithContentsOfFile(filename);
    auto frames = new Vector<SpriteFrame*>();
    for (int i = 0; i < array->count(); i++) {
//        std::string spritename = dynamic_cast<std::string>(array->data->arr[i]);
//        auto frame = SpriteFrame::create(array->getObjectAtIndex(i), rect);
//        SpriteFrameCache::getInstance()->addSpriteFrame(frame, *spritename);
//        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName((const std::string &)spritename);
//        frames->pushBack(frame);
    }
    auto animation = Animation::createWithSpriteFrames(*frames, delay);
    return animation;
}

#pragma mark -
#pragma mark touch events
void HelloWorld::enableTouchEvent(bool enabled)
{
    if (this->_touchListener != nullptr) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(this->_touchListener);
        this->_touchListener = nullptr;
    }
    if (enabled) {
        this->_touchListener = EventListenerTouchOneByOne::create();
        _touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
        _touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
        _touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
        _touchListener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);
    }
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *event)
{
    auto location = this->convertToNodeSpace(touch->getLocation());
    for (std::vector<GameSprite*>::iterator it = _moles.begin(); it != _moles.end(); it++) {
        auto mole = dynamic_cast<GameSprite*>(*it);
        if ( ! mole->getUserFlag()) {
            continue;
        }
        if (mole->getBoundingBox().containsPoint(location)) {
            mole->setUserFlag(true);
            _score += 10;
            mole->stopAllActions();
            auto hitN = AnimationCache::getInstance()->getAnimation("hit");
            hitN->setDelayPerUnit(0.2f);
            auto hit = Animate::create(hitN);
            auto moveDown = MoveTo::create(0.2f, Vec2(mole->getPositionX(), _winSize.height/2 - mole->getContentSize().height/3));
            auto easeMoveDown = EaseInOut::create(moveDown, 3.0f);
            mole->runAction(Sequence::create(hit, easeMoveDown, NULL));
        }
    }
    return true;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *event)
{
}

void HelloWorld::onTouchEnded(Touch *touch, Event *event)
{
    auto location = touch->getLocationInView();
    location = Director::getInstance()->convertToGL(location);
    
}

void HelloWorld::onTouchCancelled(Touch *touch, Event *event)
{
}

#pragma mark -

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
