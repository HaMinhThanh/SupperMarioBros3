#include <algorithm>
#include "Game.h"
#include "Mario.h"
#include "Sprites.h"

#include "Goomba.h"

//Mario::Mario(float x, float y, float vx) :GameObject(x, y)
//{
//	this->vx = vx;
//};

Mario::Mario(Camera* camera)
{
	this->camera = camera;

}

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	GameObject::Update(dt);

	vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx + nx * 0.04f;
		y += min_ty * dy + ny * 0.04f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Goomba*>(e->obj)) // e is goomba
			{
				Goomba* goomba = dynamic_cast<Goomba*>(e->obj);

				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level -= 1;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	
}

void Mario::Render()
{
	//LPANIMATION ani;
	//
	////ani = Animations::GetInstance()->Get(510);

	//if (vx > 0) 
	//	ani = Animations::GetInstance()->Get(500);
	//else 
	//	ani = Animations::GetInstance()->Get(501);

	//ani->Render(x, y);
	
	int ani;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else if (level == MARIO_LEVEL_TAIL) {
		if (vx == 0)
		{
			if (nx > 0)ani = MARIO_ANI_TAIL_IDLE_RIGHT;
			//else if (nx < 0) ani = MARIO_ANI_BIG_JUMP_LEFT;
			else
				ani = MARIO_ANI_TAIL_IDLE_LEFT;
		}
		else if (vx > 0)
			ani = MARIO_ANI_TAIL_WALKING_RIGHT;
		else ani = MARIO_ANI_TAIL_WALKING_LEFT;
	}
	else if (level == MARIO_LEVEL_BIG) {
		if (vx == 0)
		{
			if (nx > 0)ani = MARIO_ANI_BIG_IDLE_RIGHT;
			//else if (nx < 0) ani = MARIO_ANI_BIG_JUMP_LEFT;
			else
				ani = MARIO_ANI_BIG_IDLE_LEFT;
		}
		else if (vx > 0)
			ani = MARIO_ANI_BIG_WALKING_RIGHT;
		else ani = MARIO_ANI_BIG_WALKING_LEFT;
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (vx == 0)
		{
			if (nx > 0)ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			//else if (nx < 0) ani = MARIO_ANI_BIG_JUMP_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		else if (vx > 0)
			ani = MARIO_ANI_SMALL_WALKING_RIGHT;
		else ani = MARIO_ANI_SMALL_WALKING_LEFT;
	}
	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void Mario::SetState(int  state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		vy = -MARIO_JUMP_SPEED_Y;
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	case MARIO_STATE_FLY:
		//vy = -MARIO_FLY_SPEED_Y;
		break;
	case MARIO_STATE_HOLD:
		break;
	case MARIO_STATE_KICK:
		break;
	case MARIO_STATE_RUN:
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void Mario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (level == MARIO_LEVEL_TAIL)
	{
		right = x + MARIO_TAIL_BBOX_WIDTH;
		bottom = y + MARIO_TAIL_BBOX_HEIGHT;
	}
	if (level == MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void Mario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetState(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);

	isWalking = 0;
	isRunning = 0;
	isJumping = 0;
	isFlying = 0;
	isFalling = 0;
	isKicking = 0;
	isHolding = 0;
}

void Mario::CollisionWidthBrick(vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	
	coEvents.clear();

	vector<LPGAMEOBJECT>  Bricks;
	Bricks.clear();

	for (UINT i = 0; i < coObjects->size(); i++)
		if (dynamic_cast<Goomba*>(coObjects->at(i)))
			Bricks.push_back(coObjects->at(i));

	CalcPotentialCollisions(&Bricks, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		isCollisionOnAxisY = false;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx + nx * 0.4f;
		if (ny == -1)
			y += min_ty * dy + ny * 0.4f;
		else
			y += dy;

		if (ny != 0)
			isCollisionOnAxisY = true;
		else
			isCollisionOnAxisY = false;
	}

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}