#include "ReadyState.h"
#include "PlayLayer.h"

ReadyState::ReadyState()
{

}

ReadyState::~ReadyState()
{
    
}

void ReadyState::execute(PlayLayer* playLayer)
{
    playLayer->onStateReady();
    
    if (playLayer->getCanRemove()) {
        playLayer->getFSM()->changeState(new RemoveState());
    }
}