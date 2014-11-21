//
//  LeadBoard.h
//  gogocrush
//
//  Created by Computer on 14/11/20.
//
//

#ifndef __gogocrush__LeadBoard__
#define __gogocrush__LeadBoard__

#include "cocos2d.h"
#include "network/HttpClient.h"
#include <cocos-ext.h>
#include <SimpleAudioEngine.h>

using namespace cocos2d;
using namespace CocosDenshion;
using namespace network;
using namespace extension;

class LeaderScene : public Layer
{
private:
    Size leader_global_size;
    Sprite* p_left;             // 返回首页按键
    
    Label* pointboard;          // 自身最高分
    Label* pointqueue;          // 自身最高排名
    
    Label* First_Name;        // 第一名
    Label* Second_Name;       // 第二名
    Label* Third_Name;        // 第三名
    Label* Forth_Name;        // 第四名
    Label* Fifth_Name;        // 第五名
public:
    static cocos2d::Scene* createLeaderScene();
    
    virtual bool init();
    
    CREATE_FUNC(LeaderScene);
    
public:
    virtual void onLeaderResponse(HttpClient* client, HttpResponse* response);

// 设置返回主页触发内容
public:
    virtual bool onTouchBegan(Touch *t, Event *e);
};

#endif /* defined(__gogocrush__LeadBoard__) */
