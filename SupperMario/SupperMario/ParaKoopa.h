#pragma once

#include "GameObject.h"
#include "Koopas.h"

class ParaKoopa: public GameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	ParaKoopa();
	virtual void SetState(int state);

	bool isDie;
};

