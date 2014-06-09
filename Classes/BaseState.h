#ifndef __GameTest__BaseState__
#define __GameTest__BaseState__

#include <iostream>

class PlayLayer;

class BaseState
{
public:
    virtual ~BaseState() = 0;
    
    virtual void execute(PlayLayer *playLayer) = 0;
};

#endif /* defined(__GameTest__BaseState__) */
