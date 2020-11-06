#pragma once

#include "MarioState.h"
#include "mario.h"

class MarioDie: public MarioState
{
public:
	MarioDie(MarioData* marioData);
	~MarioDie();

	void HandleKeyboard(int keys);

	void Update(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult);

	virtual StateName GetState();

	virtual void changeAnimation();
};

