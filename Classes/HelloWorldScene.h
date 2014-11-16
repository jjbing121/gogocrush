#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "gogo_header.h"

using namespace cocos2d;

class Element;

class HelloWorld : public cocos2d::Layer
{
private:
    // 外部引用，内部解析
    int _element_row;
    int _element_row_space;
    int _element_column;
    int _element_column_space;
    
private:
    int source_weight;        // 原点宽像素位
    int source_height;        // 原点长像素位
    Element** vector_element; // 精灵保存的2维数组

    bool _isTouched;          // 当前被点击的状态 : 默认初始化为false
    bool _isMoved;            // 当前寿司是否被移动过
    Element* touch_element;       // 当前被点击到的精灵 : 默认初始化为NULL
    Element* touch_after_element; // 当前被移动的精灵 : 默认初始化为NULL
public:
    HelloWorld();
    virtual~ HelloWorld();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    // 初始化2维矩阵数据
    virtual void init_vec2_element(void);                                     // 循环入口
    virtual void create_each_doaction(int row_number, int col_number);        // 进行初始化操作
    virtual Vec2 get_element_location(int row_number, int col_number);        // 根据行号和列号找到定位点
    
// 全局触发动作
public:
    virtual bool global_touch_on(Touch* t, Event* e);
    virtual void global_touch_move(Touch* t, Event* e);
    virtual void global_touch_end(Touch* t, Event* e);
    virtual Element* user_click_sushi(Vec2*);           // 检测点选寿司存在
    virtual void swap_sushi(Element*, Element*);        // 交换寿司
};

#endif // __HELLOWORLD_SCENE_H__
