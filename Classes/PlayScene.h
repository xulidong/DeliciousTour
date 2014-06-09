#ifndef __GameTest__PlayScene__
#define __GameTest__PlayScene__

#include <iostream>
#include "CommonHeader.h"

class PlayLayer;

class PlayScene:public Layer
{
public:
    PlayScene();
    ~PlayScene();
    
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(PlayScene);
    
private:
    PlayLayer* mPlayLayer;
};

#endif
