#ifndef __PlayLayer_H__
#define __PlayLayer_H__

#include "cocos2d.h"
#include "CommonHeader.h"
#include "FSM.h"
#include "FoodSprite.h"

class FoodSprite;

class PlayLayer : public LayerColor
{
public:
    PlayLayer();
    ~PlayLayer();
    static PlayLayer* createLayer(int level);
    bool init(int level);
    
    void update(float dt);
    bool onTouchBegan(Touch *touch, Event *unused);
    void onTouchMoved(Touch *touch, Event *unused);
    
    //**************************************************************************
    // onState
    //**************************************************************************
    FSM* getFSM();
    void onStateReady();
    void onStateDrop();
    void onStateRemove();
    
    CC_SYNTHESIZE(bool, m_canRemove, CanRemove);// 是否可以移动
    CC_SYNTHESIZE(int, m_level, Level);         // 当前关卡
    
private:
    void initMatrix();
    
    void createAndDropFood(FoodType type, int row, int col, bool isInit = false);
    void createAndAddTiles(int row, int col);
    
    void getDestFood(Touch *touch);
    void getColChain(FoodSprite *food, std::list<FoodSprite *> &chainList);
    void getRowChain(FoodSprite *food, std::list<FoodSprite *> &chainList);
    
    void explodeEndCallback(Node *node);
    void explodeFood(FoodSprite *food);
    
    // clean line animation
    void explodeSpecialHor(Point point);
    void explodeSpecialVer(Point point);
    
    Point positionOfItem(int row, int col);
    FoodSprite *foodOfPoint(Point *point);
    
    FoodType getFoodTypeRand();
    FoodType getFoodTypeByIndex(int idx = 0);
    
    void swapFood();
    
    /*根据当前食物状态标记将要移除的食物：
     N：本身
     H：水平
     V：竖直
     E：3X3
     */
    void markRemoveHor(FoodSprite *food);
    void markRemoveVer(FoodSprite *food);
    void markRemove(FoodSprite *food);
    
    FSM* mFSM;
    
    SpriteBatchNode *spriteSheet;
    FoodSprite **m_matrix;
    
    int m_width;
    int m_height;
    
    // start of matrix position
    float m_matrixLeftBottomX;
    float m_matrixLeftBottomY;
    
    // use to change food
    FoodSprite *m_srcFood;
    FoodSprite *m_destFood;
    
    bool m_isTouchEnable;// is need to deal with touch event
    bool m_isAnimationing;
    bool m_movingVertical;// true: 4消除产生纵向炸弹.  false: 4消除产生横向炸弹.
    
    Node* m_node;
};

#endif /* defined(__PlayLayer_H__) */
