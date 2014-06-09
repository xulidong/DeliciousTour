#ifndef __GameTest__SoundManager__
#define __GameTest__SoundManager__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

#define BACKGROUNDMUSIC ""

using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;

namespace SoundManager
{
    // 声音总开关
    void setSoundOn(bool isOn);
    bool isSoundOn();
    
    // 背景音
    void stopBackGroundMusic();
    void playBackGroundMusic(string filename);
    void setBackgroundMusicVolume(float vol);
    
    // 音效
    void playEffect(string filename);
    void setEffectVolume(float vol);
}


#endif 

