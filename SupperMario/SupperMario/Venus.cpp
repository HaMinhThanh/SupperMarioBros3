#include "Venus.h"

Venus::Venus(float Max, float Min, int f)
{
	isFire = f;

	//if (isFire == 1)
		fire = new Fire();
	//fire->SetPosition(this->x, this->y);

	max = Max;
	min = Min;

	setFire = 1;

	idle = 0;
	SetState(VENUS_STATE_BOT_LEFT);
	vx = 0;
	vy = VENUS_SPEED_Y;
}

void Venus::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + VENUS_BBOX_WIDTH;
	bottom = y + max-min;
}

void Venus::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

	if(isFire==0)
	{
		if (y >= max)
		{
			vy = -VENUS_SPEED_Y;
		}
		else if (y <= min)
		{
			vy = VENUS_SPEED_Y;
		}
	}
	else {
		if (y >= max)
		{
			vy = -VENUS_SPEED_Y;
			setFire = 1;
		}
		else if (y <= min)
		{
			if (setFire == 1) {
				setFire = 0;
				fire->SetPosition(this->x, this->y);
				int state = GetState();

				if (state == VENUS_STATE_DIE)
				{
					fire->isFinish = true;
				}
				switch (state)
				{
				case VENUS_STATE_TOP_LEFT:
					fire->SetState(FIRE_TOP_LEFT);
					break;
				case VENUS_STATE_TOP_RIGHT:
					fire->SetState(FIRE_TOP_RIGHT);
					break;
				case VENUS_STATE_BOT_LEFT:
					fire->SetState(FIRE_BOT_LEFT);
					break;
				case VENUS_STATE_BOT_RIGHT:
					fire->SetState(FIRE_BOT_RIGHT);
					break;
				}
				if (idle == 0) {
					StartFire();
					vy = 0;
				}
				//vy = VENUS_SPEED_Y;
			}
		}
		if (GetTickCount() - idle_start > 1000)
		{
			idle = 0;
			idle_start = 0;
			isNormal = true;
		}

		if (isNormal)
		{
			if (y <= min)
				vy = VENUS_SPEED_Y;
			isNormal = false;
		}

		fire->Update(dt, coObjects);

		Mario* mario = Mario::GetInstance(0, 0);
		if (mario->isCollisionWithItem(fire))
		{
			if (mario->untouchable == 0)
			{
				if (mario->level > MARIO_LEVEL_SMALL)
				{
					mario->level--;
					mario->StartUntouchable();
				}
				else
					mario->SetState(MARIO_STATE_DIE);
			}

			fire->SetState(FIRE_HIDEN);
		}
	}
}

void Venus::Render()
{
	int ani = VENUS_ANI_TOP_LEFT;

	int state = GetState();

	if (state == VENUS_STATE_DIE)
	{
		return;
	}
	switch (state)
	{
	case VENUS_STATE_TOP_LEFT:
		if (idle == 1) {
			ani = VENUS_ANI_OPEN_TOP_LEFT;
		}
		else {
			ani = VENUS_ANI_TOP_LEFT;
		}
		break;
	case VENUS_STATE_TOP_RIGHT:
		if (idle == 1) {
			ani = VENUS_ANI_OPEN_TOP_RIGHT;
		}
		else {
			ani = VENUS_ANI_TOP_RIGHT;
		}
		break;
	case VENUS_STATE_BOT_LEFT:
		if (idle == 1) {
			ani = VENUS_ANI_OPEN_BOT_LEFT;
		}
		else {
			ani = VENUS_ANI_BOT_LEFT;
		}
		break;
	case VENUS_STATE_BOT_RIGHT:
		if (idle == 1) {
			ani = VENUS_ANI_OPEN_BOT_RIGHT;
		}
		else {
			ani = VENUS_ANI_BOT_RIGHT;
		}
		break;
	}

	animation_set->at(ani)->Render(x, y);
	//if (isFire == 1)
		fire->Render();

	//RenderBoundingBox();
}
void Venus::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case VENUS_STATE_TOP_LEFT:
		//fire->SetState(FIRE_TOP_LEFT);
		break;
	case VENUS_STATE_TOP_RIGHT:
		//fire->SetState(FIRE_TOP_RIGHT);
		break;
	case VENUS_STATE_BOT_LEFT:
		//fire->SetState(FIRE_BOT_LEFT);
		break;
	case VENUS_STATE_BOT_RIGHT:
		//fire->SetState(FIRE_BOT_RIGHT);
		break;
	case VENUS_STATE_DIE:
		//fire->SetState(FIRE_HIDEN);
		break;
	}
}

