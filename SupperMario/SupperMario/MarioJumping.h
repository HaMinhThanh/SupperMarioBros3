#pragma once

#include "Mario.h"
#include "MarioState.h"

class MarioJumping: public MarioState
{
public:
	MarioJumping(MarioData* marioData);
	~MarioJumping();

	void Update(float dt);

	void HandleKeyboard(std::map<int, bool> keys);

	StateName GetState();

	void changeAnimation();

protected:
	float acceleratorY;
	float acceleratorX;
};

