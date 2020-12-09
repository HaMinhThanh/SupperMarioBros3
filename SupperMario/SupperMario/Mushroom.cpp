#include "Mushroom.h"
#include "Brick.h"
#include "BrickQuesion.h"
#include "BrickGold.h"
#include "BrickColor.h"
#include "mario.h"

Mushroom::Mushroom()
{
	isFinish = false();
	start = true;
}

Mushroom::~Mushroom()
{

}

void Mushroom::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) 
{
	if (start) {
		Mario* mario = Mario::GetInstance(0, 0);

		if (x < mario->x)
		{
			vx = -0.05f;
		}
		else
		{
			vx = 0.05f;
		}
		start = false;
	}	
	
	GameObject::Update(dt, coObjects);

	vy += MUSHROOM_GRAVITY * dt;

	vector<LPGAMEOBJECT> Bricks;
	Bricks.clear();

	for (UINT i = 0; i < coObjects->size(); i++)
		if (dynamic_cast<Brick*>(coObjects->at(i)) || dynamic_cast<BrickColor*>(coObjects->at(i))
			|| dynamic_cast<BrickGold*>(coObjects->at(i)) || dynamic_cast<BrickQuesion*>(coObjects->at(i)))
			Bricks.push_back(coObjects->at(i));

	vector<LPCOLLISIONEVENT>  coEvents;
	vector<LPCOLLISIONEVENT>  coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(&Bricks, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx, rdy;
		float maxRight = 0, minLeft = 99999999;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.04f;
		y += min_ty * dy + ny * 0.04f;

		if (nx != 0) {
			vx = -vx;
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

	if (x < 0 || y>500)
		isFinish = true;
}

void Mushroom::Render()
{
	if (isFinish) return;
	animation_set->at(0)->Render(x, y);
}

void Mushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isFinish) {
		left = top = right = bottom = 0;
		return;
	}

	left = x;
	top = y;
	right = x + MUSHROOM_BBOX_WIDTH;
	bottom = y + MUSHROOM_BBOX_HEIGHT;
}
