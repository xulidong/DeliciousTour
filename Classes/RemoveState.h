#ifndef __GameTest__RemoveState__
#define __GameTest__RemoveState__

#include <iostream>

#include "CommonHeader.h"
#include "BaseState.h"

class PlayLayer;

class RemoveState:public BaseState
{
public:
    RemoveState();
    ~RemoveState();
    
    virtual void execute(PlayLayer* playLayer);
    
private:
    
};

#endif 