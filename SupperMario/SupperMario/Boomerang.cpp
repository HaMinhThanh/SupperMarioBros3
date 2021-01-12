#include "Boomerang.h"

Boomerang::Boomerang()
{
	SetAnimationSet(AnimationSets::GetInstance()->Get(46));
}

Boomerang::~Boomerang()
{
}

void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
}

void Boomerang::Render()
{
	if (direct == 1)
	{
		animation_set->at(0)->Render(x, y);
	}
	else
	{
		animation_set->at(1)->Render(x, y);
	}
}

void Boomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOMERANG_BBOX_WIDHT;
	bottom = y + BOOMERANG_BBOX_HEIGHT;
}
