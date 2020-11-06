#pragma once

#ifndef __MARIOSTATE_H__



#include "Game.h"
#include "GameObject.h"
#include "MarioData.h"
#include "Goomba.h"
#include "Koopas.h"
//#include "MarioDie.h"

#include <map>

using namespace std;

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

	virtual void HandleKeyboard(int keys);

	virtual StateName GetState() = 0;

	virtual void changeAnimation()=0 ;

protected:
	MarioState(MarioData* marioData);
	MarioState();

	MarioData* mMarioData;
};
#endif // !_MARIOSTATE_H_

