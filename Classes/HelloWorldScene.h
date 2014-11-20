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
    Sprite* bg;               // 层背景图
    int source_weight;        // 原点宽像素位
    int source_height;        // 原点长像素位
    Element** vector_element; // 精灵保存的2维数组

    bool _isTouched;          // 当前被点击的状态 : 默认初始化为true
    bool _isMoved;            // 当前寿司是否被移动过
    bool _isAction;           // 当前是否存在有单一寿司在执行动作判断阀值
    bool _isSpecialAction;    // 当前是否在执行多个元素合并
    Element* touch_element;       // 当前被点击到的精灵 : 默认初始化为NULL
    Element* touch_after_element; // 当前被移动的精灵 : 默认初始化为NULL
    
    ProgressTimer* ptimer;    // 计时器封装
    Label* countdown;         // 计时器倒计时封装
    Label* pointboard;        // 分数板
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
    
// 全局定时器及逻辑部分
public:
    virtual void update(float dtime) override;
    virtual void check_global_sushi();          // 检查全局是否有需要消除的状态
    virtual void filling_sushi();               // 检查全局需要补充的位置
    virtual void col_check_sushi(Element*, std::vector<Element*>&);     // 向右检查
    virtual void row_check_sushi(Element*, std::vector<Element*>&);     // 向上检查
    virtual void remove_sushi(Node*);                                   // 从父节点中移除这个子节点
    virtual Element* filling_up(int row_number, int col_number);        // 向上查找最近的一个元素位置
    virtual void makeup_sushi(Element*, int miss_row, int miss_col);    // 删除合并机制
    virtual void update_new_sushi();                                    // 全局检测，并且安排新寿司进入
    
    virtual void process_sixty_seconds(float dtime);    // 全局加载定时器
    virtual void setCountDown(float dtime);             // 全局加载计数器
    virtual void setPointBoard(int point);              // 全局加载积分板分数增加
    
// 全局消除效果
public:
    virtual void CleanByLevel(Vec2 point);                                    // 水平合并消除效果(消除整行)
    virtual void CleanByVeritial(Vec2 point);                                 // 垂直合并消除效果(消除整列)
};

#endif // __HELLOWORLD_SCENE_H__
