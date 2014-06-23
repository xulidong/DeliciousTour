#include "FoodSprite.h"

#define SCALE_FOOD 1.0f

FoodSprite::~FoodSprite(){

}

FoodSprite::FoodSprite()
: m_foodState(FoodState::FOOD_STATE_NONE)
{
}


bool FoodSprite::init(FoodType type,int row, int col){
    
    do{
        if (!BaseFood::init(type, row, col)) {
            return false;
        }
        
        m_foodState = FoodState::FOOD_STATE_NORMAL;
        
        return true;
    }while(0);
    
    return false;
}

FoodSprite* FoodSprite::create(FoodType type,int row, int col){

    FoodSprite* food = new FoodSprite();
    if (food && food->init(type, row, col)) {
        food->autorelease();
    }

    return food;
}

void FoodSprite::setFoodState(FoodState state)
{
    m_foodState = state;
    
    switch (state) {
        case FoodState::FOOD_STATE_HORIZONTAL:
        case FoodState::FOOD_STATE_VERTICAL:{
            addChildOnFood("dir_buff0.png");
        }
            break;
        case FoodState::FOOD_STATE_EXPLODE:{
            addChildOnFood("wrap_buff0.png");
        }
            break;
        default:
            return;
    }
}

FoodState FoodSprite::getFoodState(){
    
    return m_foodState;
}

void FoodSprite::addChildOnFood(std::string filename)
{
    auto sprt = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(filename));
    sprt->setPosition(Point(getContentWidth()*0.5,getContentHeight()*0.5));
    addChild(sprt);
}