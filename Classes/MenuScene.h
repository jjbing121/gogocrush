//
//  MenuScene.h
//  gogocrush
//
//  Created by Computer on 14/11/19.
//
//

#ifndef __gogocrush__MenuScene__
#define __gogocrush__MenuScene__

#include "cocos2d.h"
#include <SimpleAudioEngine.h>
#include "network/HttpClient.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace network;


class MenuScene : public Layer
{
private:
    Size global_menu_size;      // Menu场景
    Sprite* startbutton_start;  // 开始游戏按键(未点击前)
    Sprite* startbutton_end;    // 开始游戏按键(已点击后)
    Sprite* checkbutton_start;  // 查看排行榜按键(未点击前)
    Sprite* checkbutton_end;    // 查看排行榜按键(已点击后)
public:
    virtual bool init();
    
    static cocos2d::Scene* createMenuScene();
    
    CREATE_FUNC(MenuScene);
    
// 触摸事件检测
public:
    virtual bool onMenuStartClick(Touch* t, Event* e);  // 开始游戏按键点击检测
    virtual bool onCheckStartClick(Touch* t, Event* e); // 查看排行榜按键点击检测
};

#endif /* defined(__gogocrush__MenuScene__) */
