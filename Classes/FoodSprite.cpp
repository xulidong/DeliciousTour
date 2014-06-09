#include "FoodSprite.h"

USING_NS_CC;

#define TOTAL_FOOD (6)

#define SCALE_FOOD 0.6f

static const char *foodNormal[TOTAL_FOOD] = {
	"sushi_1n.png",
	"sushi_2n.png",
	"sushi_3n.png",
	"sushi_4n.png",
	"sushi_5n.png",
    "sushi_6n.png"
};

static const char *foodVertical[TOTAL_FOOD] = {
	"sushi_1v.png",
	"sushi_2v.png",
	"sushi_3v.png",
	"sushi_4v.png",
	"sushi_5v.png",
    "sushi_6v.png"
};

static const char *foodHorizontal[TOTAL_FOOD] = {
	"sushi_1h.png",
	"sushi_2h.png",
	"sushi_3h.png",
	"sushi_4h.png",
	"sushi_5h.png",
    "sushi_6h.png"
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

FoodSprite::FoodSprite()
: m_col(0)
, m_row(0)
, m_imgIndex(0)
, m_life(1)
, m_isNeedRemove(false)
, m_ignoreCheck(false)
, m_foodState(FOOD_STATE_NONE)
{
}

FoodSprite *FoodSprite::create(int row, int col)
{
	FoodSprite *food = new FoodSprite();
    food->setScale(SCALE_FOOD);
	food->m_row = row;
	food->m_col = col;
    food->m_imgIndex = arc4random() % TOTAL_FOOD;
    food->initWithSpriteFrameName(foodNormal[food->m_imgIndex]);
	food->autorelease();
	return food;
}

void FoodSprite::setFoodState(FoodState state)
{
    m_foodState = state;
    
    SpriteFrame *frame = NULL;
    switch (state) {
        case FOOD_STATE_HORIZONTAL:
            frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(foodHorizontal[m_imgIndex]);
            break;
        case FOOD_STATE_VERTICAL:
            frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(foodVertical[m_imgIndex]);
            break;
        case FOOD_STATE_EXPLODE:
            // 缺少图片
        default:
            return;
    }
    
    setDisplayFrame(frame);
}

void FoodSprite::removeOneLife(int count)
{
    if (m_life>0) {
        m_life -= count;
    }
}