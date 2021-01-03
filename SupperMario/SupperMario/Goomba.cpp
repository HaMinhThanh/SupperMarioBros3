#include "Goomba.h"
#include "PlayScene.h"

Goomba::Goomba()
{
	SetState(GOOMBA_STATE_WALKING);	
}

void Goomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != GOOMBA_STATE_DIE)
	{
		left = x;
		top = y;
		right = x + GOOMBA_BBOX_WIDTH;

		if (state == GOOMBA_STATE_DIE)
			bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
		else
			bottom = y + GOOMBA_BBOX_HEIGHT;
	}
}

void Goomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
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

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	
	else
	{

		float min_tx, min_ty, nx = 0, ny;
		float rdx, rdy;

		float max=0, min=999999999;

		float l, t, r, b;


		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		if (ny != 0)vy = 0;

		for (UINT i = 0; i < coEvents.size(); i++)
		{
			coEvents[i]->obj->GetBoundingBox(l, t, r, b);
			if (r> max)
			{
				max = r;
			}
			if (l < min)
			{
				min = l;
			}
		}

		if (nx > 0 && vx < 0 && ny == 0)
		{
			vx = 0.025f;
		}
		else if (nx < 0 && vx>0 && ny == 0)
		{
			vx = -0.025f;
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

		x += min_tx * dx + nx * 0.04f;
		y += min_ty * dy + ny * 0.04f;
		
	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

	
}

void Goomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) 
	{
		animation_set->at(ani)->Render(x, y);
	}
	else
	{
		animation_set->at(ani)->Render(x, y);
	}
	
}

void Goomba::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = -0.5f;
		this->SetId(-1);
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
	}
}
