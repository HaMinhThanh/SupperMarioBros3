#pragma once

#include "GameObject.h"

#define	LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 16

class Leaf: public GameObject
{
public:
	int falling;
	DWORD start_falling;
	void Start_Falling() { falling = 1; start_falling = GetTickCount(); }
	Leaf();
	~Leaf();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

