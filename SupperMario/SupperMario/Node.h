#pragma once
#include "GameObject.h"


class Node: public GameObject
{
public:
	Node(float x, float y, int l, int t, int r, int b);
	~Node();

	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	bool left;
	bool top;
	bool right;
	bool bottom;

};

