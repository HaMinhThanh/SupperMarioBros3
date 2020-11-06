#include "Goomba.h"
#include "PlayScene.h"

Goomba::Goomba()
{
	SetState(GOOMBA_STATE_WALKING);
}

void Goomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = top = right = bottom = 0;
	}
	else {
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

	x += dx;
	y += dy;

	/*if (this->GetState() == GOOMBA_STATE_DIE) {
		
	}*/

	if (vx < 0 && x < 0)
	{
		x = 0;
		vx = -vx;
	}
	if (vx > 0 && x > 290)
	{
		x = 290;
		vx = -vx;
	}
}

void Goomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		return;		
	}

	animation_set->at(ani)->Render(x, y);
}

void Goomba::SetState(int state)
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
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
	}
}
