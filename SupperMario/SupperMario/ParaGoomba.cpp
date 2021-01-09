#include "ParaGoomba.h"
#include "Goomba.h"
#include "PlayScene.h"

ParaGoomba::ParaGoomba()
{
	SetState(PARAGOOMBA_STATE_WALKING_LEFT);
	level = PARAGOOMBA_LEVEL_WING;

	time_walking = 0;
	walking = 0;

	time_jumping = 0;
	jumping = 0;

	time_momenting = 0;
	momentable = 0;

	isWalking = false;
	isJumping = false;
	isMomentable = false;
}

void ParaGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == PARAGOOMBA_LEVEL_DIE)
	{
		left = top = right = bottom = 0;
	}
	else 
	{
		left = x;
		top = y;
		
		if (level == PARAGOOMBA_LEVEL_WING) 
		{
			right = x + PARAGOOMBA_BBOX_WIDTH;
			bottom = y + PARAGOOMBA_BBOX_HEIGHT;
		}
		else if (level == PARAGOOMBA_LEVEL_NORMAL)
		{
			right = x + PARAGOOMBA_BBOX_WIDTH_NORMAL;
			bottom = y + PARAGOOMBA_BBOX_HEIGHT_NORMAL;
		}
	}
}

void ParaGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	if (level == PARAGOOMBA_LEVEL_WING)
	{
		if (walking == 1)
		{
			if (GetTickCount() - time_walking > PARAGOOMBA_TIME_WALKING)
			{
				time_walking = 0;
				walking = 0;

				isMomentable = true;
			}
		}

		if (momentable == 1)
		{
			if (GetTickCount() - time_momenting > PARAGOOMBA_TIME_MOMENT)
			{
				time_momenting = 0;
				momentable = 0;

				isJumping = true;
			}
		}

		if (jumping == 1)
		{
			if (GetTickCount() - time_jumping > PARAGOOMBA_TIME_JUMPING)
			{
				time_jumping = 0;
				jumping = 0;
				vy = -0.005f;
			}
		}


		if (walking == 1)
		{
			vy = 0;
		}


		if (momentable == 1)
		{
			vy = 0;
		}


		if (jumping == 1)
		{
			vy = -0.07f;
		}

		if (isWalking)
		{
			StartWalking();
			isWalking = false;
		}
		else if (isMomentable)
		{
			StartMoment();
			isMomentable = false;
		}
		else if (isJumping)
		{
			StartJumping();
			isJumping = false;
		}
	}

	GameObject::Update(dt, coObjects);

	if (state != GOOMBA_STATE_DIE)
	{
		vy += 0.0001f * dt;
	}

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

	if (coEvents.size() == 0)
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


		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		if (ny != 0)
		{
			if (level == PARAGOOMBA_LEVEL_WING)
				isWalking = true;
			else
				vy = 0;
		}

		for (UINT i = 0; i < coEvents.size(); i++)
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

		if (x + 16 > max && vx > 0)
		{
			//x = max;
			vx = -0.025f;
		}
		else if (x < min && vx < 0)
		{
			//x = min;
			vx = 0.025f;
		}

		x += min_tx * dx;// +nx * 0.04f;
		y += min_ty * dy + ny * 0.04f;

	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

	if (vx < 0 && x <= 672)
	{
		SetState(PARAGOOMBA_STATE_WALKING_RIGHT);
	}
	if (vx > 0 && x >= 1080)
	{
		SetState(PARAGOOMBA_STATE_WALKING_LEFT);
	}
}

void ParaGoomba::Render()
{
	int state = GetState();

	int ani = PARAGOOMBA_ANI_WING;

	if (isFinish == false)
	{
		if (isDie)
		{

		}
		else
		{
			if (level== PARAGOOMBA_LEVEL_WING)
			{
				
				ani = PARAGOOMBA_ANI_WING;
				
			}
			else if (level == PARAGOOMBA_LEVEL_NORMAL)
			{

				ani = PARAGOOMBA_ANI_NORMAL;

			}
			else if (level == PARAGOOMBA_LEVEL_DIE)
			{

				ani = PARAGOOMBA_ANI_DIE;

			}
		}
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void ParaGoomba::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case PARAGOOMBA_STATE_WALKING_LEFT:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	case PARAGOOMBA_STATE_WALKING_RIGHT:
		vx = GOOMBA_WALKING_SPEED;
		break;
	}
}

