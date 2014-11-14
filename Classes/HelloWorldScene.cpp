#include "HelloWorldScene.h"
#include "ElementSushi.h"

USING_NS_CC;
HelloWorld::~HelloWorld()
{
    if (vector_element) {
        free(vector_element);
    }
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    Size gsize = Director::getInstance()->getVisibleSize();

    // 1. 基础初始化(获取背景图片)
    Sprite* bg = Sprite::create("background_iphone6.png");
    bg->setPosition(gsize.width/2, gsize.height/2);
    bg->setAnchorPoint(Vec2(0.5,0.5));
    this->addChild(bg);

    // 2. 预加载资源(所有的寿司单例图)
    SpriteFrameCache* sushiCache = SpriteFrameCache::getInstance();
    sushiCache->addSpriteFramesWithFile("allsushi.plist", "allsushi.png");
    
    // 3. 定义起始点(外部引用的时候需要优先赋值)
    _element_row          = ELEMENT_ROW;
    _element_row_space    = ELEMENT_ROW_SPACE;
    _element_column       = ELEMENT_COLUMN;
    _element_column_space = ELEMENT_COL_SPACE;
    int vector_element_size = sizeof(Element*) * _element_row * _element_column;
    source_height = (gsize.height - ((_element_row*Element::getContentWidth())+((_element_row-1)*_element_row_space))) / 2;
    source_weight = (gsize.width - ((_element_column*Element::getContentHigth())+((_element_column-1)*_element_column_space))) / 2;

    // 4. 初始化2维数组
    vector_element = (Element**)malloc(vector_element_size);
    memset(vector_element, 0, vector_element_size);
    
    // 5. 整体初始化这个2维矩阵
    init_vec2_element();
    
    return true;
    
}

/*virtual*/ void HelloWorld::init_vec2_element(void)
{
    for (int i=1; i<=ELEMENT_COLUMN; i++) {
        for (int j=1; j<=ELEMENT_ROW; j++) {
            create_each_doaction(i, j);
        }
    }
}

/*virtual*/ void HelloWorld::create_each_doaction(int row_number, int col_number)
{
    Element* _element = Element::create(row_number, col_number);
    Vec2 _element_location = this->get_element_location(row_number, col_number);
    _element->setPosition(_element_location);
    _element->setAnchorPoint(Vec2(0, 0));
    this->addChild(_element);
}

/*virtual*/ Vec2 HelloWorld::get_element_location(int row_number, int col_number)
{
    int x = source_weight + (Element::getContentWidth() * (row_number-1)) + (_element_row_space * (row_number-1));
    int y = source_height + (Element::getContentHigth() * (col_number-1)) + (_element_column_space * (col_number-1));
    return Vec2(x,y);
}
