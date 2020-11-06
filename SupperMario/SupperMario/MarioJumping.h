#pragma once

#include "Mario.h"
#include "MarioState.h"

class MarioJumping: public MarioState
{
public:
	MarioJumping(MarioData* marioData);
	~MarioJumping();

	void Update(DWORD dt);

	virtual void HandleKeyboard(int keys);

	virtual StateName GetState();

	virtual void changeAnimation();

protected:
	float acceleratorY;
	float acceleratorX;
	bool noPressed;
	bool allowMoveRight, allowMoveLeft;
};

