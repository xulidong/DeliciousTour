#ifndef __PlayLayer_H__
#define __PlayLayer_H__

#include "cocos2d.h"
#include "CommonHeader.h"
#include "FSM.h"

class FoodSprite;

class PlayLayer : public Layer
{
public:
    PlayLayer();
    ~PlayLayer();
    static PlayLayer* createLayer();
    CREATE_FUNC(PlayLayer);
    
    virtual bool init() override;
    virtual void update(float dt) override;
    virtual bool onTouchBegan(Touch *touch, Event *unused) override;
    virtual void onTouchMoved(Touch *touch, Event *unused) override;
    
    //**************************************************************************
    // onState
    //**************************************************************************
    FSM* getFSM();
    void onReadyState();
    CC_SYNTHESIZE(bool, m_canDrop, CanDrop);// 是否有空缺需要填补
    void onDropState();
    CC_SYNTHESIZE(bool, m_canRemove, CanRemove);// 是否有空缺需要填补
    void onRemoveState();
    
private:
    FSM* mFSM;
    
    SpriteBatchNode *spriteSheet;
    FoodSprite **m_matrix;
    int m_width;
    int m_height;
    
    float m_matrixLeftBottomX;
    float m_matrixLeftBottomY;
    
    FoodSprite *m_srcFood;
    FoodSprite *m_destFood;
    
    bool m_isTouchEnable;// is need to deal with touch event
    bool m_isAnimationing;
    bool m_movingVertical;// true: 4消除产生纵向炸弹.  false: 4消除产生横向炸弹.
    
    Point positionOfItem(int row, int col);
    FoodSprite *foodOfPoint(Point *point);
    
    void initMatrix();
    void createAndDropFood(int row, int col);
    void getColChain(FoodSprite *food, std::list<FoodSprite *> &chainList);
    void getRowChain(FoodSprite *food, std::list<FoodSprite *> &chainList);
    void explodeEndCallback(Node *node);
    void explodeFood(FoodSprite *food);
    
    // line消除的动画
    void explodeSpecialHor(Point point);
    void explodeSpecialVer(Point point);
    
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
};

#endif /* defined(__PlayLayer_H__) */
