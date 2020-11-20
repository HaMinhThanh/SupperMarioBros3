#include "ParaKoopa.h"

#include "Koopas.h"
#include "Brick.h"

ParaKoopa::ParaKoopa()
{
	SetState(KOOPAS_STATE_WALKING_LEFT);
}

void ParaKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
}

void ParaKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt, coObjects);

	vy += KOOPAS_GRAVITY * dt;

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

		if (nx != 0)
		{
			/*if (nx > 0)
				SetState(KOOPAS_STATE_WALKING_RIGHT);
			else
				SetState(KOOPAS_STATE_WALKING_LEFT);*/
		};
		if (ny != 0) vy = 0;

	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

	if (vx < 0 && x < 1152)
	{
		if (isDie) {
			SetState(KOOPAS_STATE_DIE_WALKING_RIGHT);
		}
		else
			SetState(KOOPAS_STATE_WALKING_RIGHT);

	}
	if (vx > 0 && x > 1488)
	{
		if (isDie) {
			SetState(KOOPAS_STATE_DIE_WALKING_LEFT);
		}
		else
			SetState(KOOPAS_STATE_WALKING_LEFT);
	}
}

void ParaKoopa::Render()
{
	int ani;// 

	int state = this->GetState();

	switch (state)
	{
	case KOOPAS_STATE_DIE:
		ani = KOOPAS_ANI_DIE;
		break;
	case KOOPAS_STATE_DIE_WALKING_RIGHT:
		ani = KOOPAS_ANI_DIE_WALKING;
		break;
	case KOOPAS_STATE_DIE_WALKING_LEFT:
		ani = KOOPAS_ANI_DIE_WALKING;
		break;
	case KOOPAS_STATE_WALKING_RIGHT:
		ani = KOOPAS_ANI_WALKING_RIGHT;
		break;
	case KOOPAS_STATE_WALKING_LEFT:
		ani = KOOPAS_ANI_WALKING_LEFT;
		break;
	default:
		return;
	}

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void ParaKoopa::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		isDie = true;
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING_RIGHT:
		vx = KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_WALKING_LEFT:
		vx = -KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_DIE_WALKING_RIGHT:
		vx = KOOPAS_DIE_SPEED;
		break;
	case KOOPAS_STATE_DIE_WALKING_LEFT:
		vx = -KOOPAS_DIE_SPEED;
		break;

	case KOOPAS_STATE_BE_FOLLOW_MARIO:
		vx = 0;
		vy = 0;
		break;
	}
}

