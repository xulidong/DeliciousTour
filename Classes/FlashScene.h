#ifndef __GameTest__FlashScene__
#define __GameTest__FlashScene__

#include <iostream>
#include "CommonHeader.h"


class FlashScene:public Layer
{
public:
    static Scene* createScene();
    virtual bool init();
    
private:
    CREATE_FUNC(FlashScene);
    void changeScene(float dt);
};

#endif
