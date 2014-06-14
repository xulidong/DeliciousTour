#include "PlayScene.h"
#include "MainScene.h"
#include "PlayLayer.h"

PlayScene::PlayScene()
{

}

PlayScene::~PlayScene()
{
    
}

Scene* PlayScene::createScene()
{
    auto scene = Scene::create();
    auto layer = PlayScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool PlayScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    do
    {
        auto background = Sprite::create("background.png");
        background->setAnchorPoint(Point(0, 1));
        background->setPosition(Point(0, SIZE_H));
        addChild(background);
        
        mPlayLayer = PlayLayer::createLayer(1);
        addChild(mPlayLayer);
        
        auto labelPlay = Label::createWithSystemFont("Back","Arial",48);
        auto itemPlay = MenuItemLabel::create(labelPlay,CC_CALLBACK_0(PlayScene::changeScene, this));
        itemPlay->setPosition(Point(SIZE_W*0.85,SIZE_H*0.85));
        
        auto menu = CCMenu::create(itemPlay, NULL);
        menu->setPosition(Point::ZERO);
        this->addChild(menu);
        
    }while(0);
    
    return true;
}

void PlayScene::changeScene()
{
    Director::getInstance()->replaceScene((Scene *)MainScene::createScene());
    //    LIFE_MANAGER->addLife(-1);
}