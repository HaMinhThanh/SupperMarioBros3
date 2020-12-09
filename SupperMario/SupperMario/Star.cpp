#include "Star.h"

Star::Star()
{
	isFinish = false;
	fly = 0;
}

Star::~Star()
{

}

void Star::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (fly == 1)
	{
		vy = -0.2;

		if (GetTickCount() - fly_start > 500)
		{
			fly = 0;
			fly_start = 0;
			isFinish = true;
		}
	}	

	GameObject::Update(dt, coObject);

	x += dx;
	y += dy;
}

void Star::Render()
{
	if (isFinish) return;

	if (item == 1) {
		animation_set->at(1)->Render(x, y);
	}
	else if (item == 2) {
		animation_set->at(2)->Render(x, y);
	}
	else if (item == 3) {
		animation_set->at(3)->Render(x, y);
	}
	else {
		animation_set->at(0)->Render(x, y);
	}	
}

void Star::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isFinish) {
		left = top = right = bottom = 0;
		return;
	}

	left = x;
	top = y;
	right = x + STAR_BBOX_WIDTH;
	bottom = y + STAR_BBOX_HEIGHT;
}
