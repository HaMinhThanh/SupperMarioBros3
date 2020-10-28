#pragma once

#include "MarioState.h"
#include "Mario.h"

class MarioWalking: public MarioState
{
public:
	MarioWalking(MarioData* marioData);
	~MarioWalking();

	void HandleKeyboard(std::map<int, bool> keys);

	StateName GetState();

	void changeAnimation();

protected:
	float acceleratorX;
};

