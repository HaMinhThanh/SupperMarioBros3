#include "MarioWalking.h"
#include "Mario.h"
#include "MarioFalling.h"
#include "MarioIdle.h"
#include "MarioDie.h"
#include <dinput.h>

MarioWalking::MarioWalking(MarioData* marioData)
{
	this->mMarioData = marioData;

	this->mMarioData->mario->allowMoveLeft = true;
	this->mMarioData->mario->allowMoveRight = true;
}

MarioWalking::~MarioWalking()
{

}

void MarioWalking::HandleKeyboard(int keys)
{
    Game* game = Game::GetInstance();
    if (game->IsKeyDown(DIK_RIGHT))
    {
        if (mMarioData->mario->allowMoveRight)
        {            
			this->mMarioData->mario->SetVx(PLAYER_MAX_RUNNING_SPEED);
        }
    }
    else if (game->IsKeyDown(DIK_LEFT))
    {
        if (mMarioData->mario->allowMoveLeft)
        {           
            this->mMarioData->mario->SetVx(-PLAYER_MAX_RUNNING_SPEED);     
        }
    }
    else
    {
        this->mMarioData->mario->SetStateName(new MarioIdle(this->mMarioData));
        return;
    }
}

MarioState::StateName MarioWalking::GetState()
{
	return MarioState::Walking;
}

void MarioWalking::changeAnimation()
{

	if (this->mMarioData->mario->GetLevel() == MARIO_LEVEL_BIG)
	{
		if (mMarioData->mario->GetVx() > 0)
			mMarioData->mario->SetAni(MARIO_ANI_BIG_WALKING_RIGHT);
		else 
			mMarioData->mario->SetAni(MARIO_ANI_BIG_WALKING_LEFT);
	}
	else if (this->mMarioData->mario->GetLevel() == MARIO_LEVEL_SMALL)
	{
		if (mMarioData->mario->GetVx() > 0)
			mMarioData->mario->SetAni(MARIO_ANI_SMALL_WALKING_RIGHT);
		else
			mMarioData->mario->SetAni(MARIO_ANI_SMALL_WALKING_LEFT);
	}
	else if (this->mMarioData->mario->GetLevel() == MARIO_LEVEL_TAIL)
	{
		if (mMarioData->mario->GetVx() > 0)
			mMarioData->mario->SetAni(MARIO_ANI_TAIL_WALKING_RIGHT);
		else
			mMarioData->mario->SetAni(MARIO_ANI_TAIL_WALKING_LEFT);
	}
}
