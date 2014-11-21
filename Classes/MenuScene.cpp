//
//  MenuScene.cpp
//  gogocrush
//
//  Created by Computer on 14/11/19.
//
//

#include "MenuScene.h"
#include "HelloWorldScene.h"
#include "selfdefine.h"
#include "LeadBoard.h"

/*virtual*/ bool MenuScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    global_menu_size = Director::getInstance()->getVisibleSize();
    
    // 添加背景
    Sprite* menu_bg = Sprite::create("login_iphone5.png");
    menu_bg->setPosition(Vec2(global_menu_size.width/2, global_menu_size.height/2));
    menu_bg->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(menu_bg);
    
    // 所有背景音乐预加载
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("bgmusic.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("buttonclick.wav");
    // 添加背景音乐
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("bgmusic.mp3");
    
    // 添加唯一设备识别信息
    int device_random_name = UserDefault::getInstance()->getIntegerForKey("device_random_name", 0);
    if (device_random_name == 0) {
        UserDefault::getInstance()->setIntegerForKey("device_random_name", arc4random() % 1000000000);
    }
    
#pragma mark - 开始游戏
    // 添加开始游戏按键
    startbutton_start = Sprite::create("start.png");    // 点击前
    startbutton_start->setPosition(global_menu_size.width * 0.5, global_menu_size.height * 0.4);
    startbutton_start->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(startbutton_start, 1);
    
    startbutton_end = Sprite::create("start_down.png"); // 点击后
    startbutton_end->setPosition(global_menu_size.width * 0.5, global_menu_size.height * 0.4);
    startbutton_end->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(startbutton_end,0);
    
    // 添加查看排行榜按键
    checkbutton_start = Sprite::create("check.png");    // 点击前
    checkbutton_start->setPosition(global_menu_size.width * 0.5, global_menu_size.height * 0.3);
    checkbutton_start->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(checkbutton_start, 10);
    checkbutton_end = Sprite::create("check_down.png"); // 点击后
    checkbutton_end->setPosition(global_menu_size.width * 0.5, global_menu_size.height * 0.3);
    checkbutton_end->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(checkbutton_end,9);
    
#pragma mark - 触摸事件检测
    // 开始游戏按键点击
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan=CC_CALLBACK_2(MenuScene::onMenuStartClick,this);
    // 查看排行榜按键点击
    auto clistener = EventListenerTouchOneByOne::create();
    clistener->setSwallowTouches(true);
    clistener->onTouchBegan=CC_CALLBACK_2(MenuScene::onCheckStartClick,this);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, startbutton_start);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(clistener, checkbutton_start);
    return true;
}

cocos2d::Scene* MenuScene::createMenuScene()
{
    auto scene = Scene::create();
    
    auto layer = MenuScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

/*virtual*/ bool MenuScene::onMenuStartClick(Touch* t, Event* e)
{
    if (startbutton_start->getBoundingBox().containsPoint(t->getLocation())) {
        Show* show_action = Show::create();
        Hide* hide_action = Hide::create();
        startbutton_start->runAction(hide_action);
        startbutton_end->runAction(show_action);

        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonclick.wav");

        Director::getInstance()->replaceScene(TransitionFadeBL::create(1, HelloWorld::createScene()));
        return true;
    }
    return false;
}

/*virtual*/ bool MenuScene::onCheckStartClick(Touch* t, Event* e)
{
    if (checkbutton_start->getBoundingBox().containsPoint(t->getLocation())) {
        Show* show_action = Show::create();
        Hide* hide_action = Hide::create();
        checkbutton_start->runAction(hide_action);
        checkbutton_end->runAction(show_action);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonclick.wav");

        Director::getInstance()->replaceScene(LeaderScene::createLeaderScene());
        
        return true;
    }
    return false;
}

