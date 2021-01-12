#pragma once
#include "GameObject.h"
#include "Boomerang.h"

#define BOOMERANGBRO_STATE_WALKING_RIGHT	10
#define BOOMERANGBRO_STATE_WALKING_LEFT		20
#define BOOMERANGBRO_STATE_JUMPING			30
#define BOOMERANGBRO_STATE_DIE				0

#define BOOMERANGBRO_BBOX_WIDTH		16
#define BOOMERANGBRO_BBOX_HEIGHT	24

class BoomerangBro: public GameObject
{
public:
	BoomerangBro();
	~BoomerangBro();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);


};

