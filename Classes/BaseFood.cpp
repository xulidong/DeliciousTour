#include "BaseFood.h"

#define TOTAL_FOOD (6)

#define SCALE_FOOD 1.0f

static const std::string foodNormal[TOTAL_FOOD] = {
	"Blue_Vert0.png",
	"Green_Vert0.png",
	"Orange_Vert0.png",
	"Pink_Vert0.png",
	"Purple_Vert0.png",
    "Yellow_Vert0.png"
};

float BaseFood::getContentWidth()
{
    static float itemWidth = 0;
    if (0 == itemWidth) {
        Sprite *sprite = CCSprite::createWithSpriteFrameName(foodNormal[0]);
        itemWidth = sprite->getContentSize().width;
    }
    return itemWidth*SCALE_FOOD;
}

float BaseFood::getContentHeight()
{
    static float itemHeight = 0;
    if (0 == itemHeight) {
        Sprite *sprite = CCSprite::createWithSpriteFrameName(foodNormal[0]);
        itemHeight = sprite->getContentSize().height;
    }
    return itemHeight*SCALE_FOOD;
}

BaseFood::~BaseFood(){

}

BaseFood::BaseFood()
: m_col(0)
, m_row(0)
, m_foodType(FoodType::FOOD_TYPE_NONE)
, m_life(1)
, m_isNeedRemove(false)
, m_ignoreCheck(false)
{
}

bool BaseFood::init(FoodType type,int row, int col){
    
    do{
        CC_BREAK_IF(!Sprite::init());
        setScale(SCALE_FOOD);
        m_row = row;
        m_col = col;
        m_foodType = type;
        std::string file = foodNormal[(int)m_foodType];
        initWithSpriteFrameName(file);
        
        return true;
    }while(0);
    
    return false;
}

BaseFood *BaseFood::create(FoodType type, int row, int col)
{
	BaseFood *food = new BaseFood();
    if (food && food->init(type, row, col)) {
        food->autorelease();
    }
    
	return food;
}

void BaseFood::setFoodType(FoodType type)
{
    m_foodType = type;
    
    switch (type) {
        case FoodType::FOOD_TYPE_SAME:
            setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("swirl_0.png"));
            break;
        default:
            return;
    }
}

FoodType BaseFood::getFoodType(){

    return m_foodType;
}

void BaseFood::removeOneLife(int count)
{
    if (m_life>0) {
        m_life -= count;
    }
}