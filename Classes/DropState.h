#ifndef __GameTest__DropState__
#define __GameTest__DropState__

#include <iostream>
#include "CommonHeader.h"
#include "BaseState.h"

class PlayLayer;

class DropState:public BaseState
{
public:
    DropState();
    ~DropState();
    
    virtual void execute(PlayLayer* playLayer);
    
private:
    
};
#endif
