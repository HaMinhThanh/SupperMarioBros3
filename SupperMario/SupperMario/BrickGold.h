#pragma once

#include "GameObject.h"

#define BRICKGOLD_BBOX_WIDTH	16
#define BRICKGOLD_BBOX_HEIGHT	16

class BrickGold: public GameObject
{
public:
	int item;

	BrickGold(float x, float y, int item);

	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

