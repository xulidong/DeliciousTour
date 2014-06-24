#include "PlayLayer.h"
#include "MapReader.h"
#include "FoodSprite.h"

#define FOOD_GAP                (1)
#define FULL_DROP_TIME          (4)
#define DROP_TIME(distance)     (FULL_DROP_TIME*(distance)/SIZE_H)

PlayLayer::PlayLayer()
: spriteSheet(NULL)
, m_matrix(NULL)
, m_width(0)
, m_height(0)
, m_matrixLeftBottomX(0)
, m_matrixLeftBottomY(0)
, m_canRemove(false)
, m_isAnimationing(true)//start with drop animation
, m_isTouchEnable(true)
, m_srcFood(NULL)
, m_destFood(NULL)
, m_movingVertical(true)//drop animation is vertical
, m_isSwaped(false)
{
    
}

PlayLayer::~PlayLayer()
{
    CC_SAFE_FREE(m_matrix);
    CC_SAFE_DELETE(mFSM);
}

PlayLayer* PlayLayer::createLayer(int level)
{
    auto layer = new PlayLayer();
    if (layer && layer->init(level)) {
        layer->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(layer);
    }
    return layer;
}

bool PlayLayer::init(int level)
{
    if (!LayerColor::initWithColor(Color4B(255, 255, 255,0))) {
        return false;
    }
    
    m_level = level;
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("monsters.plist", "monsters.png");
    
    spriteSheet = SpriteBatchNode::create("monsters.png");
    addChild(spriteSheet);
    
    MAP_INSTANCE->readDataOf(m_level);
    m_width = MAP_INSTANCE->getWidth();
    m_height = MAP_INSTANCE->getHeight();
    
    m_matrixLeftBottomX = (SIZE_W - FoodSprite::getContentWidth() * m_width - (m_width - 1) * FOOD_GAP) / 2;
    m_matrixLeftBottomY = (SIZE_H - FoodSprite::getContentWidth() * m_height - (m_height - 1) * FOOD_GAP) / 2;
    
    int arraySize = sizeof(FoodSprite *) * m_width * m_height;
    m_matrix = (FoodSprite **)malloc(arraySize);
    memset((void*)m_matrix, 0, arraySize);
    
    // add mask
    auto clip = ClippingNode::create();
    clip->setInverted(true);
    clip->setAlphaThreshold(1);
    this->addChild(clip);
    
    auto back = Sprite::create("background.png");//LayerColor::create(Color4B(128,128,128,255));
    back->ignoreAnchorPointForPosition(false);
    back->setAnchorPoint(Point(0, 1));
    back->setPosition(Point(0, SIZE_H));
    clip->addChild(back);
    
    m_node = Node::create();
    clip->setStencil(m_node);
    
    // must initMatrix before start FSM
    initMatrix();
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
    srand(TimeManager::GetSysTimeUSec());
    for (int row = 0; row < m_height; row++) {
		for (int col = 0; col < m_width; col++) {
            createAndAddTiles(row, col);
            int idx = row * m_width + col;
            createAndDropFood(getFoodTypeByIndex(idx) ,row, col, row);
        }
    }
}

//******************************************************************************
// override Function
//******************************************************************************
#pragma mark - Override Function

bool PlayLayer::onTouchBegan(Touch *touch, Event *unused)
{
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
    
    getDestFood(touch);
    swapFood();
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
#pragma mark - Touch Function
bool PlayLayer::isEnableSwap()
{
    std::list<FoodSprite *> colChainListOfFirst;
    getColChain(m_srcFood, colChainListOfFirst);
    
    std::list<FoodSprite *> rowChainListOfFirst;
    getRowChain(m_srcFood, rowChainListOfFirst);
    
    std::list<FoodSprite *> colChainListOfSecond;
    getColChain(m_destFood, colChainListOfSecond);
    
    std::list<FoodSprite *> rowChainListOfSecond;
    getRowChain(m_destFood, rowChainListOfSecond);
    
    
    // same
    if(m_srcFood->getFoodType() == FoodType::FOOD_TYPE_SAME ||
       m_destFood->getFoodType() == FoodType::FOOD_TYPE_SAME )
    {
        return true;
    }
    // normal
    else if ( colChainListOfFirst.size() >= 3 || rowChainListOfFirst.size() >= 3 ||
             colChainListOfSecond.size() >= 3 || rowChainListOfSecond.size() >= 3) {
        return true;
    }
    
    return false;
}

void PlayLayer::swapFood()
{
    m_isAnimationing = true;
    m_isTouchEnable = false;
    if (!m_srcFood || !m_destFood) {
        m_movingVertical = true;
        return;
    }
    
    // 1.swap in matrix
    m_matrix[m_srcFood->getRow() * m_width + m_srcFood->getCol()] = m_destFood;
    m_matrix[m_destFood->getRow() * m_width + m_destFood->getCol()] = m_srcFood;
    int tmpRow = m_srcFood->getRow();
    int tmpCol = m_srcFood->getCol();
    m_srcFood->setRow(m_destFood->getRow());
    m_srcFood->setCol(m_destFood->getCol());
    m_destFood->setRow(tmpRow);
    m_destFood->setCol(tmpCol);
    
    // 2.swap animation
    float time = 0.2;
    Point posOfSrc = m_srcFood->getPosition();
    Point posOfDest = m_destFood->getPosition();
    
    // just swap
    if(isEnableSwap()){
        m_srcFood->runAction(MoveTo::create(time, posOfDest));
        m_destFood->runAction(MoveTo::create(time, posOfSrc));
        
        m_isSwaped = true;
    }
    // swap and back
    else{
        // swap back in matrix
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
        m_srcFood = NULL;
        m_destFood = NULL;
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
            && (neighborFood->getFoodType() == food->getFoodType())
            && !neighborFood->getIsNeedRemove()
            && !neighborFood->getIgnoreCheck()) {
            chainList.push_back(neighborFood);
            neighborCol--;
        }
        else {
            break;
        }
    }
    
    neighborCol = food->getCol() + 1;
    while (neighborCol < m_width) {
        FoodSprite *neighborFood = m_matrix[food->getRow() * m_width + neighborCol];
        if (neighborFood
            && (neighborFood->getFoodType() == food->getFoodType())
            && !neighborFood->getIsNeedRemove()
            && !neighborFood->getIgnoreCheck()) {
            chainList.push_back(neighborFood);
            neighborCol++;
        }
        else {
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
            && (neighborFood->getFoodType() == food->getFoodType())
            && !neighborFood->getIsNeedRemove()
            && !neighborFood->getIgnoreCheck()) {
            chainList.push_back(neighborFood);
            neighborRow--;
        }
        else {
            break;
        }
    }
    
    neighborRow = food->getRow() + 1;
    while (neighborRow < m_height) {
        FoodSprite *neighborFood = m_matrix[neighborRow * m_width + food->getCol()];
        if (neighborFood
            && (neighborFood->getFoodType() == food->getFoodType())
            && !neighborFood->getIsNeedRemove()
            && !neighborFood->getIgnoreCheck()) {
            chainList.push_back(neighborFood);
            neighborRow++;
        }
        else {
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
void PlayLayer::markList(std::list<FoodSprite *> &chainList){
    
    std::list<FoodSprite *>::iterator itList;
    for (itList = chainList.begin(); itList != chainList.end(); ++itList){
        FoodSprite* tmpFood = (FoodSprite *)*itList;
        if (tmpFood) {
            markRemoveNormal(tmpFood);
        }
    }
}

void PlayLayer::markRemoveHor(FoodSprite *food)
{
    for (int row = 0; row < m_height; row++) {
        FoodSprite *tmp = m_matrix[row * m_width + food->getCol()];
        if (!tmp || tmp == food) {
            continue;
        }
        
        switch (tmp->getFoodState()) {
            case FoodState::FOOD_STATE_NORMAL:
                tmp->setIsNeedRemove(true);
                break;
                
            default:
                markRemoveNormal(tmp);
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
            case FoodState::FOOD_STATE_NORMAL:
                tmp->setIsNeedRemove(true);
                break;
                
            default:
                markRemoveNormal(tmp);
                break;
        }
    }
}

void PlayLayer::markRemoveSame(FoodSprite *food){
    
    if (food == NULL) {
        return;
    }
    
    markRemoveNormal(food);
    
    for (int i = 0; i < m_height * m_width; i++)
    {
        FoodSprite *tmpFood = m_matrix[i];
        if (tmpFood && (tmpFood->getFoodType() == food->getFoodType()) &&
            !tmpFood->getIsNeedRemove() && !tmpFood->getIgnoreCheck())
        {
            switch (tmpFood->getFoodState()) {
                case FoodState::FOOD_STATE_NORMAL:
                    tmpFood->setIsNeedRemove(true);
                    break;
                    
                default:
                    markRemoveNormal(tmpFood);
                    break;
            }// end switch
        }// end if
    }// end for
}

void PlayLayer::markRemoveExplode(FoodSprite *food){

    int row_min = MAX(food->getRow()-1, 0);
    int col_min = MAX(food->getCol()-1, 0);
    int row_max = MIN(food->getRow()+1, m_height);
    int col_max = MIN(food->getCol()+1, m_width);
    
    for (int row = row_min; row <= row_max; ++row) {
        for (int col = col_min; col <= col_max; ++col) {
            FoodSprite *tmp = m_matrix[row * m_width + col];
            if (!tmp || tmp == food) {
                continue;
            }
            switch (tmp->getFoodState()) {
                case FoodState::FOOD_STATE_NORMAL:
                    tmp->setIsNeedRemove(true);
                    break;
                    
                default:
                    markRemoveNormal(tmp);
                    break;
            }
        }
    }
}

void PlayLayer::markRemoveNormal(FoodSprite *food)
{
    if (food->getIsNeedRemove()) {
        return;
    }
    if (food->getIgnoreCheck()) {
        return;
    }
    
    food->setIsNeedRemove(true);
    
    switch (food->getFoodState()){
        case FoodState::FOOD_STATE_VERTICAL:
            markRemoveVer(food);
            break;
        case FoodState::FOOD_STATE_HORIZONTAL:
            markRemoveHor(food);
            break;
        case FoodState::FOOD_STATE_EXPLODE:
            markRemoveExplode(food);
            break;
        default:
            break;
    }
}

bool PlayLayer::swapRemoveMark()
{
    
    if (m_srcFood == NULL || m_destFood == NULL) {
        return false;
    }
    
    bool ret = false;
    
    // same
    if(m_srcFood->getFoodType() == FoodType::FOOD_TYPE_SAME ||
       m_destFood->getFoodType() == FoodType::FOOD_TYPE_SAME )
    {
        if (m_srcFood->getFoodType() == FoodType::FOOD_TYPE_SAME) {
            m_srcFood->setIsNeedRemove(true);
            markRemoveSame(m_destFood);
        }else{
            m_destFood->setIsNeedRemove(true);
            markRemoveSame(m_srcFood);
        }
        
        ret = true;
    }
    
    // normal
    
    // m_srcFood
    std::list<FoodSprite *> colSrcList;
    getColChain(m_srcFood, colSrcList);
    if (colSrcList.size() >= 3) {
        markList(colSrcList);
        ret = true;
    }
    
    std::list<FoodSprite *> rowSrcList;
    getRowChain(m_srcFood, rowSrcList);
    if (rowSrcList.size() >= 3) {
        markList(rowSrcList);
        ret = true;
    }
    
    createSpecialFood(m_srcFood, colSrcList, rowSrcList);
    
    // m_destFood
    std::list<FoodSprite *> colDestList;
    getColChain(m_destFood, colDestList);
    if (colDestList.size() >= 3) {
        markList(colDestList);
        ret = true;
    }
    
    std::list<FoodSprite *> rowDestList;
    getRowChain(m_destFood, rowDestList);
    if (rowDestList.size() >= 3) {
        markList(rowDestList);
        ret = true;
    }
    
    createSpecialFood(m_destFood, colDestList, rowDestList);
    
    return ret;
}

bool PlayLayer::dropRemoveMark(){
    bool ret = false;
    
    FoodSprite* food;
    for (int i = 0; i < m_height * m_width; i++)
    {
        food = m_matrix[i];
        if ( !food || food->getIsNeedRemove() || food->getIgnoreCheck() ) {
            continue;
        }
        
        // start count chain
        std::list<FoodSprite *> colChainList;
        getColChain(food, colChainList);
        
        std::list<FoodSprite *> rowChainList;
        getRowChain(food, rowChainList);
        
        std::list<FoodSprite *> &longerList = (colChainList.size() > rowChainList.size() ) ? colChainList : rowChainList;
        if (longerList.size() >= 3){
            markList(longerList);
            ret = true;
        }
    }
    return ret;
}

//******************************************************************************
// Fill Function
//******************************************************************************
#pragma mark - Fill Function
void PlayLayer::createAndDropFood(FoodType type, int row, int col, int idx)
{
    FoodSprite *food = FoodSprite::create(type, row, col);
    Point endPosition = positionOfItem(row, col);
    Point startPosition = positionOfItem(m_height, col);
    food->setPosition(startPosition);
    spriteSheet->addChild(food);// add to BatchNode
    
    float dt = DROP_TIME(startPosition.y - endPosition.y);
    auto move = MoveTo::create(dt, endPosition);
    
    float time = idx * DROP_TIME(FoodSprite::getContentWidth() + FOOD_GAP);
    auto delay = DelayTime::create(time);
    
    auto seq = Sequence::create(delay, move, NULL);
    food->runAction(seq);

    m_matrix[row * m_width + col] = food;
}

void PlayLayer::createAndAddTiles(int row, int col){
    
    auto tile = Sprite::create("black.png");
    tile->setScale(8.0);
    tile->setPosition(positionOfItem(row, col));
    m_node->addChild(tile);
    
    auto tile2 = Sprite::create("black.png");
    tile2->setScale(6.50f);
    tile2->setPosition(positionOfItem(row, col));
    addChild(tile2,-1);
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

void PlayLayer::getDestFood(Touch *touch)
{
    int row = m_srcFood->getRow();
    int col = m_srcFood->getCol();
    
    auto location = touch->getLocation();
    auto halfFoodWidth = m_srcFood->getContentSize().width / 2;
    auto halfFoodHeight = m_srcFood->getContentSize().height / 2;
    
    // check four dircions up, down, left and right, get the m_dstFood
    auto  upRect = Rect(m_srcFood->getPositionX() - halfFoodWidth,
                        m_srcFood->getPositionY() + halfFoodHeight,
                        m_srcFood->getContentSize().width,
                        m_srcFood->getContentSize().height);
    if (upRect.containsPoint(location)) {
        row++;
        if (row < m_height) {
            m_destFood = m_matrix[row * m_width + col];
        }
        m_movingVertical = true;
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
        return;
    }
}

FoodType PlayLayer::getFoodTypeRand()
{
    return (FoodType)CommonFunction::getRandNum((int)FoodType::FOOD_TYPE_1,
                                                (int)FoodType::FOOD_TYPE_6);
}

FoodType PlayLayer::getFoodTypeByIndex(int idx)
{
    FoodType type = FoodType::FOOD_TYPE_NONE;
    if (MAP_INSTANCE->m_vFood[idx] == 0) {
        type = getFoodTypeRand();
    }else{
        type = (FoodType)MAP_INSTANCE->m_vFood[idx];
    }
    
    return type;
}

//******************************************************************************
// FSM
//******************************************************************************
#pragma mark - FSM
FSM* PlayLayer::getFSM()
{
    return mFSM;
}
/*
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
 std::list<FoodSprite *>::iterator itList;
 bool isSetedIgnoreCheck = false;
 for (itList = longerList.begin(); itList != longerList.end(); itList++)
 {
 food = (FoodSprite *)*itList;
 if (!food) {
 continue;
 }
 
 // 1 create boom food
 if ( colChainList.size() >= 3 && rowChainList.size() >= 3 ){
 if (food == m_srcFood || food == m_destFood) {
 isSetedIgnoreCheck = true;
 food->setIgnoreCheck(true);
 food->setIsNeedRemove(false);
 food->setFoodState(FoodState::FOOD_STATE_EXPLODE);
 continue;
 }
 }
 // 2 create line food
 else if (longerList.size() == 4) {
 
 if (food == m_srcFood || food == m_destFood) {
 isSetedIgnoreCheck = true;
 food->setIgnoreCheck(true);
 food->setIsNeedRemove(false);
 food->setFoodState(m_movingVertical ?
 FoodState::FOOD_STATE_VERTICAL :
 FoodState::FOOD_STATE_HORIZONTAL);
 continue;
 }
 }
 // 3 create same color food
 else if (longerList.size() == 5) {
 if (food == m_srcFood || food == m_destFood) {
 isSetedIgnoreCheck = true;
 food->setIgnoreCheck(true);
 food->setIsNeedRemove(false);
 food->setFoodType(FoodType::FOOD_TYPE_SAME);
 continue;
 }
 }
 
 markRemoveNormal(food);
 }
 }
 m_canRemove = true;
 }
 
 */
bool PlayLayer::createSpecialFood(FoodSprite *food, std::list<FoodSprite *> &colList, std::list<FoodSprite *> &rowList)
{
    // create explode food
    if (colList.size() >= 3 && rowList.size() >= 3) {
        if (food == m_srcFood || food == m_destFood) {
            food->setIgnoreCheck(true);
            food->setIsNeedRemove(false);
            food->setFoodState(FoodState::FOOD_STATE_EXPLODE);
            return true;
        }
    }
    
    // create same food
    if (colList.size() == 5 || rowList.size() == 5) {
        if (food == m_srcFood || food == m_destFood) {
            food->setIgnoreCheck(true);
            food->setIsNeedRemove(false);
            food->setFoodType(FoodType::FOOD_TYPE_SAME);
        }
    }
    
    // create line food
    if (colList.size() == 4 || rowList.size() == 4) {
        if (food == m_srcFood || food == m_destFood) {
            food->setIgnoreCheck(true);
            food->setIsNeedRemove(false);
            food->setFoodState(m_movingVertical ?
                               FoodState::FOOD_STATE_VERTICAL :
                               FoodState::FOOD_STATE_HORIZONTAL);
        }
    }
    
    return false;
}

void PlayLayer::onStateReady()
{
    CCLOG("ready");

    // 1. check remove
    if (m_isSwaped) {
        if (swapRemoveMark()) {
            m_canRemove = true;
        }
    }
    else{
        if (dropRemoveMark()) {
            m_canRemove = true;
        }
    }
}

void PlayLayer::onStateDrop()
{
    CCLOG("drop");
    
    // reset moving direction flag
    m_movingVertical = true;
    m_isAnimationing = true;
    
    int *colEmptyInfo = (int *)malloc(sizeof(int) * m_width);
    memset((void *)colEmptyInfo, 0, sizeof(int) * m_width);
    
    // 1. drop exist food
    FoodSprite *food = NULL;
    for (int col = 0; col < m_width; col++)
    {
        int removedFoodOfCol = 0;
        // from bottom to top
        for (int row = 0; row < m_height; row++)
        {
            food = m_matrix[row * m_width + col];
            if (NULL == food) {
                removedFoodOfCol++;
            }
            else
            {
                if (removedFoodOfCol > 0) {
                    // every item have its own drop distance
                    int newRow = row - removedFoodOfCol;
                    
                    // switch in matrix
                    m_matrix[newRow * m_width + col] = food;
                    m_matrix[row * m_width + col] = NULL;
                    
                    // move to new position
                    Point startPosition = food->getPosition();
                    Point endPosition = positionOfItem(newRow, col);
                    float dt = DROP_TIME(startPosition.y - endPosition.y);
                    food->stopAllActions();
                    food->runAction(MoveTo::create(dt, endPosition));
                    
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
        int idx = 0;
        for (int row = m_height - colEmptyInfo[col]; row < m_height; row++) {
            createAndDropFood(getFoodTypeRand(),row, col, idx);
            ++idx;
        }
    }
    
    free(colEmptyInfo);
}

void PlayLayer::onStateRemove()
{
    CCLOG("remove");
    
    m_isAnimationing = true;
    
    for (int i = 0; i < m_height * m_width; i++)
    {
        FoodSprite *food = m_matrix[i];
        if (!food) {
            continue;
        }
        
        if (food->getIsNeedRemove()) {
            
            // 1. line
            if(food->getFoodState() == FoodState::FOOD_STATE_HORIZONTAL)
            {
                explodeSpecialHor(food->getPosition());
            }
            else if (food->getFoodState() == FoodState::FOOD_STATE_VERTICAL)
            {
                explodeSpecialVer(food->getPosition());
            }
            
            // 2. self
            food->removeOneLife();
            if (food->getLife() == 0) {
                explodeFood(food);
            }
        }
    }
    
    // reset ingnore flag
    FoodSprite *food=NULL;
    for (int i = 0; i < m_height * m_width; i++) {
        food = m_matrix[i];
        if (!food) {
            continue;
        }
        food->setIgnoreCheck(false);
    }
    
    m_canRemove = false;
    m_isSwaped = false;
    m_srcFood = NULL;
    m_destFood = NULL;
}