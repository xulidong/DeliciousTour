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
    if(playLayer->getCanRemove())
    {
        playLayer->onRemoveState();
    }
    else
    {
        playLayer->getFSM()->changeState(new ReadyState());
    }
}