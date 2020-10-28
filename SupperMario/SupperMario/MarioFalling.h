#pragma once

#include "MarioState.h"

class MarioFalling: public MarioState
{
public:
	MarioFalling(MarioData* marioData);
	~MarioFalling();

	void Update(DWORD dt);

	void HandleKeyboard(std::map<int, bool> keys);

	void changeAnimation();

	StateName GetState();

protected:
	bool allowMoveX;

};

