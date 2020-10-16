#include "Koopas.h"

Koopas::Koopas() 
{
	SetState(KOOPAS_STATE_WALKING);
}

void Koopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		if (state == KOOPAS_STATE_WALKING)
			bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
}

void Koopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

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

void Koopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;

	if (state == KOOPAS_STATE_DIE)
	{
		ani = KOOPAS_ANI_DIE;
	}
	else if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
	else if (vx < 0) ani = KOOPAS_ANI_WALKING_LEFT;

	animations[ani]->Render(x, y);

	RenderBoundingBox();
}

void Koopas::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		vx = -KOOPAS_WALKING_SPEED;
	}
}