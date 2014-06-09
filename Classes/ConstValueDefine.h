#ifndef GameTest_CommonValue_h
#define GameTest_CommonValue_h

// WinSize
#define SIZE            (Director::getInstance()->getWinSize())
#define SIZE_W          (Director::getInstance()->getWinSize().width)
#define SIZE_H          (Director::getInstance()->getWinSize().height)
#define SIZE_MID_POS    ( Point(Director::getInstance()->getWinSize().width/2,Director::getInstance()->getWinSize().height/2) )

#define UM_APPKEY @"537069de56240b698d0fe810"

#define BAG_DATA PlayerData::getInstance()->getBagData()
#define LIFE_MANAGER PlayerData::getInstance()->getBagData()->getLifeManager()

#define FULL_LIFE_TEXT "Full"

#endif
