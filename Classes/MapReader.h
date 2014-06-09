#ifndef __GameTest__MapReader__
#define __GameTest__MapReader__

#include "CommonHeader.h"

enum eModel{
    eModel_None,
    eModel_Score,
    eModel_Max
};

class MapReader : public Ref
{
public:
    ~MapReader();
    static MapReader* getInstance();
    static void destroyInstance();
    
    void readDataOf(int level);
    
    CC_SYNTHESIZE(int, m_level, Level);
    CC_SYNTHESIZE(eModel, m_model, Model);
    
    // 限制条件
    CC_SYNTHESIZE(int, m_move, Move);
    CC_SYNTHESIZE(int, m_time, Time);
    
    // 过关条件
    
    // 评星标准
    
    // 地图
    
private:
    MapReader();
    static MapReader* reader;
};

#endif
