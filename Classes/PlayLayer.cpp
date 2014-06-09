#include "PlayLayer.h"
#include "FoodSprite.h"

#define MATRIX_WIDTH (9)
#define MATRIX_HEIGHT (9)

#define FOOD_GAP (1)

PlayLayer::PlayLayer()
: spriteSheet(NULL)
, m_matrix(NULL)
, m_width(0)
, m_height(0)
, m_matrixLeftBottomX(0)
, m_matrixLeftBottomY(0)
, m_canDrop(false)
, m_canRemove(false)
, m_isAnimationing(true)//start with drop animation
, m_isTouchEnable(true)
, m_srcFood(NULL)
, m_destFood(NULL)
, m_movingVertical(true)//drop animation is vertical
{
}

PlayLayer::~PlayLayer()
{
    CC_SAFE_FREE(m_matrix);
    CC_SAFE_DELETE(mFSM);
}

PlayLayer* PlayLayer::createLayer()
{
    auto layer = new PlayLayer();
    if (layer && layer->init()) {
        layer->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(layer);
    }
    return layer;
}

bool PlayLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
//    auto background = Sprite::create("gamebg.png");
//    background->setAnchorPoint(Point(0, 1));
//    background->setPosition(Point(0, SIZE_H));
//    addChild(background);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sushi.plist");
    spriteSheet = SpriteBatchNode::create("sushi.pvr.ccz");
    addChild(spriteSheet);
    
    m_width = MATRIX_WIDTH;
    m_height = MATRIX_HEIGHT;
    
    m_matrixLeftBottomX = (SIZE_W - FoodSprite::getContentWidth() * m_width - (m_width - 1) * FOOD_GAP) / 2;
    m_matrixLeftBottomY = (SIZE_H - FoodSprite::getContentWidth() * m_height - (m_height - 1) * FOOD_GAP) / 2;
    
    int arraySize = sizeof(FoodSprite *) * m_width * m_height;
    m_matrix = (FoodSprite **)malloc(arraySize);
    memset((void*)m_matrix, 0, arraySize);
    
    initMatrix();
    // 界面初始化完成之后，才能启动状态机
    mFSM = FSM::createFSM(this);
    CC_SAFE_RETAIN(mFSM);
    mFSM->changeState(new ReadyState());
    scheduleUpdate();
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(PlayLayer::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(PlayLayer::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

void PlayLayer::initMatrix()
{
    for (int row = 0; row < m_height; row++) {
		for (int col = 0; col < m_width; col++) {
            createAndDropFood(row, col);
        }
    }
}

//******************************************************************************
// override Function
//******************************************************************************
#pragma mark - Override Function

bool PlayLayer::onTouchBegan(Touch *touch, Event *unused)
{
    m_srcFood = NULL;
    m_destFood = NULL;
    if (m_isTouchEnable) {
        auto location = touch->getLocation();
        m_srcFood = foodOfPoint(&location);
    }
    return m_isTouchEnable;
}

void PlayLayer::onTouchMoved(Touch *touch, Event *unused)
{
    if (!m_srcFood || !m_isTouchEnable) {
        return;
    }
    
    int row = m_srcFood->getRow();
    int col = m_srcFood->getCol();
    
    auto halfFoodWidth = m_srcFood->getContentSize().width / 2;
    auto halfFoodHeight = m_srcFood->getContentSize().height / 2;
    
    auto  upRect = Rect(m_srcFood->getPositionX() - halfFoodWidth,
                        m_srcFood->getPositionY() + halfFoodHeight,
                        m_srcFood->getContentSize().width,
                        m_srcFood->getContentSize().height);
    
    auto location = touch->getLocation();
    
    if (upRect.containsPoint(location)) {
        row++;
        if (row < m_height) {
            m_destFood = m_matrix[row * m_width + col];
        }
        m_movingVertical = true;
        swapFood();
        return;
    }
    
    auto  downRect = Rect(m_srcFood->getPositionX() - halfFoodWidth,
                        m_srcFood->getPositionY() - (halfFoodHeight * 3),
                        m_srcFood->getContentSize().width,
                        m_srcFood->getContentSize().height);
    
    if (downRect.containsPoint(location)) {
        row--;
        if (row >= 0) {
            m_destFood = m_matrix[row * m_width + col];
        }
        m_movingVertical = true;
        swapFood();
        return;
    }
    
    auto  leftRect = Rect(m_srcFood->getPositionX() - (halfFoodWidth * 3),
                          m_srcFood->getPositionY() - halfFoodHeight,
                          m_srcFood->getContentSize().width,
                          m_srcFood->getContentSize().height);
    
    if (leftRect.containsPoint(location)) {
        col--;
        if (col >= 0) {
            m_destFood = m_matrix[row * m_width + col];
        }
        m_movingVertical = false;
        swapFood();
        return;
    }
    
    auto  rightRect = Rect(m_srcFood->getPositionX() + halfFoodWidth,
                          m_srcFood->getPositionY() - halfFoodHeight,
                          m_srcFood->getContentSize().width,
                          m_srcFood->getContentSize().height);
    
    if (rightRect.containsPoint(location)) {
        col++;
        if (col < m_width) {
            m_destFood = m_matrix[row * m_width + col];
        }
        m_movingVertical = false;
        swapFood();
        return;
    }
}

void PlayLayer::update(float dt)
{
    if (m_isAnimationing) {
        m_isAnimationing = false;
        for (int i = 0; i < m_height * m_width; i++) {
            FoodSprite *food = m_matrix[i];
            if (food && food->getNumberOfRunningActions() > 0) {
                m_isAnimationing = true;
                break;
            }
        }
    }
    
    m_isTouchEnable = !m_isAnimationing;
    
    if (!m_isAnimationing) {
        mFSM->update(dt);
    }
}

//******************************************************************************
// Process Function
//******************************************************************************
#pragma mark - Process Function
void PlayLayer::swapFood()
{
    m_isAnimationing = true;
    m_isTouchEnable = false;
    if (!m_srcFood || !m_destFood) {
        m_movingVertical = true;
        return;
    }
    
    Point posOfSrc = m_srcFood->getPosition();
    Point posOfDest = m_destFood->getPosition();
    
    // 1.swap in matrix
    m_matrix[m_srcFood->getRow() * m_width + m_srcFood->getCol()] = m_destFood;
    m_matrix[m_destFood->getRow() * m_width + m_destFood->getCol()] = m_srcFood;
    int tmpRow = m_srcFood->getRow();
    int tmpCol = m_srcFood->getCol();
    m_srcFood->setRow(m_destFood->getRow());
    m_srcFood->setCol(m_destFood->getCol());
    m_destFood->setRow(tmpRow);
    m_destFood->setCol(tmpCol);
    
    // 2.check for remove able
    std::list<FoodSprite *> colChainListOfFirst;
    getColChain(m_srcFood, colChainListOfFirst);
    
    std::list<FoodSprite *> rowChainListOfFirst;
    getRowChain(m_srcFood, rowChainListOfFirst);
    
    std::list<FoodSprite *> colChainListOfSecond;
    getColChain(m_destFood, colChainListOfSecond);
    
    std::list<FoodSprite *> rowChainListOfSecond;
    getRowChain(m_destFood, rowChainListOfSecond);
    
    float time = 0.2;
    // 3.swap
    // have chain, just swap
    if ( colChainListOfFirst.size() >= 3 || rowChainListOfFirst.size() >= 3
        || colChainListOfSecond.size() >= 3 || rowChainListOfSecond.size() >= 3)
    {
        m_srcFood->runAction(MoveTo::create(time, posOfDest));
        m_destFood->runAction(MoveTo::create(time, posOfSrc));
        m_canRemove = true;
    }
    // no chain, swap and back
    else
    {
        m_matrix[m_srcFood->getRow() * m_width + m_srcFood->getCol()] = m_destFood;
        m_matrix[m_destFood->getRow() * m_width + m_destFood->getCol()] = m_srcFood;
        tmpRow = m_srcFood->getRow();
        tmpCol = m_srcFood->getCol();
        m_srcFood->setRow(m_destFood->getRow());
        m_srcFood->setCol(m_destFood->getCol());
        m_destFood->setRow(tmpRow);
        m_destFood->setCol(tmpCol);
        
        m_srcFood->runAction(Sequence::create( MoveTo::create(time, posOfDest),
                                          MoveTo::create(time, posOfSrc),
                                          NULL));
        m_destFood->runAction(Sequence::create( MoveTo::create(time, posOfSrc),
                                          MoveTo::create(time, posOfDest),
                                          NULL));
    }
}

//******************************************************************************
// Check Remove Function
//******************************************************************************
#pragma mark - Check Remove Function
//横向chain检查
void PlayLayer::getColChain(FoodSprite *food, std::list<FoodSprite *> &chainList)
{
    chainList.push_back(food);// add first food
    
    int neighborCol = food->getCol() - 1;
    while (neighborCol >= 0) {
        FoodSprite *neighborFood = m_matrix[food->getRow() * m_width + neighborCol];
        if (neighborFood
            && (neighborFood->getImgIndex() == food->getImgIndex())
            && !neighborFood->getIsNeedRemove()
            && !neighborFood->getIgnoreCheck()) {
            chainList.push_back(neighborFood);
            neighborCol--;
        } else {
            break;
        }
    }
    
    neighborCol = food->getCol() + 1;
    while (neighborCol < m_width) {
        FoodSprite *neighborFood = m_matrix[food->getRow() * m_width + neighborCol];
        if (neighborFood
            && (neighborFood->getImgIndex() == food->getImgIndex())
            && !neighborFood->getIsNeedRemove()
            && !neighborFood->getIgnoreCheck()) {
            chainList.push_back(neighborFood);
            neighborCol++;
        } else {
            break;
        }
    }
}


void PlayLayer::getRowChain(FoodSprite *food, std::list<FoodSprite *> &chainList)
{
    chainList.push_back(food);// add first food
    
    int neighborRow = food->getRow() - 1;
    while (neighborRow >= 0) {
        FoodSprite *neighborFood = m_matrix[neighborRow * m_width + food->getCol()];
        if (neighborFood
            && (neighborFood->getImgIndex() == food->getImgIndex())
            && !neighborFood->getIsNeedRemove()
            && !neighborFood->getIgnoreCheck()) {
            chainList.push_back(neighborFood);
            neighborRow--;
        } else {
            break;
        }
    }
    
    neighborRow = food->getRow() + 1;
    while (neighborRow < m_height) {
        FoodSprite *neighborFood = m_matrix[neighborRow * m_width + food->getCol()];
        if (neighborFood
            && (neighborFood->getImgIndex() == food->getImgIndex())
            && !neighborFood->getIsNeedRemove()
            && !neighborFood->getIgnoreCheck()) {
            chainList.push_back(neighborFood);
            neighborRow++;
        } else {
            break;
        }
    }
}

//******************************************************************************
// Explode Function
//******************************************************************************
#pragma mark - Explode Function
void PlayLayer::explodeSpecialHor(Point point)
{
    float scaleX = 4 ;
    float scaleY = 0.7 ;
    float time = 0.3;
    Point startPosition = point;
    float speed = 0.6f;
    
    auto colorSpriteRight = Sprite::create("colorHRight.png");
	addChild(colorSpriteRight, 10);
    Point endPosition1 = Point(point.x - SIZE_W, point.y);
    colorSpriteRight->setPosition(startPosition);
    colorSpriteRight->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                             MoveTo::create(speed, endPosition1),
                                             CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteRight)),
                                             NULL));
    
    auto colorSpriteLeft = Sprite::create("colorHLeft.png");
	addChild(colorSpriteLeft, 10);
    Point endPosition2 = Point(point.x + SIZE_W, point.y);
    colorSpriteLeft->setPosition(startPosition);
    colorSpriteLeft->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                             MoveTo::create(speed, endPosition2),
                                             CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteLeft)),
                                             NULL));
}

void PlayLayer::explodeSpecialVer(Point point)
{
    float scaleY = 4 ;
    float scaleX = 0.7 ;
    float time = 0.3;
    Point startPosition = point;
    float speed = 0.6f;

    auto colorSpriteDown = Sprite::create("colorVDown.png");
	addChild(colorSpriteDown, 10);
    Point endPosition1 = Point(point.x , point.y - SIZE_H);
    colorSpriteDown->setPosition(startPosition);
    colorSpriteDown->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                             MoveTo::create(speed, endPosition1),
                                             CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteDown)),
                                             NULL));
    
    auto colorSpriteUp = Sprite::create("colorVUp.png");
	addChild(colorSpriteUp, 10);
    Point endPosition2 = Point(point.x , point.y + SIZE_H);
    colorSpriteUp->setPosition(startPosition);
    colorSpriteUp->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                             MoveTo::create(speed, endPosition2),
                                             CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteUp)),
                                             NULL));
}

void PlayLayer::explodeEndCallback(Node *node)
{
    FoodSprite *food = (FoodSprite *)node;
    m_matrix[food->getRow() * m_width + food->getCol()] = NULL;
    food->removeFromParent();
}

void PlayLayer::explodeFood(FoodSprite *food)
{
    float time = 0.3;
    
    // 1. action for food
    food->runAction(Sequence::create( ScaleTo::create(time, 0.0),
                                      CallFuncN::create(CC_CALLBACK_1(PlayLayer::explodeEndCallback, this)),
                                      NULL));
    
    // 2. action for circle
    auto circleSprite = Sprite::create("circle.png");
	addChild(circleSprite, 10);
	circleSprite->setPosition(food->getPosition());
	circleSprite->setScale(0);// start size
    circleSprite->runAction(Sequence::create(ScaleTo::create(time, 1.0),
                                             CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, circleSprite)),
                                             NULL));

    // 3. particle effect
	auto particleStars = ParticleSystemQuad::create("stars.plist");
    particleStars->setAutoRemoveOnFinish(true);
	particleStars->setBlendAdditive(false);
	particleStars->setPosition(food->getPosition());
	particleStars->setScale(0.3);
    addChild(particleStars, 20);
}

//******************************************************************************
// MarkRemove Function
//******************************************************************************
#pragma mark - MarkRemove Function
void PlayLayer::markRemoveHor(FoodSprite *food)
{
    for (int row = 0; row < m_height; row++) {
        FoodSprite *tmp = m_matrix[row * m_width + food->getCol()];
        if (!tmp || tmp == food) {
            continue;
        }
        
        switch (tmp->getFoodState()) {
            case FOOD_STATE_NORMAL:
                tmp->setIsNeedRemove(true);
                break;
                
            default:
                markRemove(tmp);
                break;
        }
    }
}

void PlayLayer::markRemoveVer(FoodSprite *food)
{
    for (int col = 0; col < m_width; col++) {
        FoodSprite *tmp = m_matrix[food->getRow() * m_width + col];
        if (!tmp || tmp == food) {
            continue;
        }
        
        switch (tmp->getFoodState()) {
            case FOOD_STATE_NORMAL:
                tmp->setIsNeedRemove(true);
                break;
                
            default:
                markRemove(tmp);
                break;
        }
    }
}

void PlayLayer::markRemove(FoodSprite *food)
{
    if (food->getIsNeedRemove()) {
        return;
    }
    if (food->getIgnoreCheck()) {
        return;
    }
    
    food->setIsNeedRemove(true);
    
    switch (food->getFoodState())
    {
        case FOOD_STATE_VERTICAL:
            markRemoveVer(food);
            break;
            
        case FOOD_STATE_HORIZONTAL:
            markRemoveHor(food);
            break;
            
        case FOOD_STATE_EXPLODE:
            
        case FOOD_STATE_SAME:
            
        case FOOD_STATE_BRING:
            
        default:
            break;
    }
}

//******************************************************************************
// Fill Function
//******************************************************************************
#pragma mark - Fill Function
void PlayLayer::createAndDropFood(int row, int col)
{
    FoodSprite *food = FoodSprite::create(row, col);
    
    // create animation
    Point endPosition = positionOfItem(row, col);
    Point startPosition = Point(endPosition.x, endPosition.y + SIZE_H / 2);
    food->setPosition(startPosition);
    float speed = startPosition.y / (1.5 * SIZE_H);
    food->runAction(MoveTo::create(speed, endPosition));
    // add to BatchNode
    spriteSheet->addChild(food);

    m_matrix[row * m_width + col] = food;
}

//******************************************************************************
// Mini Function
//******************************************************************************
#pragma mark - Mini Function

Point PlayLayer::positionOfItem(int row, int col)
{
    float x = m_matrixLeftBottomX + (FoodSprite::getContentWidth() + FOOD_GAP) * col + FoodSprite::getContentWidth() / 2;
    float y = m_matrixLeftBottomY + (FoodSprite::getContentWidth() + FOOD_GAP) * row + FoodSprite::getContentWidth() / 2;
    return Point(x, y);
}

FoodSprite *PlayLayer::foodOfPoint(Point *point)
{
    FoodSprite *food = NULL;
    Rect rect = Rect(0, 0, 0, 0);
    
    for (int i = 0; i < m_height * m_width; i++) {
        food = m_matrix[i];
        if (food) {
            rect.origin.x = food->getPositionX() - (food->getContentSize().width / 2);
            rect.origin.y = food->getPositionY() - (food->getContentSize().height / 2);
            rect.size = food->getContentSize();
            if (rect.containsPoint(*point)) {
                return food;
            }
        }
    }
    
    return NULL;
}

//******************************************************************************
// FSM
//******************************************************************************
#pragma mark - FSM
FSM* PlayLayer::getFSM()
{
    return mFSM;
}

void PlayLayer::onReadyState()
{
    CCLOG("ready");
    FoodSprite *food;
    // 1. reset ingnore flag
    for (int i = 0; i < m_height * m_width; i++) {
        food = m_matrix[i];
        if (!food) {
            continue;
        }
        food->setIgnoreCheck(false);
    }
    
    // 2. check chain
    for (int i = 0; i < m_height * m_width; i++)
    {
        food = m_matrix[i];
        if (!food) {
            continue;
        }
        
        if (food->getIsNeedRemove()) {
            continue;// 已标记过的跳过检查
        }
        if (food->getIgnoreCheck()) {
            continue;// 新变化的特殊寿司，不消除
        }
        
        // start count chain
        std::list<FoodSprite *> colChainList;
        getColChain(food, colChainList);
        
        std::list<FoodSprite *> rowChainList;
        getRowChain(food, rowChainList);
        
        std::list<FoodSprite *> &longerList = (colChainList.size() > rowChainList.size() ) ? colChainList : rowChainList;
        if (longerList.size() >= 3)
        {
            m_canRemove = true;
            
            std::list<FoodSprite *>::iterator itList;
            bool isSetedIgnoreCheck = false;
            for (itList = longerList.begin(); itList != longerList.end(); itList++) {
                food = (FoodSprite *)*itList;
                if (!food) {
                    continue;
                }
                
                if (longerList.size() > 3) {
                    // 4消产生特殊寿司
                    if (food == m_srcFood || food == m_destFood) {
                        isSetedIgnoreCheck = true;
                        food->setIgnoreCheck(true);
                        food->setIsNeedRemove(false);
                        food->setFoodState(m_movingVertical ? FOOD_STATE_VERTICAL : FOOD_STATE_HORIZONTAL);
                        continue;
                    }
                }
                
                markRemove(food);
            }
            
            // 如何是自由掉落产生的4消, 取最后一个变化为特殊寿司
            if (!isSetedIgnoreCheck && longerList.size() > 3) {
                food->setIgnoreCheck(true);
                food->setIsNeedRemove(false);
                food->setFoodState(m_movingVertical ? FOOD_STATE_VERTICAL : FOOD_STATE_HORIZONTAL);
            }
        }
    }
}

void PlayLayer::onDropState()
{
    CCLOG("drop");
    // reset moving direction flag
    m_movingVertical = true;
    m_isAnimationing = true;
    
    int *colEmptyInfo = (int *)malloc(sizeof(int) * m_width);
    memset((void *)colEmptyInfo, 0, sizeof(int) * m_width);
    
    // 1. drop exist food
    FoodSprite *food = NULL;
    for (int col = 0; col < m_width; col++) {
        int removedFoodOfCol = 0;
        // from buttom to top
        for (int row = 0; row < m_height; row++) {
            food = m_matrix[row * m_width + col];
            if (NULL == food) {
                removedFoodOfCol++;
            } else {
                if (removedFoodOfCol > 0) {
                    // every item have its own drop distance
                    int newRow = row - removedFoodOfCol;
                    // switch in matrix
                    m_matrix[newRow * m_width + col] = food;
                    m_matrix[row * m_width + col] = NULL;
                    // move to new position
                    Point startPosition = food->getPosition();
                    Point endPosition = positionOfItem(newRow, col);
                    float speed = (startPosition.y - endPosition.y) / SIZE_H;
                    food->stopAllActions();// must stop pre drop action
                    food->runAction(CCMoveTo::create(speed, endPosition));
                    // set the new row to item
                    food->setRow(newRow);
                }
            }
        }
        
        // record empty info
        colEmptyInfo[col] = removedFoodOfCol;
    }
    
    // 2. create new item and drop down.
    for (int col = 0; col < m_width; col++) {
        for (int row = m_height - colEmptyInfo[col]; row < m_height; row++) {
            createAndDropFood(row, col);
        }
    }
    
    free(colEmptyInfo);
    
    m_canDrop = false;
}

void PlayLayer::onRemoveState()
{
    CCLOG("remove");
    
    // make sequence remove
    m_isAnimationing = true;
    
    for (int i = 0; i < m_height * m_width; i++)
    {
        FoodSprite *food = m_matrix[i];
        if (!food) {
            continue;
        }
        
        if (food->getIsNeedRemove()) {
            m_canDrop = true;//需要掉落
            // 检查类型，并播放一行消失的动画
            if(food->getFoodState() == FOOD_STATE_HORIZONTAL)
            {
                explodeSpecialHor(food->getPosition());
            }
            else if (food->getFoodState() == FOOD_STATE_VERTICAL)
            {
                explodeSpecialVer(food->getPosition());
            }
            else if (food->getFoodState() == FOOD_STATE_EXPLODE)
            {
                // 功能待加入
            }
            
            // 减少一条生命
            food->removeOneLife();
            
            if (food->getLife() == 0) {
                explodeFood(food);
            }
        }
    }
    
    m_canRemove = false;
}