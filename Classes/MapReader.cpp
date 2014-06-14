#include "MapReader.h"
#include <cstdio>
#include "json/rapidjson.h"
#include "json/document.h"

#define LEVEL_FILE_NAME "level_%.3d.json"

#define LEVEL_LEVEL "level"
#define LEVEL_MODEL "model"
#define LEVEL_MOVE "move"
#define LEVEL_TIME "time"
#define LEVEL_WIDtH "width"
#define LEVEL_HEIGHT "height"

#define LEVEL_FOOD "food"
#define LEVEL_TILE "tile"

MapReader::MapReader()
: m_level(0)
, m_model(eModel_None)
, m_move(0)
, m_time(0)
, m_width(0)
, m_height(0)
, m_vFood(NULL)
, m_vTile(NULL)
{

}

MapReader::~MapReader()
{
    if (m_vFood != NULL) {
        delete [] m_vFood;
    }
    
    if (m_vTile != NULL) {
        delete [] m_vTile;
    }
}

MapReader* MapReader::reader=NULL;

MapReader* MapReader::getInstance()
{
    if (!reader) {
        reader = new MapReader();
    }
    
    return reader;
}

void MapReader::destroyInstance()
{
    delete reader;
}

void MapReader::readDataOf(int level)
{
    if (m_level == level){
        return;
    }
    
    char filename[20] = {0};
    sprintf(filename, LEVEL_FILE_NAME, level);
    CCLOG("%s",filename);
    
    if (!FileUtils::getInstance()->isFileExist(filename)) {
        CCLOG("error--level:%d config file:%s is not exist.", level, filename);
        exit(1);
    }
    
    string data = FileUtils::getInstance()->getStringFromFile(filename);
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    
    if (doc.HasParseError() || !doc.IsArray() ) {
        CCLOG("error--level:%d config file:%s is parse error.", level, filename);
        exit(1);
    }
    
    for (unsigned int i = 0; i < doc.Size(); ++i) {
        rapidjson::Value& v = doc[i];
        
        if (! v.IsObject()) {
            CCLOG("error--level:%d config file:%s is parse error index:%d.", level, filename, i);
            exit(-1);
        }
        
        //判断各属性是否存在（可以支持中文（UTF8格式））
        if (v.HasMember(LEVEL_LEVEL)) {
            m_level = v[LEVEL_LEVEL].GetInt();
        }
        
        if (v.HasMember(LEVEL_MODEL)) {
            m_model = static_cast<eModel>(v[LEVEL_MODEL].GetInt());
        }
        
        if (v.HasMember(LEVEL_MOVE)) {
            m_move = v[LEVEL_MOVE].GetInt();
        }
        
        if (v.HasMember(LEVEL_TIME)) {
            m_time = v[LEVEL_TIME].GetInt();
        }
        
        if (v.HasMember(LEVEL_WIDtH)) {
            m_width = v[LEVEL_WIDtH].GetInt();
        }
        
        if (v.HasMember(LEVEL_HEIGHT)) {
            m_height = v[LEVEL_HEIGHT].GetInt();
        }
        
        if (m_vFood == NULL) {
            m_vFood = new int[m_width * m_height];
        }
        if (v.HasMember(LEVEL_FOOD)) {
            const rapidjson::Value& food = v[LEVEL_FOOD];
            for (unsigned int i = 0; i < food.Size(); ++i) {
                m_vFood[i] = food[i].GetInt();
            }
        }
        
        if (m_vTile == NULL) {
            m_vTile = new int[m_width * m_height];
        }
        if (v.HasMember(LEVEL_TILE)) {
            const rapidjson::Value& tile = v[LEVEL_TILE];
            for (unsigned int i = 0; i < tile.Size(); ++i) {
                m_vTile[i] = tile[i].GetInt();
            }
        }
    }
}