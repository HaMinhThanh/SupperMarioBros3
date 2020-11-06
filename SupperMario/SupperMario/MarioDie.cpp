#include "MarioDie.h"

MarioDie::MarioDie(MarioData* marioData)
{
	this->mMarioData = marioData;
}

MarioDie::~MarioDie()
{

}

void MarioDie::HandleKeyboard(int keys)
{

}

void MarioDie::Update(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult)
{
	this->mMarioData->mario->SetVx(0);
	this->mMarioData->mario->SetVy(-MARIO_DIE_DEFLECT_SPEED);
}

void MarioDie::changeAnimation()
{
	this->mMarioData->mario->SetAni(MARIO_ANI_DIE);
}

MarioState::StateName MarioDie::GetState()
{
	return MarioState::Die;
}
