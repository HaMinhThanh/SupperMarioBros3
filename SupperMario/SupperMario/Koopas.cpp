#include "Koopas.h"
#include "Brick.h"

Koopas::Koopas()
{
	SetState(KOOPAS_STATE_WALKING_LEFT);
}

void Koopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE || state == KOOPAS_STATE_DIE_UP || state == KOOPAS_STATE_DIE_WALKING_RIGHT || state == KOOPAS_STATE_DIE_WALKING_LEFT)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void Koopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
		if (isDie) 
			SetState(KOOPAS_STATE_DIE_WALKING_RIGHT);
		
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

void Koopas::Render()
{
	int ani;

	int state = this->GetState();

	switch (state)
	{
	case KOOPAS_STATE_DIE:
		ani = KOOPAS_ANI_DIE_DOWN;
		break;
	case KOOPAS_STATE_DIE_UP:
		ani = KOOPAS_ANI_DIE_UP;
		break;
	case KOOPAS_STATE_DIE_WALKING_RIGHT:
		ani = KOOPAS_ANI_DIE_DOWN;
		break;
	case KOOPAS_STATE_DIE_WALKING_LEFT:
		ani = KOOPAS_ANI_DIE_DOWN;
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

	//RenderBoundingBox();
}

void Koopas::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		isDie = true;
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_DIE_UP:
		isDie = true;
		vx = 0;
		vy = -0.3f;
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
	}
}
