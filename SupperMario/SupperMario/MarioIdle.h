#pragma once

#include "MarioState.h"

class MarioIdle: public MarioState
{
public:
	MarioIdle(MarioData* marioData);
	~MarioIdle();

	void HandleKeyboard(std::map<int, bool> keys);

	StateName GetState();	

	void changeAnimation();

protected:

};

