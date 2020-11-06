#pragma once

#include "MarioState.h"

class MarioFalling: public MarioState
{
public:
	MarioFalling(MarioData* marioData);
	~MarioFalling();

	void Update(DWORD dt);

	void HandleKeyboard(int keys);

	virtual void changeAnimation();

	virtual StateName GetState();

protected:
	float acceleratorY;
	float acceleratorX;

	//neu nhu van toc ban dau = khong thi se khong cho giam toc do
	bool allowMoveX;

	bool isLeftOrRightKeyPressed;

};

