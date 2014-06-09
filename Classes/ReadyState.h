#ifndef __GameTest__ReadyState__
#define __GameTest__ReadyState__

#include <iostream>
#include "CommonHeader.h"
#include "BaseState.h"

class PlayLayer;

class ReadyState:public BaseState
{
public:
    ReadyState();
    ~ReadyState();
    
    virtual void execute(PlayLayer* playLayer);
    
private:
    
};
#endif
