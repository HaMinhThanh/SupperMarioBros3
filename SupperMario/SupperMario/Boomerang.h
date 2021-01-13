#pragma once
#include "GameObject.h"

#define BOOMERANG_STATE_RIGHT	10
#define BOOMERANG_STATE_LEFT	20
#define BOOMERANG_STATE_FINISH	0

#define BOOMERANG_BBOX_WIDHT	15	
#define BOOMERANG_BBOX_HEIGHT	16

class Boomerang: public GameObject
{
public:
	Boomerang();
	~Boomerang();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetState(int state);

	int direct;

	DWORD time_boomerang;
	int boomerang;
	bool isBoom;

	void StartBoomerang() { boomerang = 1; time_boomerang = GetTickCount(); }
};

