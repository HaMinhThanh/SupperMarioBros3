#include "MarioJumping.h"
#include "MarioFalling.h"
#include "MarioState.h"
#include "MarioIdle.h"

MarioJumping::MarioJumping(MarioData* marioData)
{
	this->mMarioData = marioData;
	this->mMarioData->mario->SetVy(-MARIO_JUMP_DEFLECT_SPEED);

}

MarioJumping::~MarioJumping()
{

}

void MarioJumping::Update(float dt)
{
	if (mMarioData->mario->GetVy() >= 0)
	{
		//mMarioData->mario->SetState(new MarioFalling(this->mMarioData));
		return;
	}

}

void MarioJumping::HandleKeyboard(std::map<int, bool> keys)
{
	if (keys[VK_RIGHT])
	{
		this->mMarioData->mario->SetVx(MARIO_WALKING_SPEED);
		this->mMarioData->mario->nx = -1;
	}
	else if (keys[VK_LEFT])
	{
		this->mMarioData->mario->SetVx(-MARIO_WALKING_SPEED);
		this->mMarioData->mario->nx = 1;
	}
}

MarioState::StateName MarioJumping::GetState()
{
	return MarioState::Jumping;
}

void MarioJumping::changeAnimation()
{

}
