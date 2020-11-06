#pragma once

#include "MarioState.h"

class MarioIdle: public MarioState
{
public:
	MarioIdle(MarioData* marioData);
	~MarioIdle();

	void HandleKeyboard(int keys);

	virtual StateName GetState();	

	virtual void changeAnimation();

protected:

};

