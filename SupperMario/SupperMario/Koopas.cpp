#include "Koopas.h"
#include "Brick.h"
#include "BrickColor.h"
#include "BrickQuesion.h"
#include "BrickGold.h"

#include "mario.h"
#include "Goomba.h"
#include "ParaGoomba.h"
#include "Venus.h"
#include "PlayScene.h"

Koopas::Koopas(float x, float y, int lvl, int t)
{
	SetPosition(x, y);
	level = lvl;
	type = t;
	isWait = false;
	//SetState(KOOPAS_STATE_WALKING_RIGHT);
	vx = -0.025f;

	backupX = x;
	backupY = y;

	jumping = 0;
	time_jumping = 0;

	isWaiting = 0;
	isWaiting_start = 0;

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

	if (level < KOOPAS_LEVEL_NORMAL && Game::GetInstance()->GetCurrentSceneId() != 3)
	{
		if (isWaiting == 0)
		{
			StartWaitToNormal();
		}

		if (isWaiting == 1)
		{
			if (GetTickCount() - isWaiting_start > 2500 && GetTickCount() - isWaiting_start <= 5000)
			{
				isWait = true;
			}
			else if (GetTickCount() - isWaiting_start > 5000)
			{
				isWaiting_start = 0;
				isWaiting = 0;
				isWait = false;
				turnToNormal = true;
			}
		}
		else
		{
			isWaiting_start = 0;
			isWaiting = 0;
			isWait = false;
			turnToNormal = false;
		}

		if (vx != 0)
		{
			isWait = false;
			isWaiting_start = 0;
			isWaiting = 0;
		}
	}
	else
	{
		isWaiting = 0;
	}

	if (isFinish)
	{
		
	}

	if (jumping == 1)
	{
		if (GetTickCount() - time_jumping > 500)
		{
			jumping = 0;
			time_jumping = 0;
			vy = -0.0005f;
		}
	}

	if (jumping == 1)
	{
		vy = -0.05f;
	}

	GameObject::Update(dt, coObjects);

	vy += KOOPAS_GRAVITY * dt;

	vector<LPGAMEOBJECT> Bricks;
	Bricks.clear();

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<Brick*>(coObjects->at(i))
			|| dynamic_cast<BrickColor*>(coObjects->at(i))
			|| dynamic_cast<BrickGold*>(coObjects->at(i))
			|| dynamic_cast<BrickQuesion*>(coObjects->at(i)))
		{
			Bricks.push_back(coObjects->at(i));
		}
	}

	vector<LPCOLLISIONEVENT>  coEvents;
	vector<LPCOLLISIONEVENT>  coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(&Bricks, coEvents);

	if (coEvents.size() == 0 || isFinish)
	{
		x += dx;
		y += dy;
	}
	else
	{

		float min_tx, min_ty, nx = 0, ny;
		float rdx, rdy;

		float max = 0, min = 999999999;

		float l, t, r, b;
		float l1, t1, r1, b1;
		GetBoundingBox(l1, t1, r1, b1);

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			/*if (nx != 0 && ny == 0)
			{
				nx = -nx;
				vx = -vx;
			}*/

			if (dynamic_cast<BrickColor*>(e->obj))
			{
				BrickColor* b = dynamic_cast<BrickColor*>(e->obj);

				nx = 0;

				if (e->ny == 1)
				{
					ny = 0;
				}
			}
			else if (dynamic_cast<BrickGold*>(e->obj) ||(dynamic_cast<BrickQuesion*>(e->obj)))
			{
				if (e->nx != 0 && level < KOOPAS_LEVEL_NORMAL)
				{
					if (vx > 0)
						SetState(KOOPAS_STATE_WALKING_LEFT);
					else if (vx < 0)
						SetState(KOOPAS_STATE_WALKING_RIGHT);
				}

			}
		}

		if (rdx != 0 && rdx != dx)
			x += nx * abs(rdx);
		
		x += min_tx * dx;// +nx * 0.04f;
		y += min_ty * dy + ny * 0.04f;

		if (ny != 0)
		{
			if (level == KOOPAS_LEVEL_WING)
			{
				StartJumping();
			}
			else
			{
				vy = 0;
			}
		}

		for (UINT i = 0; i < coEvents.size(); i++)
		{
			if (dynamic_cast<BrickColor*>(coEvents[i]->obj))
			{
				if (type == 2)
				{
					coEvents[i]->obj->GetBoundingBox(l, t, r, b);

					if (r > max)
					{
						max = r;
					}
					if (l < min)
					{
						min = l;
					}
				}
				else
				{
					min = 0;
					max = 9999999;
				}
			}
			else
			{
				coEvents[i]->obj->GetBoundingBox(l, t, r, b);

				if (r > max && t1 < t)
				{
					max = r;
				}
				if (l < min && t1 < t)
				{
					min = l;
				}
			}
		}

		if (nx > 0 && vx < 0 && ny == 0)
		{
			//vx = 0.05f;
		}
		else if (nx < 0 && vx>0 && ny == 0)
		{
			//vx = -0.05f;
		}

		if (level >= KOOPAS_LEVEL_NORMAL)
		{
			if (r1 - 10 > max && vx > 0)
			{
				//x = max;
				SetState(KOOPAS_STATE_WALKING_LEFT);
			}
			else if (l1 + 10 < min && vx < 0)
			{
				//x = min;
				SetState(KOOPAS_STATE_WALKING_RIGHT);

				if (level == KOOPAS_LEVEL_WING)
				{
					SetPosition(1424, 250);
					SetState(KOOPAS_STATE_WALKING_LEFT);
				}
			}
		}

		min = 0;
		max = 9999999;

	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

	if (y > 1000 && level == KOOPAS_LEVEL_WING)
	{
		SetPosition(1424, 250);
	}
}

void Koopas::Render()
{
	int ani;

	int state = this->GetState();

	if (level == KOOPAS_LEVEL_WING)
	{
		if (vx > 0)
			ani = KOOPAS_ANI_WING_RIGHT;
		else
			ani = KOOPAS_ANI_WING_LEFT;
	}
	else if (level == KOOPAS_LEVEL_NORMAL)
	{
		if (vx > 0)
			ani = KOOPAS_ANI_NORMAL_RIGHT;
		else
			ani = KOOPAS_ANI_NORMAL_LEFT;
	}
	else if (level == KOOPAS_LEVEL_DIE_DOWN)
	{
		if (vx == 0)
			ani = KOOPAS_ANI_DIE_DOWN;
		else
			ani = KOOPAS_ANI_DIE_RUNNING_DOWN;
	}
	else if (level == KOOPAS_LEVEL_DIE_UP)
	{
		if (vx == 0)
			ani = KOOPAS_ANI_DIE_UP;
		else
			ani = KOOPAS_ANI_DIE_RUNNING_UP;
	}

	if (isWait)
	{
		if (level == KOOPAS_LEVEL_DIE_DOWN)
			ani = KOOPAS_ANI_WAIT_DOWN;
		else if (level == KOOPAS_LEVEL_DIE_UP)
			ani = KOOPAS_ANI_DIE_UP;
	}


	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void Koopas::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case KOOPAS_STATE_WALKING_RIGHT:

		if (level == KOOPAS_LEVEL_DIE_DOWN || level == KOOPAS_LEVEL_DIE_UP) {
			vx = KOOPAS_DIE_SPEED;
		}
		else {
			vx = KOOPAS_WALKING_SPEED;
		}
		break;

	case KOOPAS_STATE_WALKING_LEFT:

		if (level == KOOPAS_LEVEL_DIE_DOWN || level == KOOPAS_LEVEL_DIE_UP) {
			vx = -(KOOPAS_DIE_SPEED);
		}
		else {
			vx = -KOOPAS_WALKING_SPEED;
		}
		break;
	case KOOPAS_STATE_IDLE:

		if (level == KOOPAS_LEVEL_DIE_UP)
		{
			vy = -0.5;
		}

		vx = 0;

		//StartWaitToNormal();
		break;
	}
}

