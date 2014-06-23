/*
 一 位置
 
 二 食物种类
 
 二 状态
 1 横消
 2 竖消
 3 爆炸
 4 同色消
 
 三 生命值
 1 普通为1
 2 被束缚的为2
 3 冰块 N
 4 厨具未-1
 
 *为0的将会被消除
 */

#ifndef __FoodSprite_H__
#define __FoodSprite_H__

#include "cocos2d.h"
#include "BaseFood.h"

USING_NS_CC;

enum class FoodState{
    FOOD_STATE_NONE,
    FOOD_STATE_NORMAL = FOOD_STATE_NONE,
    FOOD_STATE_HORIZONTAL,
    FOOD_STATE_VERTICAL,
    FOOD_STATE_EXPLODE,
    FOOD_STATE_MAX
} ;

class FoodSprite :  public BaseFood
{
public:
    FoodSprite();
    ~FoodSprite();
    bool init(FoodType type,int row, int col);
    static FoodSprite *create(FoodType type,int row, int col);
    
    CC_PROPERTY(FoodState, m_foodState, FoodState);
    
protected:
    void addChildOnFood(std::string filename);
};

#endif
