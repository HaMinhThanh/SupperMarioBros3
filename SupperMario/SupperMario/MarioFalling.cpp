#include "MarioFalling.h"
#include "Mario.h"
#include "MarioIdle.h"
#include <dinput.h>


MarioFalling::MarioFalling(MarioData* marioData)
{
	this->mMarioData = marioData;

	acceleratorY = 15.0f;
	acceleratorX = 8.0f;

	if (this->mMarioData->mario->GetVx() == 0)
	{
		allowMoveX = false;
	}
	else
	{
		allowMoveX = true;
	}
}

MarioFalling::~MarioFalling()
{

}

void MarioFalling::HandleKeyboard(int keys)
{
	//Game* game = Game::GetInstance();

	//if (game->IsKeyDown(DIK_RIGHT))
	//{

	//	isLeftOrRightKeyPressed = true;
	//	//di chuyen sang phai
	//	if (this->mMarioData->mario->GetVx() < PLAYER_MAX_RUNNING_SPEED)
	//	{
	//		this->mMarioData->mario->AddVx(acceleratorX);

	//		if (this->mMarioData->mario->GetVx() >= PLAYER_MAX_RUNNING_SPEED)
	//		{
	//			this->mMarioData->mario->SetVx(PLAYER_MAX_RUNNING_SPEED);
	//		}
	//	}
	//}
	//else if (game->IsKeyDown(DIK_LEFT))
	//{

	//	isLeftOrRightKeyPressed = true;
	//	//di chuyen sang trai
	//	if (this->mMarioData->mario->GetVx() > -PLAYER_MAX_RUNNING_SPEED)
	//	{
	//		this->mMarioData->mario->AddVx(-acceleratorX);

	//		if (this->mMarioData->mario->GetVx() <= -PLAYER_MAX_RUNNING_SPEED)
	//		{
	//			this->mMarioData->mario->SetVx(-PLAYER_MAX_RUNNING_SPEED);
	//		}
	//	}
	//}
	//else
	//{
	//	isLeftOrRightKeyPressed = false;
	//}
}

MarioState::StateName MarioFalling::GetState()
{
	return MarioState::Falling;
}

void MarioFalling::changeAnimation()
{
	if (this->mMarioData->mario->GetLevel() == MARIO_LEVEL_BIG)
	{
		if (mMarioData->mario->GetVx() > 0)
			mMarioData->mario->SetAni(MARIO_ANI_BIG_FALLING_RIGHT);
		else
			mMarioData->mario->SetAni(MARIO_ANI_BIG_FALLING_LEFT);
	}
	else if (this->mMarioData->mario->GetLevel() == MARIO_LEVEL_SMALL)
	{
		if (mMarioData->mario->GetVx() > 0)
			mMarioData->mario->SetAni(MARIO_ANI_SMALL_FALLING_RIGHT);
		else
			mMarioData->mario->SetAni(MARIO_ANI_SMALL_FALLING_LEFT);
	}
	else if (this->mMarioData->mario->GetLevel() == MARIO_LEVEL_TAIL)
	{
		if (mMarioData->mario->GetVx() > 0)
			mMarioData->mario->SetAni(MARIO_ANI_TAIL_FALLING_RIGHT);
		else
			mMarioData->mario->SetAni(MARIO_ANI_TAIL_FALLING_LEFT);
	}
}

void MarioFalling::Update(DWORD dt)
{
	mMarioData->mario->SetVy(PLAYER_MAX_JUMP_VELOCITY);
	
}