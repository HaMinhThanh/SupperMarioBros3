#include "MarioIdle.h"
#include "mario.h"
#include "MarioFalling.h"
#include "MarioWalking.h"
#include "MarioData.h"

MarioIdle::MarioIdle(MarioData* marioData)
{
	this->mMarioData = marioData;
	this->mMarioData->mario->SetVx(0);
	this->mMarioData->mario->SetVy(0);
}

MarioIdle::~MarioIdle()
{

}

void MarioIdle::HandleKeyboard(std::map<int, bool> keys)
{
	if (keys[VK_LEFT] || keys[VK_RIGHT]) 
	{
		//this->mMarioData->mario->SetStateName(new MarioWalking(this->mMarioData));
		return;
	}

}

MarioState::StateName MarioIdle::GetState()
{
	return MarioState::Idle;
}

void MarioIdle::changeAnimation()
{

}