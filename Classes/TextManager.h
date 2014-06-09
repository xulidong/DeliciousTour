#ifndef __GameTest__TextManager__
#define __GameTest__TextManager__

#include <iostream>
#include "cocos2d.h"

using namespace std;
USING_NS_CC;


//Key
#define kSHARE      "kShare"
#define kTime       "kTime"
#define kLife       "kLife"

class TextManager
{
public:
    ~TextManager();
    static TextManager* getInstance();
    static void destroyInstance();
    string getTextByKey(const string &key);
    
private:
    TextManager();
    void readTextFromFile();
    
    static TextManager *pInstance;
    ValueMap m_valueMap;
};

#endif
