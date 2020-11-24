#pragma once

#include "GameObject.h"
#include "Goomba.h"

#define PARAGOOMBA_GRAVITY		0.001f

#define PARAGOOMBA_BBOX_WIDTH	20
#define PARAGOOMBA_BBOX_HEIGHT	24
#define PARAGOOMBA_BBOX_WIDTH_NORMAL	16
#define PARAGOOMBA_BBOX_HEIGHT_NORMAL	16

#define PARAGOOMBA_STATE_WING		1
#define PARAGOOMBA_STATE_NORMAL		2
#define PARAGOOMBA_STATE_DIE		9

#define PARAGOOMBA_ANI_WING			2
#define PARAGOOMBA_ANI_NORMAL		3
#define PARAGOOMBA_ANI_DIE			4

class ParaGoomba: public GameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:

	ParaGoomba();
	virtual void SetState(int state);
};

