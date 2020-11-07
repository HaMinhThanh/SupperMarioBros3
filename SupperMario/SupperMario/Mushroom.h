#pragma once
#include "GameObject.h"

#define	MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

class Mushroom: public GameObject
{
public:
	Mushroom();
	~Mushroom();

	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

