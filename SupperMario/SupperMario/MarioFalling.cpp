#include "MarioFalling.h"
#include "Mario.h"
#include "MarioIdle.h"


MarioFalling::MarioFalling(MarioData* marioData)
{
	this->mMarioData = marioData;

	if (this->mMarioData->mario->GetVx() == 0)
		allowMoveX = false;
	else
		allowMoveX = true;
}

MarioFalling::~MarioFalling()
{

}

void MarioFalling::Update(DWORD dt)
{
	
}

void MarioFalling::HandleKeyboard(std::map<int, bool>keys)
{
	
}

MarioState::StateName MarioFalling::GetState()
{
	return MarioState::Falling;
}

void MarioFalling::changeAnimation()
{

}
