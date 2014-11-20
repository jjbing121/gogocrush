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
    "broccoli.png",
    "carrot.png",
    "mushroom.png",
    "bell_pepper.png",
    "strawberry.png",
    "sugar_snap.png"
};
// 水平合成精灵配图文件名称
static const char* element_level[ELEMENT_SORT] = {
    "broccoli_H.png",
    "carrot_H.png",
    "mushroom_H.png",
    "bell_pepper_H.png",
    "strawberry_H.png",
    "sugar_snap_H.png"
};
// 垂直合成精灵配图文件名称
static const char* element_vertical[ELEMENT_SORT] = {
    "broccoli_V.png",
    "carrot_V.png",
    "mushroom_V.png",
    "bell_pepper_V.png",
    "strawberry_V.png",
    "sugar_snap_V.png"
};

Element* Element::create(int row, int column, int normal_pic/*=0*/)
{
    Size gsize = Director::getInstance()->getVisibleSize();
    float scale = gsize.width/gsize.height;
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
    new_element->setContentSize(Size(new_element->getContentSize().width * scale, new_element->getContentSize().height * scale));
    new_element->initWithSpriteFrameName(element_normal[new_element->e_index]);
    new_element->autorelease();
    return new_element;
}

Element* Element::createSpecial(int row, int column, bool direct, int index_number)
{
    Size gsize = Director::getInstance()->getVisibleSize();
    float scale = gsize.width/gsize.height;
    Element* new_element = new Element();
    
    new_element->e_row = row;
    new_element->e_column = column;
    new_element->e_index = index_number;
    new_element->setContentSize(Size(new_element->getContentSize().width * scale, new_element->getContentSize().height * scale));
    
    // 水平图标
    if (!direct) {
        new_element->e_special = 1;
        new_element->initWithSpriteFrameName(element_level[new_element->e_index]);
    // 垂直图标
    } else {
        new_element->e_special = 2;
        new_element->initWithSpriteFrameName(element_vertical[new_element->e_index]);
    }
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