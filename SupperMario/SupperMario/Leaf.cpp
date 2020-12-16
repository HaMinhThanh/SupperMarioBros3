#include "Leaf.h"

Leaf::Leaf()
{
	isFinish = false();
	falling = 0;
	Start_Falling();
}

Leaf::~Leaf()
{

}

void Leaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount() - start_falling < 400) {
		vx = -0.05f;
	}
	else if (GetTickCount() - start_falling < 800) {
		vx = 0.05f;
	}
	else if (GetTickCount() - start_falling >= 800) {
		Start_Falling();
	}
	vy = 0.02f;

	GameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
}

void Leaf::Render()
{
	if (isFinish) return;
	animation_set->at(0)->Render(x, y);
	
}

void Leaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isFinish) {
		left = top = right = bottom = 0;
		return;
	}

	left = x;
	top = y;
	right = x + LEAF_BBOX_WIDTH;
	bottom = y + LEAF_BBOX_HEIGHT;
}

