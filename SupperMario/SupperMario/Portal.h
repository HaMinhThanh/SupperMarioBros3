#pragma once
#include "GameObject.h"

class Portal: public GameObject
{
	int scene_id;

	int width;
	int height;
public:
	Portal(float l, float t, float r, float b, int scene_id);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetSceneId() { return scene_id; }
};

