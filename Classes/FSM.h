#ifndef __GameTest__FSM__
#define __GameTest__FSM__

#include <iostream>
#include "CommonHeader.h"
#include "BaseState.h"
#include "DropState.h"
#include "ReadyState.h"
#include "RemoveState.h"
#include "PreOverState.h"
#include "WinState.h"
#include "LostState.h"

/*
 游戏界面初始化完成后启动状态机，控制游戏状态在以下几种之间切换：
 1 就绪状态
 2 消除状态
 3 下落状态
 4
 */

class PlayLayer;

class FSM:public Node
{
public:
    FSM();
    ~FSM();
    
    static FSM* createFSM(PlayLayer* playLayer);
    virtual bool init(PlayLayer* playLayer);
    
    void update(float dt);
    
    void changeState(BaseState * state);
    
private:
    BaseState* mCurState;
    PlayLayer* mPlayLayer;
};

#endif
