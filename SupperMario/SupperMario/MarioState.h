#pragma once

#include "GameObject.h"
#include "mario.h"

class MarioState
{
public:
	enum StateName
	{
		Idle,
		Walking,
		Running,
		Jumping,
		Holding,
		Kicking,
		Flying
	};

	~MarioState();

	virtual void Update(DWORD dt);
	virtual void HandleKeyboard();

	virtual StateName GetState() = 0;

protected:
	MarioState();
};

