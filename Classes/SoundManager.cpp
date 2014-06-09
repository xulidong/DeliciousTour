#include "SoundManager.h"
#include "SimpleAudioEngine.h"

// key
#define IS_SOUND_ON "isBackGroundMusicOn"
#define BACKGROUNDMUSIC_VOL "backGroundMusicVol"
#define EFFECT_VOL "backGroundMusicVol"

namespace SoundManager
{
    // 声音总开关
    bool isSoundOn()
    {
        return UserDefault::getInstance()->getBoolForKey(IS_SOUND_ON, true);
    }
    
    void setBackgroundMusic(bool isOn)
    {
        UserDefault::getInstance()->setBoolForKey(IS_SOUND_ON, isOn);
    }
    
    // 背景音
    void stopBackGroundMusic()
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }
    
    void playBackGroundMusic(string filename)
    {
        if (isSoundOn()) {
            float vol = UserDefault::getInstance()->getFloatForKey(BACKGROUNDMUSIC_VOL, 0.5f);
            setBackgroundMusicVolume(vol);
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(BACKGROUNDMUSIC);
        }
    }
    
    void setBackgroundMusicVolume(float vol)
    {
        UserDefault::getInstance()->setFloatForKey(BACKGROUNDMUSIC_VOL, vol);
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(vol);
    }
    
    // 音效
    void playEffect(string filename)
    {
        if (isSoundOn()) {
            float vol = UserDefault::getInstance()->getFloatForKey(EFFECT_VOL, 0.5f);
            setEffectVolume(vol);
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(filename.c_str());
        }
    }
    
    void setEffectVolume(float vol)
    {
        if (isSoundOn()) {
            UserDefault::getInstance()->setFloatForKey(EFFECT_VOL, vol);
            CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(vol);
        }
    }
}