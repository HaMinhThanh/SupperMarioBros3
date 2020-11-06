#pragma once

#include "GameObject.h"

class BrickColor: public GameObject
{
protected:
	float w;
	float h;

public:
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetBoundingBox(float w, float h);
};

