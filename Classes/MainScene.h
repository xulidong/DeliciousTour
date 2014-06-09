#ifndef __GameTest__MainScene__
#define __GameTest__MainScene__

#include <iostream>
#include "CommonHeader.h"

/*
 1 地图
 2 商店
 3 背包
 */

class MainScene:public Layer
{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MainScene);
    
    virtual void update(float fDelta);
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
private:
    void changeScene();
    void goShare();
    
    Label* m_lifeLbl;
    Label* m_lifeStateLbl;
};

#endif 
