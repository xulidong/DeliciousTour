#ifndef __GameTest__Props__
#define __GameTest__Props__

#include <iostream>
#include "CommonHeader.h"

enum eProps{
    ePropsNone,
    ePropsLine = 0,
    ePropsExplode,
    ePropsSame,
    ePropsStorm,
    ePropsSwap,
    ePropsClock,
    ePropsChilli,
    ePropsSort,
    ePropsBoom,
    ePropsMax
};

class Props : public Ref
{
public:
    Props();
    ~Props();
    
    CC_SYNTHESIZE(eProps, m_id, Id);
    CC_SYNTHESIZE(string, m_name, Name);
    CC_SYNTHESIZE(int, m_count, Count);
    CC_SYNTHESIZE(bool, m_visible, Visible);
    CC_SYNTHESIZE(string, m_des, Des);
};

#endif
