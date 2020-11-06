#include "MarioIdle.h"
#include "mario.h"
#include "MarioFalling.h"
#include "MarioWalking.h"
#include "MarioData.h"
#include <dinput.h>

MarioIdle::MarioIdle(MarioData* marioData)
{
	this->mMarioData = marioData;
	this->mMarioData->mario->SetVx(0);
	this->mMarioData->mario->SetVy(0);
}

MarioIdle::~MarioIdle()
{

}

void MarioIdle::HandleKeyboard(int keys)
{
	Game* game = Game::GetInstance();

	if (game->IsKeyDown(DIK_RIGHT) ) 
	{
		this->mMarioData->mario->SetStateName(new MarioWalking(this->mMarioData));
		this->mMarioData->mario->SetVx(PLAYER_MAX_RUNNING_SPEED);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		this->mMarioData->mario->SetStateName(new MarioWalking(this->mMarioData));
		this->mMarioData->mario->SetVx(-PLAYER_MAX_RUNNING_SPEED);
	}
}

MarioState::StateName MarioIdle::GetState()
{
	return MarioState::Idle;
}

void MarioIdle::changeAnimation()
{
	if (this->mMarioData->mario->GetLevel() == MARIO_LEVEL_BIG)
	{
		if (mMarioData->mario->GetNx() > 0)
			mMarioData->mario->SetAni(MARIO_ANI_BIG_IDLE_RIGHT);
		else
			mMarioData->mario->SetAni(MARIO_ANI_BIG_IDLE_LEFT);
	}
	else if (this->mMarioData->mario->GetLevel() == MARIO_LEVEL_SMALL)
	{
		if (mMarioData->mario->GetNx() > 0)
			mMarioData->mario->SetAni(MARIO_ANI_SMALL_IDLE_RIGHT);
		else
			mMarioData->mario->SetAni(MARIO_ANI_SMALL_IDLE_LEFT);
	}
	else if (this->mMarioData->mario->GetLevel() == MARIO_LEVEL_TAIL)
	{
		if (mMarioData->mario->GetNx() > 0)
			mMarioData->mario->SetAni(MARIO_ANI_TAIL_IDLE_RIGHT);
		else
			mMarioData->mario->SetAni(MARIO_ANI_TAIL_IDLE_LEFT);
	}
}