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
    int source_weight;  // 原点宽像素位
    int source_height;  // 原点长像素位
    Element** vector_element;
    
public:
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
};

#endif // __HELLOWORLD_SCENE_H__
