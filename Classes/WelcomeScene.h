#ifndef __foodtrip__WelcomeScene__
#define __foodtrip__WelcomeScene__

#include "cocos2d.h"

USING_NS_CC;

class WelcomeScene : public Layer
{
public:
    WelcomeScene();
    ~WelcomeScene();
    static Scene* createScene();
    
    virtual bool init();
    
protected:
    CREATE_FUNC(WelcomeScene);
    void initData();
    
private:
    void changeScene();
};

#endif 
