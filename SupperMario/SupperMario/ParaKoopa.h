#pragma once

#include "GameObject.h"
#include "Koopas.h"
#include "PlayScene.h"

#define PARAKOOPAS_BBOX_WIDTH	16
#define PARAKOOPAS_BBOX_HEIGHT	28

#define PARAKOOPA_STATE_DIE		200	


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

