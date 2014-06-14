#include "RemoveState.h"
#include "PlayLayer.h"

RemoveState::RemoveState()
{
    
}

RemoveState::~RemoveState()
{
    
}

void RemoveState::execute(PlayLayer* playLayer)
{
    playLayer->onStateRemove();
    playLayer->getFSM()->changeState(new DropState());
}