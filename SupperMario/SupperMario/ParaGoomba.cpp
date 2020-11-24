#include "ParaGoomba.h"
#include "Goomba.h"
#include "PlayScene.h"

ParaGoomba::ParaGoomba()
{
	SetState(PARAGOOMBA_STATE_WING);
}

void ParaGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == PARAGOOMBA_STATE_DIE)
	{
		left = top = right = bottom = 0;
	}
	else 
	{
		left = x;
		top = y;
		
		if (state == PARAGOOMBA_STATE_WING) 
		{
			right = x + PARAGOOMBA_BBOX_WIDTH;
			bottom = y + PARAGOOMBA_BBOX_HEIGHT;
		}
		else if (state == PARAGOOMBA_STATE_NORMAL)
		{
			right = x + PARAGOOMBA_BBOX_WIDTH_NORMAL;
			bottom = y + PARAGOOMBA_BBOX_HEIGHT_NORMAL;
		}
	}
}

void ParaGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt, coObjects);

	vy += PARAGOOMBA_GRAVITY * dt;

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
		if (state == PARAGOOMBA_STATE_WING)
			vy = -0.1f;

		float min_tx, min_ty, nx = 0, ny;
		float rdx, rdy;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.04f;
		y += min_ty * dy + ny * 0.04f;

	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

	if (vx < 0 && x < 624)
	{
		x = 624;
		vx = -vx;
	}
	if (vx > 0 && x > 1088)
	{
		x = 1088;
		vx = -vx;
	}
}

void ParaGoomba::Render()
{
	int state = GetState();
	int ani = PARAGOOMBA_ANI_WING;

	if (state == PARAGOOMBA_STATE_DIE) {
		return;
	}
	switch (state)
	{
	case PARAGOOMBA_STATE_WING:
		ani = PARAGOOMBA_ANI_WING;
		break;
	case PARAGOOMBA_STATE_NORMAL:
		ani = PARAGOOMBA_ANI_NORMAL;
		break;
	}

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void ParaGoomba::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		this->SetId(-1);
		break;
	case PARAGOOMBA_STATE_WING:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	case PARAGOOMBA_STATE_NORMAL:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	}
}

