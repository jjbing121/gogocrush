#include "HelloWorldScene.h"
#include "ElementSushi.h"
#include "ResultScene.h"

USING_NS_CC;

HelloWorld::HelloWorld() :
    _isTouched(true),
    _isMoved(false),
    _isAction(false),
    _isSpecialAction(false),
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
#pragma mark - 整体场景初始化
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    Size gsize = Director::getInstance()->getVisibleSize();

    // 1. 基础初始化(获取背景图片)
//      bg = Sprite::create("background_iphone6s.png");    // iphone6s
//      bg = Sprite::create("background_iphone6.png");     // iphone6
      bg = Sprite::create("background_iphone5.png");             // iphone5s
//      bg = Sprite::create("background_iphone4s.png");    // iphone4s

    bg->setPosition(gsize.width/2, gsize.height/2);
    bg->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(bg);
    
    // 2. 预加载资源(所有的寿司单例图)
    SpriteFrameCache* sushiCache = SpriteFrameCache::getInstance();
    sushiCache->addSpriteFramesWithFile("allvegetables.plist", "allvegetables.png");
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("bgmusic.mp3");
    
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
    
    // 6. 初始化所有的触摸动作
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::global_touch_on, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::global_touch_move, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::global_touch_end, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    // 7. 初始化全局定时器
    // 7.1 进度条
    Sprite* processbar_str = Sprite::create("processbutton_start.png");
    Sprite* processbar_end = Sprite::create("processbutton_after.png");
    
    processbar_str->setPosition(gsize.width/2, 70);
    processbar_str->setAnchorPoint(Vec2(0.5, 0.5));
    ptimer = ProgressTimer::create(processbar_end);
    ptimer->setType(ProgressTimer::Type::BAR);
    ptimer->setMidpoint(Vec2(0, 0.5));
    ptimer->setBarChangeRate(Vec2(1, 0));
    ptimer->setPercentage(0);
    ptimer->setPosition(gsize.width/2, 70);
    ptimer->setAnchorPoint(Vec2(0.5, 0.5));
    
    // 7.2 倒计时
    countdown = Label::createWithSystemFont("0", "Courier", 50);
    countdown->setPosition(gsize.width/2+(processbar_str->getContentSize().width/2)+40, 70);
    countdown->setAnchorPoint(Vec2(0.5,0.5));
    countdown->setTextColor(Color4B(100, 0, 0, 150));
    
    this->addChild(processbar_str, 20);
    this->addChild(ptimer, 21);
    this->addChild(countdown);
    
    // 8. 记分板
    pointboard = Label::createWithSystemFont("0", "Courier", 50);
    pointboard->setPosition(gsize.width-80, gsize.height-60);
    pointboard->setAnchorPoint(Vec2(0.5,0.5));
    pointboard->setTextColor(Color4B(0, 0, 0, 0));
    this->addChild(pointboard);
    
    // 整体定时器设置
    scheduleUpdate();
    schedule(schedule_selector(HelloWorld::process_sixty_seconds), 1);
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

#pragma mark - 整体触摸动作实现
/*virtual*/ bool HelloWorld::global_touch_on(Touch* t, Event* e)
{
    // 获得用户点击的位置
    if (_isTouched) {
        auto location = t->getLocation();
        // 判断用户是否点击在寿司上
        touch_element = user_click_sushi(&location);
        if (touch_element != NULL) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("effectvoice.wav");
            return _isTouched;
        }
        return true;
    }
    return true;
}

/*virtual*/ void HelloWorld::global_touch_move(Touch* t, Event* e)
{
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

#pragma mark - 手指移动(上下左右)
    
    // 向上
    Rect new_up_rect = Rect(touch_element->getPositionX(), touch_element->getPositionY()+(element_higth_half), touch_element->getContentSize().width, touch_element->getContentSize().height);
    if (new_up_rect.containsPoint(location)) {
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
    Rect new_down_rect = Rect(touch_element->getPositionX(), touch_element->getPositionY()-(element_higth_half), touch_element->getContentSize().width, touch_element->getContentSize().height);
    if (new_down_rect.containsPoint(location)) {
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
    Rect new_left_rect = Rect(touch_element->getPositionX()-(element_width_half), touch_element->getPositionY(), touch_element->getContentSize().width, touch_element->getContentSize().height);
    if (new_left_rect.containsPoint(location)) {
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
    Rect new_right_rect = Rect(touch_element->getPositionX()+(element_width_half), touch_element->getPositionY(), touch_element->getContentSize().width, touch_element->getContentSize().height);
    if (new_right_rect.containsPoint(location)) {
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

    // 若要形成交换回来的情况，则需要做如下2点
    // 1. 纪录开始元素和交换后的元素 各2个
    // 2. 比较这4个元素是否有含有3个以上的合并状态
    // 存在： 则合并动画
    // 不存在：则替换回来，交换取消
}

#pragma mark - 整体定时器实现
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
    
    // 触发状态使用的递归方式
    check_global_sushi();   // 递归移动所有的寿司和检测消失
    filling_sushi();        // 消除所有相似寿司
    update_new_sushi();     // 补充所有寿司
    return;
}

/*virtual*/ void HelloWorld::check_global_sushi()
{
    if (_isSpecialAction == true) {
        return;
    }
    
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
                // 三个元素进行合并消除操作
                // 积分原则：
                //          普通元素消除1个共计10分, 一次消除3个共计30分 = (10*3) = 30分
                //          水平消除1个共计30分, 一次消除一行共计6个 ＝ (5*10+30) = 80分
                //          垂直消除1个共计20分, 一次消除一列共计11个 = (10*10+20) = 120分
                if (sure_list.size() == 3) {
                    // 注意删除过的需要进行重复判断
                    for (int each_element = 0; each_element<sure_list.size(); each_element++) {
                        if (sure_list[each_element]) {
                            // 将寿司从父节点中移除 - 普通消除
                            if (sure_list[each_element]->getSPETYPE() == 0) {
                                sure_list[each_element]->runAction(Sequence::create(
                                                                                    FadeTo::create(0.5, 1),
                                                                                    CallFuncN::create(CC_CALLBACK_1(HelloWorld::remove_sushi, this)),
                                                                                    NULL));
                                this->setPointBoard(1);
                            }
                            // 特殊消除1 ：水平消除
                            else if (sure_list[each_element]->getSPETYPE() == 1) {
                                this->CleanByLevel(Vec2(sure_list[each_element]->getPositionX()+(Element::getContentWidth()/2),
                                                        sure_list[each_element]->getPositionY()+(Element::getContentWidth()/2)));
                                // 找到所有的本行寿司
                                int cols = sure_list[each_element]->getCOLUMN();
                                // 清理这些寿司
                                for (int ci=1; ci<=_element_column; ci++) {
                                    Element* element_row = vector_element[_element_row*(ci-1)+(cols-1)];
                                    element_row->runAction(Sequence::create(
                                                                            FadeTo::create(0.5, 1),
                                                                            CallFuncN::create(CC_CALLBACK_1(HelloWorld::remove_sushi, this)),
                                                                            NULL));
                                    this->setPointBoard(1);
                                }
                                this->setPointBoard(15);
                            }
                            // 特殊消除2 ：垂直消除
                            else if (sure_list[each_element]->getSPETYPE() == 2) {
                                this->CleanByVeritial(Vec2(sure_list[each_element]->getPositionX()+(Element::getContentWidth()/2),
                                                           sure_list[each_element]->getPositionY()+(Element::getContentHigth()/2)));
                                // 找到所有的本列寿司
                                int rows = sure_list[each_element]->getROW();
                                // 清理这些寿司
                                for (int di=1; di<=_element_row; di++) {
                                    Element* element_col = vector_element[_element_row*(rows-1)+(di-1)];
                                    element_col->runAction(Sequence::create(
                                                                            FadeTo::create(0.5, 1),
                                                                            CallFuncN::create(CC_CALLBACK_1(HelloWorld::remove_sushi, this)),
                                                                            NULL));
                                    this->setPointBoard(1);
                                }
                                this->setPointBoard(10);
                            }
                            // 光圈效果
                            auto circleSprite = Sprite::create("circle.png");
                            addChild(circleSprite, 10);
                            circleSprite->setPosition(sure_list[each_element]->getPosition()+Vec2(Element::getContentWidth()/2,Element::getContentHigth()/2));
                            circleSprite->setAnchorPoint(Vec2(0.5, 0.5));
                            circleSprite->setScale(0);// start size
                            circleSprite->runAction(Sequence::create(ScaleTo::create(0.5, 1.0),
                                                                     CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, circleSprite)),
                                                                     NULL));
                            
                            // 星星效果
                            auto particleStars = ParticleSystemQuad::create("stars.plist");
                            particleStars->setAutoRemoveOnFinish(true);
                            particleStars->setBlendAdditive(false);
                            particleStars->setPosition(sure_list[each_element]->getPosition()+Vec2(Element::getContentWidth()/2,Element::getContentHigth()/2));
                            particleStars->setAnchorPoint(Vec2(0.5, 0.5));
                            particleStars->setScale(0.3);
                            addChild(particleStars, 20);
                        }
                    }
                }
                
                // 4个以上的合并
                else if (sure_list.size() >= 4)
                {
                    _isSpecialAction = true;    // 开始进行元素补充，不可以进行同时的其他操作
                    bool vertical_or_level = false; // 默认false为水平, true为垂直
                    
                    if (sure_list[0]->getROW() == sure_list[sure_list.size()-1]->getROW())
                        vertical_or_level = true;

                    // 随机选取一个点: 基本使用最左边和最下面多点来实现
                    // 来作为4个点的生成点 : random_reserved
                    int random_sure = 0;
                    Element* random_reserverd = Element::createSpecial(sure_list[random_sure]->getROW(), sure_list[random_sure]->getCOLUMN(), !vertical_or_level, sure_list[random_sure]->getINDEX());
                    
                    // 将所有寿司从父节点中移除(除了第一个)
                    for (int j = 1; j<sure_list.size(); j++) {
                        if (sure_list[j]) {
                            sure_list[j]->runAction(Sequence::create(
                                                                    CallFuncN::create(CC_CALLBACK_1(HelloWorld::remove_sushi, this)),
                                                                    NULL));
                        }
                        this->setPointBoard(1);
                    }
                    // 将第一个寿司进行替换操作 (先从场景中移除，再替换2维数组)
                    sure_list[random_sure]->stopAllActions();
                    sure_list[random_sure]->removeFromParent();
                    
                    Vec2 special_location = this->get_element_location(random_reserverd->getROW(), random_reserverd->getCOLUMN());
                    random_reserverd->setPosition(special_location);
                    random_reserverd->setAnchorPoint(Vec2(0, 0));
                    this->addChild(random_reserverd);
                    vector_element[_element_row * (random_reserverd->getROW()-1) + (random_reserverd->getCOLUMN()-1)] = random_reserverd;
                    
                    _isSpecialAction = false;    // 补充完成后，释放即可
                }
            }
        }
    }
}

/*virtual*/ void HelloWorld::filling_sushi()
{
    if (_isSpecialAction == true) {
        return;
    }
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
    if (_isSpecialAction == true) {
        return;
    }
    
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
    if (_isSpecialAction == true) {
        return;
    }
    
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

#pragma mark - 爆炸效果(带有效果的新单位合并)
void HelloWorld::CleanByLevel(Vec2 point)
{
    Size size = Director::getInstance()->getWinSize();
    float scaleX = 4 ;
    float scaleY = 0.7 ;
    float time = 0.3;
    Vec2 startPosition = point;
    float speed = 0.6f;
    
    auto colorSpriteRight = Sprite::create("colorHRight.png");
    addChild(colorSpriteRight, 10);
    Point endPosition1 = Point(point.x - size.width, point.y);
    colorSpriteRight->setPosition(startPosition);
    colorSpriteRight->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                                 MoveTo::create(speed, endPosition1),
                                                 CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteRight)),
                                                 NULL));
    
    auto colorSpriteLeft = Sprite::create("colorHLeft.png");
    addChild(colorSpriteLeft, 10);
    Point endPosition2 = Point(point.x + size.width, point.y);
    colorSpriteLeft->setPosition(startPosition);
    colorSpriteLeft->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                                MoveTo::create(speed, endPosition2),
                                                CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteLeft)),
                                                NULL));
    
    
}

void HelloWorld::CleanByVeritial(Vec2 point)
{
    Size size = Director::getInstance()->getWinSize();
    float scaleY = 4 ;
    float scaleX = 0.7 ;
    float time = 0.3;
    Vec2 startPosition = point;
    float speed = 0.6f;
    
    auto colorSpriteDown = Sprite::create("colorVDown.png");
    addChild(colorSpriteDown, 10);
    Point endPosition1 = Point(point.x , point.y - size.height);
    colorSpriteDown->setPosition(startPosition);
    colorSpriteDown->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                                MoveTo::create(speed, endPosition1),
                                                CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteDown)),
                                                NULL));
    
    auto colorSpriteUp = Sprite::create("colorVUp.png");
    addChild(colorSpriteUp, 10);
    Point endPosition2 = Point(point.x , point.y + size.height);
    colorSpriteUp->setPosition(startPosition);
    colorSpriteUp->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                              MoveTo::create(speed, endPosition2),
                                              CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteUp)),
                                              NULL));
}

/*virtual*/ void HelloWorld::process_sixty_seconds(float dtime)
{
    float percent_get = ptimer->getPercentage();
    
    percent_get += 2;
    
    if (percent_get <= 100){
        // 计时器增加
        ptimer->setPercentage(percent_get);
        // 计数器增加
        this->setCountDown(1);
    }
    else
    {
        // 跳转到结束页面
        std::string get_atlast_point = pointboard->getString();
        this->removeChild(pointboard);
        Director::getInstance()->replaceScene(ResultScene::createScene(get_atlast_point));
    }
}

void HelloWorld::setCountDown(float dtime)
{
    int tmp_count_down = atoi(countdown->getString().c_str());
    char add_count_down[6];
    memset(add_count_down, 0, 6);
    sprintf(add_count_down, "%d", (int)(tmp_count_down+dtime));
    countdown->setString(add_count_down);
}

/*virtual*/ void HelloWorld::setPointBoard(int point)
{
    int tmp_point = atoi(pointboard->getString().c_str());
    char point_add[6];
    memset(point_add, 0, 6);
    sprintf(point_add, "%d", (int)(tmp_point+point));
    pointboard->setString(point_add);
}