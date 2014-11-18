//
//  ElementSushi.h
//  gogocrush
//
//  Created by Computer on 14/11/14.
//
//

#ifndef __gogocrush__ElementSushi__
#define __gogocrush__ElementSushi__
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "cocos2d.h"
#include "gogo_header.h"

using namespace cocos2d;

class Element : public cocos2d::Sprite
{
public:
    Element() : e_weight(0), e_height(0), e_index(0), e_row(0), e_column(0), e_pic(NULL), e_special(0) {};
    static Element* create(int row, int column, int normal_pic=0);                      // 普通图标创建
    static Element* createSpecial(int row, int column, bool direct, int index_number);   // 合成后图标创建 => [bool] false/true : 水平/垂直
    
    // 基础属性（gl坐标 or 世界坐标）
    CC_SYNTHESIZE(float, e_weight, WEIGHT);
    CC_SYNTHESIZE(float, e_height, HEIGHT);
    // 基础位置信息
    CC_SYNTHESIZE(int, e_index, INDEX);
    CC_SYNTHESIZE(int, e_row, ROW);
    CC_SYNTHESIZE(int, e_column, COLUMN);
    CC_SYNTHESIZE(char32_t, e_pic, PICNAME);
    CC_SYNTHESIZE(int, e_special, SPETYPE);    // 第4个特殊点的方向纪录, 0表示初始化寿司, 1为特殊水平寿司, 2为特殊垂直寿司
    
    // 定义基础的长度和宽度，所有图片均一致性
    static float getContentWidth();
    static float getContentHigth();
};
#endif /* defined(__gogocrush__ElementSushi__) */
