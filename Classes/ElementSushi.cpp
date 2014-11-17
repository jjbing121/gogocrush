//
//  ElementSushi.cpp
//  gogocrush
//
//  Created by Computer on 14/11/14.
//
//

#include "ElementSushi.h"

// 普通精灵配图文件名称
static const char* element_normal[ELEMENT_SORT] = {
    "sushi_1n.png",
    "sushi_2n.png",
    "sushi_3n.png",
    "sushi_4n.png",
    "sushi_5n.png",
    "sushi_6n.png"
};
// 水平合成精灵配图文件名称
static const char* element_level[ELEMENT_SORT] = {
    "sushi_1v.png",
    "sushi_2v.png",
    "sushi_3v.png",
    "sushi_4v.png",
    "sushi_5v.png",
    "sushi_6v.png"
};
// 垂直合成精灵配图文件名称
static const char* element_vertical[ELEMENT_SORT] = {
    "sushi_1h.png",
    "sushi_2h.png",
    "sushi_3h.png",
    "sushi_4h.png",
    "sushi_5h.png",
    "sushi_6h.png"
};

Element* Element::create(int row, int column, int normal_pic/*=0*/)
{
    Element* new_element = new Element();
    
    new_element->e_row = row;
    new_element->e_column = column;
    
    // 设置默认图片名称和默认索引
    if (normal_pic < ELEMENT_SORT and normal_pic >= 1) {
        new_element->e_index = 0;
        memcpy(&new_element->e_pic, element_normal[normal_pic], sizeof(char16_t));
    }else {
        int _element_sort = ELEMENT_SORT;
        new_element->e_index = arc4random() % _element_sort;
        memcpy(&new_element->e_pic, element_normal[new_element->e_index], sizeof(char16_t));
    }
    
    // 图片在未放置前，基础坐标都是0，只有位置的2维数组
    log("picname => %s", element_normal[new_element->e_index]);
    new_element->initWithSpriteFrameName(element_normal[new_element->e_index]);
    new_element->autorelease();
    return new_element;
}

float Element::getContentWidth()
{
    Sprite* exampleT = Sprite::createWithSpriteFrameName(element_normal[0]);
    if (exampleT)
        return exampleT->getContentSize().width;
    return 0;
}

float Element::getContentHigth()
{
    Sprite* exampleT = Sprite::createWithSpriteFrameName(element_normal[0]);
    if (exampleT)
        return exampleT->getContentSize().height;
    return 0;
}