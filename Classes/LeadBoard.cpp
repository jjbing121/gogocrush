//
//  LeadBoard.cpp
//  gogocrush
//
//  Created by Computer on 14/11/20.
//
//

#include "LeadBoard.h"
#include "selfdefine.h"
#include "MenuScene.h"

cocos2d::Scene* LeaderScene::createLeaderScene()
{
    auto scene = Scene::create();
    
    auto layer = LeaderScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

/*virtual*/ bool LeaderScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    leader_global_size = Director::getInstance()->getVisibleSize();
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("buttonicon.plist", "buttonicon.png");
    
    // 设置背景
    Sprite* bg = Sprite::create("background_iphone5.png");
    bg->setPosition(Vec2(leader_global_size.width/2, leader_global_size.height/2));
    bg->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(bg);
    
    // 从服务端获取到最新排名
    Self_Define::Leader_Data get_leader_instance;
    
    HttpRequest* request = new HttpRequest();
    std::string send_url = "http://118.194.49.4:51999/UploadCommitSearch";
    std::string postData = get_leader_instance.get_leader_all();
    log("post data => %s", postData.c_str());
    
    request->setUrl(send_url.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    request->setRequestData(postData.c_str(),strlen(postData.c_str()));
    request->setTag("PostUserGetScoreList");
    
    // 发送方式
    request->setResponseCallback(this, httpresponse_selector(LeaderScene::onLeaderResponse));
    HttpClient::getInstance()->send(request);
    request->release();

    // 列表实现
    Sprite* label_First = Sprite::create("labelbg_blue.png");
    label_First->setPosition(Vec2(leader_global_size.width/2, leader_global_size.height*0.8));
    label_First->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(label_First);
    
    Sprite* label_Second = Sprite::create("labelbg_blue.png");
    label_Second->setPosition(Vec2(leader_global_size.width/2, leader_global_size.height*0.7));
    label_Second->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(label_Second);
    
    Sprite* label_Third = Sprite::create("labelbg_blue.png");
    label_Third->setPosition(Vec2(leader_global_size.width/2, leader_global_size.height*0.6));
    label_Third->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(label_Third);
    
    Sprite* label_Forth = Sprite::create("labelbg_blue.png");
    label_Forth->setPosition(Vec2(leader_global_size.width/2, leader_global_size.height*0.5));
    label_Forth->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(label_Forth);
    
    Sprite* label_Fifth = Sprite::create("labelbg_blue.png");
    label_Fifth->setPosition(Vec2(leader_global_size.width/2, leader_global_size.height*0.4));
    label_Fifth->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(label_Fifth);
    
    // 历史最高分实现
    Label* pointboard = Label::createWithSystemFont("历史最高分: 0", "Courier", 40);
    pointboard->setPosition(leader_global_size.width/2, leader_global_size.height*0.25);
    pointboard->setAnchorPoint(Vec2(0.5,0.5));
    pointboard->setTextColor(Color4B(255, 255, 255, 255));
    this->addChild(pointboard);
    
    // 历史最高分排名
    Label* pointqueue = Label::createWithSystemFont("历史最高排名: 0", "Courier", 40);
    pointqueue->setPosition(leader_global_size.width/2, leader_global_size.height*0.2);
    pointqueue->setAnchorPoint(Vec2(0.5,0.5));
    pointqueue->setTextColor(Color4B(255, 255, 255, 255));
    this->addChild(pointqueue);
    
    // 返回首页按键
    p_left = Sprite::createWithSpriteFrameName("left.png");
    p_left->setPosition(80, leader_global_size.height-80);
    p_left->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(p_left);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LeaderScene::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, p_left);
    
    return true;
}

/*virtual*/ void LeaderScene::onLeaderResponse(HttpClient* client, HttpResponse* response)
{
    // 接受回调，并且填充list上的内容
    if(!response || !response->isSucceed())
        return;
    
    // 获取响应中的内容
    std::vector<char>* buffer = response->getResponseData();
    std::stringstream ss;
    for(int i=0;i<buffer->size();i++) {
        ss << (*buffer)[i];
    }
    // 做相应处理
    log("output => %s", ss.str().c_str());

}

/*virtual*/ bool LeaderScene::onTouchBegan(Touch *t, Event *e)
{
    if (p_left != NULL) {
        if (p_left->getBoundingBox().containsPoint(t->getLocation())) {
            this->stopAllActions();
            this->removeAllChildren();
            Director::getInstance()->replaceScene(MenuScene::createMenuScene());
        }
    }
}