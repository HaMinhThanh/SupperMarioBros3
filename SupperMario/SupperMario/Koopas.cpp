#include "Koopas.h"
#include "Brick.h"
#include "BrickColor.h"
#include "BrickQuesion.h"
#include "BrickGold.h"

Koopas::Koopas(float x, float y, int level)
{
	SetPosition(x, y);
	this->level = level;
	isWait = false;
	SetState(KOOPAS_STATE_IDLE);
}

void Koopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (level == KOOPAS_LEVEL_DIE_DOWN || level == KOOPAS_LEVEL_DIE_UP)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void Koopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (turnToNormal) {
		y -= 11;
		level = KOOPAS_LEVEL_NORMAL;
		SetState(KOOPAS_STATE_WALKING_LEFT);
		turnToNormal = false;
	}

	if (GetTickCount() - isWaiting_start > KOOPAS_WAIT_TO_NORMAL_TIME)
	{
		isWaiting_start = 0;
		isWaiting = 0;
		isWait = false;	
		turnToNormal = true;
	}
	else if (GetTickCount() - isWaiting_start > KOOPAS_WAIT_TO_NORMAL_TIME / 2)
	{
		isWait = true;
	}

	if (vx != 0) {
		isWait = false;
		isWaiting_start = 0;
		isWaiting = 0;
	}

	if (level == KOOPAS_LEVEL_WING) {
		vy = -0.3;
	}

	GameObject::Update(dt, coObjects);

	vy += KOOPAS_GRAVITY * dt;

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

		/*for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			GameObject* b = dynamic_cast<GameObject*>(e->obj);

			if (b->GetX() + 16 > maxRight)
			{
				maxRight = b->GetX() + 16;
			}
			if (b->GetX() < minLeft)
			{
				minLeft = b->GetX();
			}

		}
		if (!isDie) {
			if (this->GetX() > maxRight && vx > 0)
				SetState(KOOPAS_STATE_WALKING_LEFT);
			else if (this->GetX() < minLeft && vx < 0)
				SetState(KOOPAS_STATE_WALKING_RIGHT);
		}*/

	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];


	if (vx < 0 && x < 1152)
	{
		//SetState(KOOPAS_STATE_WALKING_RIGHT);
		x = 1152;
		vx = KOOPAS_WALKING_SPEED;
	}
	if (vx > 0 && x > 1488)
	{
		SetState(KOOPAS_STATE_WALKING_LEFT);
		//vx *= -1;
	}
}

void Koopas::Render()
{
	int ani;

	int state = this->GetState();

	if (level == KOOPAS_LEVEL_WING) {
		if (vx > 0)
			ani = KOOPAS_ANI_WING_RIGHT;
		else
			ani = KOOPAS_ANI_WING_LEFT;
	}
	else if (level == KOOPAS_LEVEL_NORMAL) {
		if (vx > 0)
			ani = KOOPAS_ANI_NORMAL_RIGHT;
		else
			ani = KOOPAS_ANI_NORMAL_LEFT;
	}
	else if (level == KOOPAS_LEVEL_DIE_DOWN) {
		if (vx == 0)
			ani = KOOPAS_ANI_DIE_DOWN;
		else
			ani = KOOPAS_ANI_DIE_RUNNING_DOWN;
	}
	else if (level == KOOPAS_LEVEL_DIE_UP) {
		if (vx == 0)
			ani = KOOPAS_ANI_DIE_UP;
		else
			ani = KOOPAS_ANI_DIE_RUNNING_UP;
	}

	if (isWait) {
		if (level == KOOPAS_LEVEL_DIE_DOWN)
			ani = KOOPAS_ANI_WAIT_DOWN;
		else if (level == KOOPAS_LEVEL_DIE_UP)
			ani = KOOPAS_ANI_DIE_UP;
	}


	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void Koopas::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_WALKING_RIGHT:
		if (level == KOOPAS_LEVEL_DIE_DOWN || level == KOOPAS_LEVEL_DIE_UP) {
			vx = KOOPAS_WALKING_SPEED + 0.02f;
		}
		else {
			vx = KOOPAS_WALKING_SPEED;
		}
		break;

	case KOOPAS_STATE_WALKING_LEFT:
		if (level == KOOPAS_LEVEL_DIE_DOWN || level == KOOPAS_LEVEL_DIE_UP) {
			vx = -(KOOPAS_WALKING_SPEED + 0.02f);
		}
		else {
			vx = -KOOPAS_WALKING_SPEED;
		}
		break;
	case KOOPAS_STATE_IDLE:
		if (level == KOOPAS_LEVEL_DIE_UP)
			vy = -0.4;
		vx = 0;
		StartWaitToNormal();
		break;
	}
}

