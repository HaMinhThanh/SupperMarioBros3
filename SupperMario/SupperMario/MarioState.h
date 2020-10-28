#pragma once

#include "GameObject.h"
#include "MarioData.h"

#include <map>

class MarioState
{
public:
	enum StateName
	{
		Idle,
		Walking,
		Running,
		Falling,
		Jumping,
		Die
	};
	~MarioState();

	virtual void Update(DWORD dt);

	virtual void HandleKeyboard(std::map<int, bool> keys);

	virtual StateName GetState() = 0;

	virtual void changeAnimation() = 0;

protected:
	MarioState(MarioData* marioData);
	MarioState();

	MarioData* mMarioData;
};

