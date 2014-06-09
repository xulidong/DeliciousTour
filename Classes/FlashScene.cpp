#include "FlashScene.h"
#include "WelcomeScene.h"

Scene* FlashScene::createScene()
{
    auto scene = Scene::create();
    auto layer = FlashScene::create();
    if (!layer) {
        CCLOG("Scene* FlashScene::createScene():layer == NULL");
    }
    scene->addChild(layer);
    
    return scene;
}

bool FlashScene::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!Layer::init());
        
        Sprite* bg = Sprite::create("background.png");
        CC_BREAK_IF(!bg);
        bg->setPosition(Point(SIZE_W/2,SIZE_H/2));
        addChild(bg);
        
        scheduleOnce(schedule_selector(FlashScene::changeScene), 2.0f);
        
        bRet = true;
    }while(0);
    
    return bRet;
}

void FlashScene::changeScene(float dt)
{
    Director::getInstance()->replaceScene((Scene *)WelcomeScene::createScene());
}