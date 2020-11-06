#pragma once

#include "GameObject.h"

#define	LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 16

class Leaf: public GameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

