#include "PlayScene.h"
#include "PlayLayer.h"

PlayScene::PlayScene()
{

}

PlayScene::~PlayScene()
{
    CC_SAFE_DELETE(mPlayLayer);
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
        mPlayLayer = PlayLayer::createLayer();
        mPlayLayer->retain();
        addChild(mPlayLayer);
    }while(0);
    
    return true;
}