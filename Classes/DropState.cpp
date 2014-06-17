//
//  DropState.cpp
//  GameTest
//
//  Created by Macmini on 14-5-5.
//
//

#include "DropState.h"
#include "PlayLayer.h"
#include "ReadyState.h"

DropState::DropState()
{

}

DropState::~DropState()
{

}

void DropState::execute(PlayLayer* playLayer)
{
    playLayer->onStateDrop();
    playLayer->getFSM()->changeState(new ReadyState());
}