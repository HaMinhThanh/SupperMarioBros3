#include "MarioWalking.h"
#include "Mario.h"
#include "MarioFalling.h"
#include "MarioIdle.h"

MarioWalking::MarioWalking(MarioData* marioData)
{
	this->mMarioData = marioData;
}

MarioWalking::~MarioWalking()
{

}

void MarioWalking::HandleKeyboard(std::map<int, bool> keys)
{
	if (keys[VK_RIGHT])
	{
		this->mMarioData->mario->SetVx(MARIO_WALKING_SPEED);
	}
	else if (keys[VK_LEFT])
	{
		this->mMarioData->mario->SetVx(-MARIO_WALKING_SPEED);
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
		if (this->mMarioData->mario->GetVx() == 0)
			if (this->mMarioData->mario->nx > 0)
				this->mMarioData->mario->SetAni(MARIO_ANI_BIG_IDLE_RIGHT);
			else
				this->mMarioData->mario->SetAni(MARIO_ANI_BIG_IDLE_LEFT);


		/*if (level == MARIO_LEVEL_BIG)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
				else ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_BIG_WALKING_RIGHT;
			else ani = MARIO_ANI_BIG_WALKING_LEFT;
		}*/
}
