#include "WelcomeScene.h"
#include "CommonHeader.h"
#include "MainScene.h"

WelcomeScene::WelcomeScene()
{
    
}

WelcomeScene::~WelcomeScene()
{
    
}

Scene* WelcomeScene::createScene()
{
    auto scene = Scene::create();
    auto layer = WelcomeScene::create();
    scene->addChild(layer);
    return scene;
}

bool WelcomeScene::init()
{
    bool bRet = false;
    do{
        CC_BREAK_IF(!Layer::init());
        initData();
        
        auto bg = Sprite::create("welcomebg.png");
        bg->setPosition(SIZE_MID_POS);
        addChild(bg);
        
        auto labelPlay = Label::createWithSystemFont("Play","Arial",48);
        auto itemPlay = MenuItemLabel::create(labelPlay,CC_CALLBACK_0(WelcomeScene::changeScene, this));
        itemPlay->setPosition(Point(SIZE_W*0.5,SIZE_H*0.1));
        
        auto menu = CCMenu::create(itemPlay, NULL);
        menu->setPosition(Point::ZERO);
        this->addChild(menu);
        
        bRet = true;
    }while(0);
    
    return true;
}

#pragma mark - initData
void WelcomeScene::initData()
{
    
}

void WelcomeScene::changeScene()
{
    Director::getInstance()->replaceScene((Scene *)MainScene::createScene());
}