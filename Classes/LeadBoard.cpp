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
    
    // 全层加载
    leader_global_size = Director::getInstance()->getVisibleSize();
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("buttonicon.plist", "buttonicon.png");
    
    // 背景音乐加载
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("bgmusic.mp3");
    
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
    First_Name = Label::createWithSystemFont("暂无排名数据", "Courier", 25);
    label_First->setPosition(Vec2(leader_global_size.width/2, leader_global_size.height*0.6));
    First_Name->setPosition(Vec2(leader_global_size.width/2, leader_global_size.height*0.6));
    label_First->setAnchorPoint(Vec2(0.5, 0.5));
    First_Name->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(label_First);
    this->addChild(First_Name);
    
    Sprite* label_Second = Sprite::create("labelbg_blue.png");
    Second_Name = Label::createWithSystemFont("暂无排名数据", "Courier", 25);
    label_Second->setPosition(Vec2(leader_global_size.width/2, leader_global_size.height*0.5));
    Second_Name->setPosition(Vec2(leader_global_size.width/2, leader_global_size.height*0.5));
    label_Second->setAnchorPoint(Vec2(0.5, 0.5));
    Second_Name->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(label_Second);
    this->addChild(Second_Name);
    
    Sprite* label_Third = Sprite::create("labelbg_blue.png");
    Third_Name = Label::createWithSystemFont("暂无排名数据", "Courier", 25);
    label_Third->setPosition(Vec2(leader_global_size.width/2, leader_global_size.height*0.4));
    Third_Name->setPosition(Vec2(leader_global_size.width/2, leader_global_size.height*0.4));
    label_Third->setAnchorPoint(Vec2(0.5, 0.5));
    Third_Name->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(label_Third);
    this->addChild(Third_Name);
    
    Sprite* label_Forth = Sprite::create("labelbg_blue.png");
    Forth_Name = Label::createWithSystemFont("暂无排名数据", "Courier", 25);
    label_Forth->setPosition(Vec2(leader_global_size.width/2, leader_global_size.height*0.3));
    Forth_Name->setPosition(Vec2(leader_global_size.width/2, leader_global_size.height*0.3));
    label_Forth->setAnchorPoint(Vec2(0.5, 0.5));
    Forth_Name->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(label_Forth);
    this->addChild(Forth_Name);
    
    Sprite* label_Fifth = Sprite::create("labelbg_blue.png");
    Fifth_Name = Label::createWithSystemFont("暂无排名数据", "Courier", 25);
    label_Fifth->setPosition(Vec2(leader_global_size.width/2, leader_global_size.height*0.2));
    Fifth_Name->setPosition(Vec2(leader_global_size.width/2, leader_global_size.height*0.2));
    label_Fifth->setAnchorPoint(Vec2(0.5, 0.5));
    Fifth_Name->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(label_Fifth);
    this->addChild(Fifth_Name);
    
    // 历史最高分实现
    pointboard = Label::createWithSystemFont("历史最高分: 0", "Courier", 40);
    pointboard->setPosition(leader_global_size.width/2, leader_global_size.height*0.8);
    pointboard->setAnchorPoint(Vec2(0.5,0.5));
    pointboard->setTextColor(Color4B(70, 70, 70, 80));
    this->addChild(pointboard);
    
    // 历史最高分排名
    pointqueue = Label::createWithSystemFont("历史最高排名: 0", "Courier", 40);
    pointqueue->setPosition(leader_global_size.width/2, leader_global_size.height*0.75);
    pointqueue->setAnchorPoint(Vec2(0.5,0.5));
    pointqueue->setTextColor(Color4B(70, 70, 70, 80));
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
    Self_Define::Leader_Data get_leader_instance;
    std::vector< std::map<std::string, std::string> > all_return = get_leader_instance.get_leader_detail(ss.str().c_str());
    if (all_return.size() != 0) {
        // 从本地获取自己的设备id
        int user_device_id = UserDefault::getInstance()->getIntegerForKey("device_random_name", 0);

        
        for (int i = 0; i<all_return.size(); i++) {
            // 补充用户自身数据
            if (all_return[i]["self_id"] != "") {
                if ( atoi(all_return[i]["self_id"].c_str()) == user_device_id ) {
                    // 补充历史最高分
                    std::string scores = "历史最高分: ";
                    scores += all_return[i]["tmp_user_score"];
                    pointboard->setString(scores);
                    
                    // 补充历史最高排名
                    std::string ranks = "历史最高排名: ";
                    ranks += all_return[i]["self_rank"];
                    pointqueue->setString(ranks);
                }
            }
            // 补充排名数据
            else if ( all_return[i]["tmp_user_id"] != "" && atoi(all_return[i]["rank"].c_str()) > 0)
            {
                // 第1名补充
                if (atoi(all_return[i]["rank"].c_str()) == 1) {
                    std::string ranks_data = all_return[i]["tmp_user_id"]+" : "+all_return[i]["tmp_user_score"];
                    First_Name->setString(ranks_data);
                }
                // 第2名补充
                else if (atoi(all_return[i]["rank"].c_str()) == 2) {
                    std::string ranks_data = all_return[i]["tmp_user_id"]+" : "+all_return[i]["tmp_user_score"];
                    Second_Name->setString(ranks_data);
                }
                // 第3名补充
                else if (atoi(all_return[i]["rank"].c_str()) == 3) {
                    std::string ranks_data = all_return[i]["tmp_user_id"]+" : "+all_return[i]["tmp_user_score"];
                    Third_Name->setString(ranks_data);
                }
                // 第4名补充
                else if (atoi(all_return[i]["rank"].c_str()) == 4) {
                    std::string ranks_data = all_return[i]["tmp_user_id"]+" : "+all_return[i]["tmp_user_score"];
                    Forth_Name->setString(ranks_data);
                }
                // 第5名补充
                else if (atoi(all_return[i]["rank"].c_str()) == 5) {
                    std::string ranks_data = all_return[i]["tmp_user_id"]+" : "+all_return[i]["tmp_user_score"];
                    Fifth_Name->setString(ranks_data);
                }
            }
        }
    }
    
    return;
}

/*virtual*/ bool LeaderScene::onTouchBegan(Touch *t, Event *e)
{
    if (p_left != NULL) {
        if (p_left->getBoundingBox().containsPoint(t->getLocation())) {
            this->stopAllActions();
            this->removeAllChildren();
            
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonclick.wav");
            
            Director::getInstance()->replaceScene(MenuScene::createMenuScene());
        
            return true;
        }
        return true;
    }
}