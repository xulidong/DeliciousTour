#include "MainScene.h"
#include "PlayScene.h"
#include "MapReader.h"
#include "PlayerData.h"

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool MainScene::init()
{
    bool bRet = false;
    
    do
    {
        CC_BREAK_IF(!Layer::init());
        
        scheduleUpdate();
        
        auto labelPlay = Label::createWithSystemFont("Play","Arial",24);
        auto itemPlay = MenuItemLabel::create(labelPlay,CC_CALLBACK_0(MainScene::changeScene, this));
        itemPlay->setPosition(Point(SIZE_W*0.85,SIZE_H*1.5));
        
        auto labelShare = Label::createWithSystemFont(TextManager::getInstance()->getTextByKey(kSHARE),"Arial",24);
        auto itemShare = MenuItemLabel::create(labelShare,CC_CALLBACK_0(MainScene::goShare, this));
        itemShare->setPosition(Point(SIZE_W*0.85,SIZE_H*1.7));
        
        auto menu = CCMenu::create(itemPlay,itemShare, NULL);
        menu->setPosition(Point::ZERO);
        this->addChild(menu);
        
        itemPlay->runAction(EaseBounceOut::create(MoveTo::create(1.0f,Point(SIZE_W*0.85,SIZE_H*0.5))));
        itemShare->runAction( Sequence::create(DelayTime::create(1.2f),
                                               EaseBounceOut::create(MoveTo::create(1.0f,Point(SIZE_W*0.85,SIZE_H*0.7))),
                                               NULL)
                             );
        
        MapReader::getInstance()->readDataOf(1);
        
        m_lifeLbl = Label::createWithSystemFont("Play","Arial",24);
        m_lifeLbl->setPosition(Point(SIZE_W*0.85,SIZE_H*0.8));
        m_lifeLbl->setTextColor(Color4B(255,0,0,255));
        m_lifeLbl->setString("Life");
        addChild(m_lifeLbl);
        
        m_lifeStateLbl = Label::createWithSystemFont("Play","Arial",24);
        m_lifeStateLbl->setPosition(Point(SIZE_W*0.85,SIZE_H*0.9));
        m_lifeStateLbl->setTextColor(Color4B(255,0,0,255));
        m_lifeStateLbl->setString("Time");
        addChild(m_lifeStateLbl);
        
//        auto layermap = MapLayer::create();
//        addChild(layermap);
        
//        auto layershop = ShopLayer::create();
//        addChild(layershop);
        
//        auto layerbag = BagLayer::createLayer();
//        addChild(layerbag);
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan,this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        
        bRet = true;
    }while(0);
    
    return bRet;
}

void MainScene::changeScene()
{
    Director::getInstance()->replaceScene((Scene *)PlayScene::create());
//    LIFE_MANAGER->addLife(-1);
}

void MainScene::goShare()
{

}

void MainScene::update(float fDelta)
{
    int life = LIFE_MANAGER->getLife();
    int time = LIFE_MANAGER->getNextLifeTime();
    string strState  = (time == -1)?FULL_LIFE_TEXT:CommonFunction::num2str(time);
    m_lifeLbl->setString(CommonFunction::num2str(life));
    m_lifeStateLbl->setString(strState);
}

//触摸事件
bool MainScene::onTouchBegan(Touch * touch,Event * pEvent)
{
    //获得触摸点坐标
    Point touchLocation = touch->getLocation();

    ParticleSystemQuad *particle =  ParticleSystemQuad::create("showClick.plist");
    particle->setScale(0.5f);
    particle->setPosition(touchLocation);
    //如果不设置,粒子播放后内存不释放
    particle->setAutoRemoveOnFinish(true);
    addChild(particle);

    return false;
}
