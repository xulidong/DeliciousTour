#include "ReadyState.h"
#include "PlayLayer.h"
#include "ReadyState.h"

ReadyState::ReadyState()
{

}

ReadyState::~ReadyState()
{
    
}

void ReadyState::execute(PlayLayer* playLayer)
{
    if (playLayer->getCanDrop()) {
        playLayer->getFSM()->changeState(new DropState());
        return;
    }
    
    playLayer->onReadyState();
    
    if (playLayer->getCanRemove()) {
        playLayer->getFSM()->changeState(new RemoveState());
    }
}