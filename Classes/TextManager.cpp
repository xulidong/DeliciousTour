#include "TextManager.h"

#define CHINESE_FILE "zh_CN.plist"

TextManager::TextManager()
{
    
}

TextManager::~TextManager()
{

}

TextManager* TextManager::pInstance = NULL;

TextManager* TextManager::getInstance()
{
    if (! pInstance) {
        pInstance = new TextManager();
        pInstance->readTextFromFile();
    }
    
    return pInstance;
}

void TextManager::destroyInstance()
{
    CC_SAFE_DELETE(pInstance);
}

void TextManager::readTextFromFile()
{
    string fileName;
    LanguageType currentLanguageType = CCApplication::getInstance()->getCurrentLanguage();
    switch (currentLanguageType) {
        default:
            fileName = CHINESE_FILE;
            break;
    }
    
   m_valueMap = FileUtils::getInstance()->getValueMapFromFile(CHINESE_FILE);
}

string TextManager::getTextByKey(const string &key)
{
    return m_valueMap.at(key).asString();
}