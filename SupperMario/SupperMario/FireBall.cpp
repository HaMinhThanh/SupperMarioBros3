#include "FireBall.h"
#include "Brick.h"

FireBall::FireBall()
{
	StartUnFinish();

	isFinish = false;
	vy = -FIREBALL_SPEED_Y;
}

FireBall::~FireBall()
{

}

void FireBall::Render()
{
	if (isFinish) return;

	animation_set->at(0)->Render(x, y);
}

void FireBall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isFinish) {
		left = top = right = bottom = 0;
		return;
	}

	left = x;
	top = y;
	right = x + FIREBALL_BBOX_WIDTH;
	bottom = y + FIREBALL_BBOX_HEIGHT;
}

void FireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt, coObjects);

	vy += FIREBALL_GRAVITY * dt;

	if (GetTickCount() - untouchable_start > FIREBALL_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;

		isFinish = true;
	}
	vector<LPGAMEOBJECT> Bricks;
	Bricks.clear();

	for (UINT i = 0; i < coObjects->size(); i++)
		if (dynamic_cast<Brick*>(coObjects->at(i)))
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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.04f;
		y += min_ty * dy + ny * 0.04f;

		if (nx != 0) vx *= -1;
		if (ny != 0) vy *= -1;

		//if (vy < -FIREBALL_SPEED_MAX_Y) vy = -FIREBALL_SPEED_MAX_Y;

	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
	
}

void FireBall::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case FIREBALL_GOING_RIGHT:
		vx = FIREBALL_SPEED_X;
		break;
	case FIREBALL_GOING_LEFT:
		vx = -FIREBALL_SPEED_X;
		break;
	}
}


