#include "AppDelegate.h"
#include "FlashScene.h"
#include "PlayerData.h"

USING_NS_CC;

#define RW (640.0f)
#define RH (1136.0f)

#define DW (640.0f)
#define DH (1136.0f)

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("foodtrip");
        director->setOpenGLView(glview);
    }
    
    // multi screen
    glview->setDesignResolutionSize(DW, DH, ResolutionPolicy::FIXED_WIDTH);
    
    // add file path
    std::vector<std::string> searchPath;
    searchPath.push_back("images");
    searchPath.push_back("configs");
    CCFileUtils::getInstance()->setSearchPaths(searchPath);
    
    director->setContentScaleFactor(RW/DW);
    
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    // read player data
    PlayerData::getInstance()->readFromFile();
    
    // create a scene. it's an autorelease object
    auto scene = FlashScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
