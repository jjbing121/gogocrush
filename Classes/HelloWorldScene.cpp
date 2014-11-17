#include "HelloWorldScene.h"
#include "ElementSushi.h"

USING_NS_CC;

HelloWorld::HelloWorld() :
    _isTouched(true),
    _isMoved(false),
    _isAction(false),
    touch_element(NULL),
    touch_after_element(NULL)
{

}

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
    bg->setAnchorPoint(Vec2(0.5, 0.5));
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
    scheduleUpdate();
    
    // 6. 初始化所有的触摸动作
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::global_touch_on, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::global_touch_move, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::global_touch_end, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
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
    // 获取当前寿司的位置，并初始化内容
    Vec2 _element_location = this->get_element_location(row_number, col_number);        // 最后需要到达的位置
    Vec2 _element_location_before = Vec2(_element_location.x, _element_location.y+500); // 最开始存在的位置：根据最后的位置推算
    // 添加下落到位动作
    _element->setPosition(_element_location_before);
    MoveTo* _element_action = MoveTo::create(1, _element_location);
    _element->setAnchorPoint(Vec2(0, 0));
    _element->runAction(_element_action);
    this->addChild(_element);
    // 添加至2维数组 (row * row_number + column_number)
    // 注意行和列对应2维数组的行和列各－1
    vector_element[_element_row * (row_number-1) + (col_number-1)] = _element;
}

/*virtual*/ Vec2 HelloWorld::get_element_location(int row_number, int col_number)
{
    int x = source_weight + (Element::getContentWidth() * (row_number-1)) + (_element_row_space * (row_number-1));
    int y = source_height + (Element::getContentHigth() * (col_number-1)) + (_element_column_space * (col_number-1));
    return Vec2(x,y);
}

/*virtual*/ bool HelloWorld::global_touch_on(Touch* t, Event* e)
{
    log("touch on");
    log("touch judge -> %d", _isTouched);
    // 获得用户点击的位置
    if (_isTouched) {
        auto location = t->getLocation();
        // 判断用户是否点击在寿司上
        touch_element = user_click_sushi(&location);
        if (touch_element != NULL) {
            return _isTouched;
        }
        return true;
    }
    return true;
}

/*virtual*/ void HelloWorld::global_touch_move(Touch* t, Event* e)
{
#pragma mark - 调试打印
    log("{%f, %f}", t->getLocation().x, t->getLocation().y);
    log("touch false or true => %d", _isTouched);
    
    // 判断最先决条件
    if (!_isTouched || touch_element == NULL) {
        return;
    }
    
    // 开始进行触发判断
    // 获取当前元素的 行和列
    int touch_element_row = touch_element->getROW();
    int touch_element_col = touch_element->getCOLUMN();
    
    // 查询运动趋势是否与其他块接触
    auto location = t->getLocation();
    auto element_width_half = touch_element->getContentSize().width;
    auto element_higth_half = touch_element->getContentSize().height;

    // 向上
    Rect new_up_rect = Rect(touch_element->getPositionX()-(element_width_half/2), touch_element->getPositionY()+(element_higth_half), touch_element->getContentSize().width, touch_element->getContentSize().height);
    if (new_up_rect.containsPoint(location)) {
        log("up");
        int after_touch_col = touch_element_col + 1;
        if (after_touch_col > 0) {
            // 获取触发方向的寿司
            touch_after_element = vector_element[_element_row* (touch_element_row-1) + (after_touch_col-1)];
        }
        // 交换寿司
        this->swap_sushi(touch_element, touch_after_element);
        return;
    }
    // 向下
    Rect new_down_rect = Rect(touch_element->getPositionX()-(element_width_half/2), touch_element->getPositionY()-(element_higth_half), touch_element->getContentSize().width, touch_element->getContentSize().height);
    if (new_down_rect.containsPoint(location)) {
        log("down");
        int after_touch_col = touch_element_col - 1;
        if (after_touch_col > 0) {
            // 获取触发方向的寿司
            touch_after_element = vector_element[_element_row* (touch_element_row-1) + (after_touch_col-1)];
        }
        // 交换寿司
        this->swap_sushi(touch_element, touch_after_element);
        return;

    }
    // 向左
    Rect new_left_rect = Rect(touch_element->getPositionX()-(element_width_half), touch_element->getPositionY()-(element_higth_half/2), touch_element->getContentSize().width, touch_element->getContentSize().height);
    if (new_left_rect.containsPoint(location)) {
        log("left");
        int after_touch_row = touch_element_row - 1;
        if (after_touch_row > 0) {
            // 获取触发方向的寿司
            touch_after_element = vector_element[_element_row* (after_touch_row-1) + (touch_element_col-1)];
        }
        // 交换寿司
        this->swap_sushi(touch_element, touch_after_element);
        return;
    }
    // 向右
    Rect new_right_rect = Rect(touch_element->getPositionX()+(element_width_half), touch_element->getPositionY()-(element_higth_half/2), touch_element->getContentSize().width, touch_element->getContentSize().height);
    if (new_right_rect.containsPoint(location)) {
        log("right");
        int after_touch_row = touch_element_row + 1;
        if (after_touch_row > 0) {
            // 获取触发方向的寿司
            touch_after_element = vector_element[_element_row* (after_touch_row-1) + (touch_element_col-1)];
        }
        // 交换寿司
        this->swap_sushi(touch_element, touch_after_element);
        return;
    }
}
/*virtual*/ void HelloWorld::global_touch_end(Touch* t, Event* e)
{
    _isTouched = true;
}
/*virtual*/ Element* HelloWorld::user_click_sushi(Vec2* click_point)
{
    Element* _tmp_element;
    Rect rect = Rect(0, 0, 0, 0);
    
    for (int i=1; i<=ELEMENT_COLUMN; i++) {
        for (int j=1; j<=ELEMENT_ROW; j++) {
            // 判断2维矩阵中的寿司存在 : 为了节省重复使用矩阵，则声明一个固定的变量
            _tmp_element = vector_element[_element_row*(i-1) + (j-1)];
            if (_tmp_element) {
                rect.origin.x = _tmp_element->getPositionX();
                rect.origin.y = _tmp_element->getPositionY();
                rect.size = _tmp_element->getContentSize();
                if (rect.containsPoint(*click_point)) {
                    return _tmp_element;
                }
            }
        }
    }
    return nullptr;
}

/*virtual*/ void HelloWorld::swap_sushi(Element* e_before, Element* e_after)
{
    _isTouched = false;
    if (e_before == NULL || e_after == NULL || _isMoved) {
        return;
    }
    
    // 交换动画前置内容提取
    Vec2 e_before_position = e_before->getPosition();
    Vec2 e_after_position = e_after->getPosition();
    
    // 简单交换位置(2维数组中)
    vector_element[_element_row * (e_before->getROW()-1) + (e_before->getCOLUMN()-1)] = e_after;
    vector_element[_element_row * (e_after->getROW()-1) + (e_after->getCOLUMN()-1)] = e_before;
    
    // 交换所有的基本属性(注意使用中间变量)
    int e_tmp_row = e_before->getROW();
    int e_tmp_col = e_before->getCOLUMN();
    e_before->setROW(e_after->getROW());
    e_before->setCOLUMN(e_after->getCOLUMN());
    e_after->setROW(e_tmp_row);
    e_after->setCOLUMN(e_tmp_col);

    // 交换动画执行
    e_before->runAction(Sequence::create(
                                         MoveTo::create(0.5, e_after_position),
                                         NULL));
    e_after->runAction(Sequence::create(
                                        MoveTo::create(0.5, e_before_position),
                                        NULL));
}

/*virtual*/ void HelloWorld::update(float dtime)
{
    if (!_isAction) {
        _isAction = true;
        // 遍历所有的寿司，查看寿司是否有运动状态在
        for (int i=1; i<=ELEMENT_COLUMN; i++) {
            for (int j=1; j<=ELEMENT_ROW; j++) {
                Element* judge_element = vector_element[_element_row*(i-1)+(j-1)];
                if (judge_element != NULL && judge_element->getNumberOfRunningActions() > 0) {
                    _isAction = false;
                    break;
                }
            }
            break;
        }
        return;
    }
    
    // 判断到没有寿司在执行其他动作的情况下 => _isAction=true;
//    _isTouched = !_isAction; // 触发可以移动的状态
    
    // 触发状态使用的递归方式
    check_global_sushi();   // 递归移动所有的寿司和检测消失
    filling_sushi();        // 消除所有相似寿司
    update_new_sushi();     // 补充所有寿司
    return;
}

/*virtual*/ void HelloWorld::check_global_sushi()
{
    for (int i=1; i<=ELEMENT_COLUMN; i++) {
        for (int j=1; j<=ELEMENT_ROW; j++) {
            // 单一元素只向右和向上匹配
            Element* check_element = vector_element[_element_row * (i-1) + (j-1)];
            
            if (check_element) {

                // 向右检测
                std::vector<Element*> col_list;
                this->col_check_sushi(check_element, col_list);
                
                // 向上检测
                std::vector<Element*> row_list;
                this->row_check_sushi(check_element, row_list);
                
                // 判断哪个方向是正确的方向 -> 保存在sure_list中
                std::vector<Element*> &sure_list = col_list.size() > row_list.size() ? col_list : row_list;
                // 根据判断来进行消除动作
                if (sure_list.size() >= 3) {
                    // 注意删除过的需要进行重复判断
                    
                    for (int each_element = 0; each_element<sure_list.size(); each_element++) {
                        if (sure_list[each_element]) {
                            // 将寿司从父节点中移除
                            sure_list[each_element]->runAction(Sequence::create(
                                                                            ScaleTo::create(0.5, 0.0),
                                                                            CallFuncN::create(CC_CALLBACK_1(HelloWorld::remove_sushi, this)),
                                                                            NULL));
                        }
                    }
                }
            }
        }
    }
}

/*virtual*/ void HelloWorld::filling_sushi()
{
    for (int i=1; i<=ELEMENT_COLUMN; i++) {
        for (int j=1; j<=ELEMENT_ROW; j++) {
            // 从原点开始扫描
            Element* tmp_element = vector_element[_element_row * (i-1) + (j-1)];
            if (!tmp_element) {
                // 向上查找最近一个存在的元素
                Element* nearest_element = this->filling_up(i, j);
                // 开始消除机制
                makeup_sushi(nearest_element, i, j);
            }
        }
    }
}

/*virtual*/ Element* HelloWorld::filling_up(int row_number, int col_number)
{
    for (int i=col_number; i<=_element_row; i++) {
        if (vector_element[_element_row * (row_number-1) + (i-1)]) {
            Element* thiselement = vector_element[_element_row * (row_number-1) + (i-1)];
            return thiselement;
        }
    }
    return NULL;
}

/*virtual*/ void HelloWorld::makeup_sushi(Element* exist, int miss_row, int miss_col)
{
    bool get_flush = false;
    Element* exist_makeup;      // 本函数临时变量
    int exist_row;              // 原始函数变量的行(临时保存)
    int exist_col;              // 原始函数变量的列(临时保存)
    
    // 从判断到的寿司开始
    if (exist) { // 已经存在的寿司并且被查询到
        exist_row = exist->getROW();
        exist_col = exist->getCOLUMN();
        exist_makeup = exist;
        
        exist_makeup->stopAllActions();
        MoveTo* mvto = MoveTo::create(0.5, get_element_location(miss_row, miss_col));
        exist_makeup->runAction(mvto);
    } else {
        return;
    }
    
    // 将移动后的内容更改对应值
    vector_element[_element_row * (miss_row-1) + (miss_col-1)] = exist_makeup;
    exist_makeup->setROW(miss_row);
    exist_makeup->setCOLUMN(miss_col);
    // 移动的内容置空
    vector_element[_element_row * (exist_row-1) + (exist_col-1)] = NULL;
    // 将移动后新增精灵增加入场景
    if (get_flush == true)
        this->addChild(exist_makeup);
}

/*virtual*/ void HelloWorld::update_new_sushi()
{
    for (int i=1; i<=ELEMENT_COLUMN; i++) {
        for (int j=1; j<=ELEMENT_ROW; j++) {
            // 全局消失的寿司位置
            if (!vector_element[_element_row * (i-1) + (j-1)]) {
                Element* update_element = Element::create(i, j);
                // 获取当前寿司的位置，并初始化内容
                Vec2 update_end_location = get_element_location(i, j);                                // 最后需要到达的位置
                Vec2 update_before_location = Vec2(update_end_location.x, update_end_location.y+500); // 最开始存在的位置：根据最后的位置推算
                // 添加下落到位动作
                update_element->setPosition(update_before_location);
                MoveTo* update_action = MoveTo::create(1, update_end_location);
                update_element->setAnchorPoint(Vec2(0, 0));
                update_element->stopAllActions();
                update_element->runAction(update_action);
                // 重新增加入场景
                this->addChild(update_element);
                // 重新加载到2维数组
                vector_element[_element_row * (i-1) + (j-1)] = update_element;
            }
        }
    }
}

#warning 数据bug
/*virtual*/ void HelloWorld::row_check_sushi(Element* check_element, std::vector<Element*>& col_list)
{
    // 主动放入第一个元素
    col_list.push_back(check_element);
    
    // 获取当前元素的row 和 col值
    int row = check_element->getROW();
    int col = check_element->getCOLUMN();
    
    // 开始向上匹配index值
    for (int i = col; i<_element_row; i++) {
        Element* up_element = vector_element[_element_row * (row-1) + ((i-1)+1)];
        if (up_element && check_element->getINDEX() == up_element->getINDEX()) {
            col_list.push_back(up_element);
        }
        else
            return;
    }
}

/*virtual*/ void HelloWorld::col_check_sushi(Element* check_element, std::vector<Element*>& row_list)
{
    // 主动放入第一个元素
    row_list.push_back(check_element);
    
    // 获取当前元素的row 和 col值
    int row = check_element->getROW();
    int col = check_element->getCOLUMN();

    // 开始向右匹配index值
    for (int i = row; i<_element_column; i++) {
        Element* right_element = vector_element[_element_row * ((i-1)+1) + (col-1)];
        if (right_element && check_element->getINDEX() == right_element->getINDEX()) {
            row_list.push_back(right_element);
        }
        else
            return;
    }
}

/*virtual*/ void HelloWorld::remove_sushi(Node* node)
{
    Element* remove_sushi_ = (Element*)node;
    // 从2维数组中删除
    vector_element[_element_row * (remove_sushi_->getROW()-1) + (remove_sushi_->getCOLUMN()-1)] = NULL;
    // 从父类中删除
    remove_sushi_->removeFromParent();
}