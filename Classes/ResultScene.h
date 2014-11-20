//
//  ResultScene.h
//  PlaneDemo
//
//  Created by Computer on 14/11/9.
//
//

#ifndef __PlaneDemo__ResultScene__
#define __PlaneDemo__ResultScene__

#include "cocos2d.h"
#include <SimpleAudioEngine.h>

using namespace cocos2d;
using namespace CocosDenshion;

class ResultScene : public cocos2d::Layer
{
private:
    std::string score_pass;
    Label* score_result;
    
    Sprite* p_return;
    Sprite* p_left;
    Sprite* p_next;
    
public:
    static cocos2d::Scene* createScene(std::string);
    
    virtual bool init();
    
    static ResultScene* create(std::string);
    
public:
    virtual bool RestartBegan(Touch *t, Event *e);
    
    virtual bool ReturnMenu(Touch *t, Event *e);
    
    virtual bool EndGame(Touch *t, Event *e);
};

#endif /* defined(__PlaneDemo__ResultScene__) */
