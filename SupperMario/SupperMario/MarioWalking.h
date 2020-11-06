#pragma once

#include "MarioState.h"
#include "Mario.h"

class MarioWalking: public MarioState
{
public:
	MarioWalking(MarioData* marioData);
	~MarioWalking();

	void HandleKeyboard(int keys);

	virtual StateName GetState();

	virtual void changeAnimation();

protected:
	float acceleratorX;
};

