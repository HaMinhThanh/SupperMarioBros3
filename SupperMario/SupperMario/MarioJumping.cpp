#include "MarioJumping.h"
#include "MarioFalling.h"
#include "MarioState.h"
#include "MarioIdle.h"
#include <dinput.h>

MarioJumping::MarioJumping(MarioData* marioData)
{
	this->mMarioData = marioData;
	this->mMarioData->mario->SetVy(PLAYER_MIN_JUMP_VELOCITY);

	acceleratorY = 15.0f;
	acceleratorX = 14.0f;

	noPressed = false;
}

MarioJumping::~MarioJumping()
{

}

void MarioJumping::HandleKeyboard(int keys)
{
	Game* game = Game::GetInstance();

	if (game->IsKeyDown(DIK_RIGHT))
	{		
		this->mMarioData->mario->SetVx(PLAYER_MAX_RUNNING_SPEED);

		noPressed = false;
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{		
		this->mMarioData->mario->SetVx(-PLAYER_MAX_RUNNING_SPEED);

		noPressed = false;
	}
	else
	{
		noPressed = true;
	}
}

MarioState::StateName MarioJumping::GetState()
{
	return MarioState::Jumping;
}

void MarioJumping::changeAnimation()
{
	if (this->mMarioData->mario->GetLevel() == MARIO_LEVEL_BIG)
	{
		if (mMarioData->mario->GetVx() > 0)
			mMarioData->mario->SetAni(MARIO_ANI_BIG_JUMPING_RIGHT);
		else
			mMarioData->mario->SetAni(MARIO_ANI_BIG_JUMPING_LEFT);
	}
	else if (this->mMarioData->mario->GetLevel() == MARIO_LEVEL_SMALL)
	{
		if (mMarioData->mario->GetVx() > 0)
			mMarioData->mario->SetAni(MARIO_ANI_SMALL_JUMPING_RIGHT);
		else
			mMarioData->mario->SetAni(MARIO_ANI_SMALL_JUMPING_LEFT);
	}
	else if (this->mMarioData->mario->GetLevel() == MARIO_LEVEL_TAIL)
	{
		if (mMarioData->mario->GetVx() > 0)
			mMarioData->mario->SetAni(MARIO_ANI_TAIL_JUMPING_RIGHT);
		else
			mMarioData->mario->SetAni(MARIO_ANI_TAIL_JUMPING_LEFT);
	}
}

void MarioJumping::Update(DWORD dt)
{
	this->mMarioData->mario->AddVy(acceleratorY);

	if (mMarioData->mario->GetVy() >= 0)
	{
		mMarioData->mario->SetStateName(new MarioFalling(this->mMarioData));

		return;
	}

	if (noPressed)
	{
		if (mMarioData->mario->GetVx() < 0)
		{
			//player dang di chuyen sang ben trai      
			if (mMarioData->mario->GetVx() < 0)
			{
				this->mMarioData->mario->AddVx(acceleratorX);

				if (mMarioData->mario->GetVx() > 0)
					this->mMarioData->mario->SetVx(0);
			}
		}
		else if (mMarioData->mario->GetVx() > 0)
		{
			//player dang di chuyen sang phai   
			if (mMarioData->mario->GetVx() > 0)
			{
				this->mMarioData->mario->AddVx(-acceleratorX);

				if (mMarioData->mario->GetVx() < 0)
					this->mMarioData->mario->SetVx(0);
			}
		}
	}
}