#pragma once
#include "GameObject.h"


class Node: public GameObject
{
public:
	Node(float x, float y, int l, int t, int r, int b, int  type );
	~Node();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetDirectNode(float x, float y, int l, int t, int r, int b);

	bool left;
	bool top;
	bool right;
	bool bottom;

	int type;

};

