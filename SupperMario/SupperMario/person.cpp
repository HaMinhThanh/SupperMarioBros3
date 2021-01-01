#include "person.h"

person::person(float mx, float mn)
{
	max = mx;
	min = mn;

	vx = 0.02f;
}

person::~person()
{

}

void person::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

	if (x >= max)
	{
		vx = -vx;
	}

	if (x <= min)
	{
		vx = -vx;
	}
}

void person::Render()
{
	if (vx > 0)
	{
		animation_set->at(0)->Render(x, y);
	}
	else
	{
		animation_set->at(1)->Render(x, y);
	}
}

void person::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x+16;
	bottom = y+15;
}
