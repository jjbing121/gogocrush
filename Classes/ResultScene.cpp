//
//  ResultScene.cpp
//  PlaneDemo
//
//  Created by Computer on 14/11/9.
//
//

#include "ResultScene.h"
#include "HelloWorldScene.h"
#include "MenuScene.h"
#include "LeadBoard.h"
#include "selfdefine.h"

cocos2d::Scene* ResultScene::createScene(std::string getScore)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ResultScene::create(getScore);

    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

/*virtual*/ bool ResultScene::init()
{
    if ( !Layer::init() ) {
        return false;
    }
    
    Size result_global_size = Director::getInstance()->getWinSize();
    
    // 重新设置记分板的位置
    Sprite* a = Sprite::create("background_iphone5.png");
    a->setPosition(result_global_size.width/2, result_global_size.height/2);
    a->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(a);
    
    // 重设记分板内容
    score_result = Label::createWithSystemFont(score_pass, "Courier", 70);
    score_result->setPosition(result_global_size.width/2, result_global_size.height/2);
    score_result->setAnchorPoint(Vec2(0.5, 0.5));
    int scores = atoi(score_result->getString().c_str());
    char get_board[25];
    memset(get_board, 0, 25);
    sprintf(get_board, "您的总分：%d", scores);
    score_result->setString(get_board);
    score_result->setTextColor(Color4B(0, 0, 100, 150));
    this->addChild(score_result);
    
    // 设置3个标记点的位置
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("buttonicon.plist", "buttonicon.png");
    
    p_return = Sprite::createWithSpriteFrameName("return.png");
    p_return->setPosition(result_global_size.width*0.2, result_global_size.height*0.4);
    p_return->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(p_return);
    
    p_left = Sprite::createWithSpriteFrameName("left.png");
    p_left->setPosition(result_global_size.width*0.5, result_global_size.height*0.4);
    p_left->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(p_left);
    
    p_next = Sprite::createWithSpriteFrameName("speed.png");
    p_next->setPosition(result_global_size.width*0.8, result_global_size.height*0.4);
    p_next->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(p_next);
    
    // 增加对于3个标记点的操作信息
    // 跳转场景至所有对应场景前需要注意如下几点
    // A. 释放本场景的所有动作效果
    // B. 释放本场景的child资源
    
    // 1. 新增重新开始
    auto restart_listener = EventListenerTouchOneByOne::create();
    restart_listener->setSwallowTouches(false);
    restart_listener->onTouchBegan = CC_CALLBACK_2(ResultScene::RestartBegan, this);
    
    // 2. 新增返回首页
    auto menu_listener = EventListenerTouchOneByOne::create();
    menu_listener->setSwallowTouches(false);
    menu_listener->onTouchBegan = CC_CALLBACK_2(ResultScene::ReturnMenu, this);
    
    // 3. 新增结束按键z
    auto end_listener = EventListenerTouchOneByOne::create();
    end_listener->setSwallowTouches(false);
    end_listener->onTouchBegan = CC_CALLBACK_2(ResultScene::EndGame, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(restart_listener, p_return);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(menu_listener, p_left);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(end_listener, p_next);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("bgmusic.mp3");
    
    return true;
}

ResultScene* ResultScene::create(std::string getScore)
{
    ResultScene* pRet = new ResultScene();
    pRet->score_pass = getScore;
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

/*virtual*/ bool ResultScene::RestartBegan(Touch *t, Event *e)
{
    
    if (p_return != NULL) {
        if (p_return->getBoundingBox().containsPoint(t->getLocation())) {

            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonclick.wav");
            
            this->stopAllActions();
            this->removeAllChildren();

            Director::getInstance()->replaceScene(HelloWorld::createScene());
        }
        return true;
    }
    return true;
}


/*virtual*/ bool ResultScene::ReturnMenu(Touch *t, Event *e)
{
    if (p_left != NULL) {
        if (p_left->getBoundingBox().containsPoint(t->getLocation())) {
            
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonclick.wav");
            
            this->stopAllActions();
            this->removeAllChildren();
            
            Director::getInstance()->replaceScene(MenuScene::createMenuScene());
        }
        return true;
    }
    return true;
}

/*virtual*/ bool ResultScene::EndGame(Touch *t, Event *e)
{   
    if (p_next != NULL) {
        if (p_next->getBoundingBox().containsPoint(t->getLocation())) {

            // 跳转前异步提交一次积分上传
            // 合并内容
            Self_Define::Leader_Data getdata;
            HttpRequest* request = new HttpRequest();
            int user_device_id = UserDefault::getInstance()->getIntegerForKey("device_random_name", 0);
            char user_device_string[20];
            memset(user_device_string, 0, 20);
            sprintf(user_device_string, "%d", user_device_id);
            
            // 排行榜事件实现
            std::string send_url = "http://118.194.49.4:51999/UploadCommitIndex";
            std::string postData = getdata.get_data("1390013900", atoi(score_pass.c_str()), user_device_string, "UploadCommitIndex");
            
            request->setUrl(send_url.c_str());
            request->setRequestType(HttpRequest::Type::POST);
            request->setRequestData(postData.c_str(),strlen(postData.c_str()));
            request->setTag("PostUserUploadScore");
            
            // 发送方式
            request->setResponseCallback(this, httpresponse_selector(ResultScene::onCheckResponse));
            HttpClient::getInstance()->send(request);
            request->release();
            
            // 场景清理工作
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonclick.wav");
            this->stopAllActions();
            this->removeAllChildren();
            
            // 跳转到排行榜界面
            Director::getInstance()->replaceScene(LeaderScene::createLeaderScene());
        }
        return true;
    }
    return true;
}

/*virtual*/ void ResultScene::onCheckResponse(HttpClient* client, HttpResponse* response)
{
    if(!response || !response->isSucceed())
        return;
    
    // 获取响应中的内容
    std::vector<char>* buffer = response->getResponseData();
    std::stringstream ss;
    for(int i=0;i<buffer->size();i++) {
        ss << (*buffer)[i];
    }
    // 做相应处理
    Self_Define::Leader_Data getdata;
    int incode = getdata.get_incode(ss.str().c_str());
    log("incode => %d", incode);
    
    return;
}