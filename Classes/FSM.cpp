//
//  FSM.cpp
//  GameTest
//
//  Created by Macmini on 14-5-5.
//
//

#include "FSM.h"
#include "PlayLayer.h"

FSM::FSM()
{
    
}

FSM::~FSM()
{
    CC_SAFE_DELETE(mPlayLayer);
}

FSM* FSM::createFSM(PlayLayer* playLayer)
{
    auto node = new FSM;
    if (node && node->init(playLayer))
    {
        node->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(node);
    }
    return node;
}

bool FSM::init(PlayLayer* playLayer)
{
    if (!Node::init()) {
        return false;
    }
    
    do
    {
        mCurState = NULL;
        mPlayLayer = playLayer;
        mPlayLayer->retain();
    }while(0);
    
    return true;
}

void FSM::changeState(BaseState* state)
{
    CC_SAFE_DELETE(mCurState);
    mCurState = state;
}

void FSM::update(float dt)
{
    mCurState->execute(mPlayLayer);
}