#include "FoodSprite.h"

USING_NS_CC;

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

float FoodSprite::getContentWidth()
{
    static float itemWidth = 0;
    if (0 == itemWidth) {
        Sprite *sprite = CCSprite::createWithSpriteFrameName(foodNormal[0]);
        itemWidth = sprite->getContentSize().width;
    }
    return itemWidth*SCALE_FOOD;
}

float FoodSprite::getContentHeight()
{
    static float itemHeight = 0;
    if (0 == itemHeight) {
        Sprite *sprite = CCSprite::createWithSpriteFrameName(foodNormal[0]);
        itemHeight = sprite->getContentSize().height;
    }
    return itemHeight*SCALE_FOOD;
}

FoodSprite::FoodSprite()
: m_col(0)
, m_row(0)
, m_foodType(FoodType::FOOD_TYPE_NONE)
, m_life(1)
, m_isNeedRemove(false)
, m_ignoreCheck(false)
, m_foodState(FoodState::FOOD_STATE_NONE)
{
}

FoodSprite *FoodSprite::create(FoodType type, int row, int col)
{
	FoodSprite *food = new FoodSprite();
    food->setScale(SCALE_FOOD);
	food->m_row = row;
	food->m_col = col;
    food->m_foodType = type;
    std::string file = foodNormal[(int)food->m_foodType];
    food->initWithSpriteFrameName(file);
	food->autorelease();
    
	return food;
}

void FoodSprite::setFoodState(FoodState state)
{
    m_foodState = state;
    
//    SpriteFrame *frame = NULL;
    switch (state) {
        case FoodState::FOOD_STATE_HORIZONTAL:
            
            break;
        case FoodState::FOOD_STATE_VERTICAL:
//            frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(foodVertical[(int)m_foodType]);
            break;
        case FoodState::FOOD_STATE_SAME:
            
        case FoodState::FOOD_STATE_EXPLODE:
            // 缺少图片
        default:
            return;
    }
    
//    setDisplayFrame(frame);
}

void FoodSprite::removeOneLife(int count)
{
    if (m_life>0) {
        m_life -= count;
    }
}