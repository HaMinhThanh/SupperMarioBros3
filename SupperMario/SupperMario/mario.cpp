#include <algorithm>
#include "Game.h"
#include "Mario.h"
#include "Sprites.h"


#include "Goomba.h"
#include "BrickGold.h"
#include "BrickQuesion.h"
#include "PlayScene.h"

Mario::Mario(float x, float y) :GameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	momentable = 0;
	flyable = 0;

	SetState(MARIO_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->vx = x;
	this->vy = y;
}

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (isTurnToBig) {
		y -= MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT ;
		isTurnToBig = false;
	}
	if (isTurnToTail) {
		y -= MARIO_TAIL_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT ;
		isTurnToTail = false;
	}

	if (isAutoGo)
		x += 1;

	if (x >= 2816 && isAutoGo) {
		isAutoGo = false;
		Reset();
	}

	if (vy == 0 && isFlying == false)
		isJumping = true;

	if (vy > 0 && isWagging && level == MARIO_LEVEL_TAIL) {
		vy -= 0.0007f * dt;
	}
	else if (vy < 0 && isWagging) {
		vy = 0;
	}
	else
		isWagging = false;
	

	if (vx == 0 || vy == MARIO_JUMP_SPEED_Y)
		momentable = 0;

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

	if (momentable == 1) {

		if (GetTickCount() - momentable_start > MARIO_MOMENTUM_TIME) {

			momentable_start = 0;
			momentable = 0;
			isMomentum = false;

			if (level == MARIO_LEVEL_TAIL && isMomentum == false)
				StartFlyable();
		}
	}


	if (isFlying)
	{

		if (GetTickCount() - flyable_start > MARIO_FLY_TIME) {
			isFlying = false;
			flyable = 0;
			isMomentum = true;

			level = MARIO_LEVEL_TAIL;
		}
	}

	
	if (coEvents.size() == 0)
	{		
		collision_x = false;
		collision_y = false;

		x += dx;
		y += dy;
		
	}
	else
	{
		noCollision = true;
		

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		if (rdx != 0 && rdx != dx)
			x += nx * abs(rdx);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<BrickColor*>(e->obj)) 
			{
				BrickColor* b = dynamic_cast<BrickColor*>(e->obj);

				nx = 0;
				
				if (e->ny ==1)
				{
					ny = 0;
				}
			}
		}

		x += min_tx * dx + nx * 0.04f;
		y += min_ty * dy + ny * 0.04f;
	
		if (nx != 0)
		{
			vx = 0;	
			collision_x = true;
		}
		else
			collision_x = false;

		if (ny != 0)
		{
			vy = 0;
			collision_y = true;
		}
		else
			collision_y = false;
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	
}

void Mario::Render()
{
	changeAni();

	if (isAllowKick) {
		if (level == MARIO_LEVEL_BIG) {
			if (vx == 0) {
				if (nx > 0) SetAni(MARIO_ANI_BIG_KICK_RIGHT);
				else SetAni(MARIO_ANI_BIG_KICK_LEFT);
			}
			else if (vx > 0)
				SetAni(MARIO_ANI_BIG_KICK_RIGHT);
			else SetAni(MARIO_ANI_BIG_KICK_LEFT);
		}
		else if (level == MARIO_LEVEL_TAIL) {
			if (vx == 0) {
				if (nx > 0) SetAni(MARIO_ANI_TAIL_KICK_RIGHT);
				else SetAni(MARIO_ANI_TAIL_KICK_LEFT);
			}
			else if (vx > 0)
				SetAni(MARIO_ANI_TAIL_KICK_RIGHT);
			else SetAni(MARIO_ANI_TAIL_KICK_LEFT);
		}
		else if (level == MARIO_LEVEL_SMALL) {
			if (vx == 0) {
				if (nx > 0) SetAni(MARIO_ANI_SMALL_KICK_RIGHT);
				else SetAni(MARIO_ANI_SMALL_KICK_LEFT);
			}
			else if (vx > 0)
				SetAni(MARIO_ANI_SMALL_KICK_RIGHT);
			else SetAni(MARIO_ANI_SMALL_KICK_LEFT);
		}

		isAllowKick = false;
	}
	else if (isHoldingItem )
	{
		if (level == MARIO_LEVEL_BIG) {
			if (vx == 0) {
				if (nx > 0) SetAni(MARIO_ANI_IDLE_BIG_HOLD_RIGHT);
				else SetAni(MARIO_ANI_IDLE_BIG_HOLD_LEFT);
			}
			else if (vx > 0)
				SetAni(MARIO_ANI_WALKING_BIG_HOLD_RIGHT);
			else SetAni(MARIO_ANI_WALKING_BIG_HOLD_LEFT);
		}
		else if (level == MARIO_LEVEL_TAIL) {
			if (vx == 0) {
				if (nx > 0) SetAni(MARIO_ANI_IDLE_TAIL_HOLD_RIGHT);
				else SetAni(MARIO_ANI_IDLE_TAIL_HOLD_LEFT);
			}
			else if (vx > 0)
				SetAni(MARIO_ANI_WALKING_TAIL_HOLD_RIGHT);
			else SetAni(MARIO_ANI_WALKING_TAIL_HOLD_LEFT);
		}			

		isAllowHold = false;
	}
	else if (isAllowSwing) {
		if (vx > 0 || nx > 0)
			SetAni(MARIO_ANI_SWING_RIGHT);
		else
			SetAni(MARIO_ANI_SWING_LEFT);

		isSwing = false;
		isAllowSwing = false;
	}

	else if (isWagging) {
		if (vx > 0 || nx > 0)
			SetAni(MARIO_ANI_WAGGING_RIGHT);
		else
			SetAni(MARIO_ANI_WAGGING_LEFT);

		//isWagging = false;
	}


	int alpha = 255;
	if (untouchable) alpha = 128;
	//if (level == MARIO_LEVEL_BIG && vx == 0)
	//DebugOut(L"Direct: %d", nx);
		//animation_set->at(currentAni)->RenderDirect(x, y, nx, alpha);
	//else
 	animation_set->at(currentAni)->Render(x, y, alpha);

	//RenderBoundingBox();
}

void Mario::SetState(int  state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:

		vx = MARIO_WALKING_SPEED;	

		isTurnLeft = false;
		//isTurnRight = true;

		if (momentable == 0 && level == MARIO_LEVEL_TAIL && isTurnLeft == false)
			StartMomentum();
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		vx = -MARIO_WALKING_SPEED;

		//isTurnLeft = true;
		isTurnRight = false;

		if (momentable == 0 && level == MARIO_LEVEL_TAIL && isTurnRight == false)
			StartMomentum();
		
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isFlying)
			vy = -MARIO_GRAVITY_TAIL;
		else
			vy = -MARIO_JUMP_SPEED_Y;
		break;
	case MARIO_STATE_IDLE:
		isMomentum = false;
		//momentable = 0;
		vx = 0;
		break;
	
	case MARIO_STATE_FLY:
		isFlying = true;
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

	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_TAIL || level == MARIO_LEVEL_FLY)
	{
		right = x + MARIO_TAIL_BBOX_WIDTH;
		bottom = y + MARIO_TAIL_BBOX_HEIGHT;
	}

}

void Mario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

int Mario::GetLevel()
{
	return level;
}

int Mario::GetAni()
{
	return currentAni;
}

void Mario::SetAni(int ani)
{
	this->currentAni = ani;
}

void Mario::changeAni()
{
	if (state == MARIO_STATE_DIE)
		SetAni(MARIO_ANI_DIE);
	else {
		if (level == MARIO_LEVEL_BIG)
		{
			if (vy >= 0 ) {
				if (vx == 0)
				{
					if (nx > 0) SetAni(MARIO_ANI_BIG_IDLE_RIGHT);
					else SetAni(MARIO_ANI_BIG_IDLE_LEFT);
					//SetAni(MARIO_ANI_BIG_IDLE_LEFT);
				}
				else if (vx > 0)
					SetAni(MARIO_ANI_BIG_WALKING_RIGHT);
				else SetAni(MARIO_ANI_BIG_WALKING_LEFT);
			}

			else if (vy < 0) {
				if (vx > 0 || nx > 0)
				{
					SetAni(MARIO_ANI_BIG_JUMPING_RIGHT);
				}
				else SetAni(MARIO_ANI_BIG_JUMPING_LEFT);
			}


			else if (vy > 0 && collision_y == false) {

				if (vx > 0 || nx>0)
				{
					SetAni(MARIO_ANI_BIG_FALLING_RIGHT);				

				}
				
				else SetAni(MARIO_ANI_BIG_FALLING_LEFT);
			}
		}

		else if (level == MARIO_LEVEL_SMALL)
		{
			if (vy >= 0) {
				if (vx == 0)
				{
					if (nx > 0) SetAni(MARIO_ANI_SMALL_IDLE_RIGHT);
					else SetAni(MARIO_ANI_SMALL_IDLE_LEFT);
				}
				else if (vx > 0)
					SetAni(MARIO_ANI_SMALL_WALKING_RIGHT);
				else SetAni(MARIO_ANI_SMALL_WALKING_LEFT);
			}
			else if (vy < 0) {
				if (vx == 0)
				{
					if (nx > 0) SetAni(MARIO_ANI_SMALL_JUMPING_RIGHT);
					else SetAni(MARIO_ANI_SMALL_JUMPING_LEFT);
				}
				else if (vx > 0)
					SetAni(MARIO_ANI_SMALL_JUMPING_RIGHT);
				else SetAni(MARIO_ANI_SMALL_JUMPING_LEFT);
			}

			else if (vy > 0 && collision_y == false)
			{
				if (vx > 0 || nx > 0)
				{
					SetAni(MARIO_ANI_SMALL_FALLING_RIGHT);
				
				}
			
				else SetAni(MARIO_ANI_SMALL_FALLING_LEFT);
			}
			
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			if (vy >= 0) {
				if (vx == 0)
				{
					if (nx > 0) SetAni(MARIO_ANI_TAIL_IDLE_RIGHT);
					else SetAni(MARIO_ANI_TAIL_IDLE_LEFT);
				}
				else if (vx > 0)
					SetAni(MARIO_ANI_TAIL_WALKING_RIGHT);
				else SetAni(MARIO_ANI_TAIL_WALKING_LEFT);
			}
			else if (vy < 0) {
				if (vx == 0)
				{
					if (nx > 0) SetAni(MARIO_ANI_TAIL_JUMPING_RIGHT);
					else SetAni(MARIO_ANI_TAIL_JUMPING_LEFT);
				}
				else if (vx > 0)
					SetAni(MARIO_ANI_TAIL_JUMPING_RIGHT);
				else SetAni(MARIO_ANI_TAIL_JUMPING_LEFT);
			}

			else if (vy > 0 && collision_y == false)
			{
				if (vx > 0 || nx > 0)
				{
					SetAni(MARIO_ANI_TAIL_FALLING_RIGHT);					
				}	
				else SetAni(MARIO_ANI_TAIL_FALLING_LEFT);
			}
		}
		if (level == MARIO_LEVEL_FIRE)
		{
			if (vy >= 0 ) {
				if (vx == 0)
				{
					if (nx > 0) SetAni(MARIO_ANI_IDLE_FIRE_RIGHT);
					else SetAni(MARIO_ANI_IDLE_FIRE_LEFT);
				}
				else if (vx > 0)
					SetAni(MARIO_ANI_WALKING_FIRE_RIGHT);
				else SetAni(MARIO_ANI_WALKING_FIRE_LEFT);
			}

			else if (vy < 0) {
				if (vx > 0 || nx > 0)
				{
					SetAni(MARIO_ANI_JUMPING_FIRE_RIGHT);
				}
				else SetAni(MARIO_ANI_JUMPING_FIRE_LEFT);
			}

			else if (vy > 0 && collision_y == false) {

				if (vx > 0 || nx > 0)
				{
					SetAni(MARIO_ANI_BIG_FALLING_RIGHT);

				}
				else SetAni(MARIO_ANI_BIG_FALLING_LEFT);
			}
		}
		else if (level == MARIO_LEVEL_FLY)
		{
			if (vy >= 0) {
				if (vx > 0 )
				{
					SetAni(MARIO_ANI_BE_FALL_RIGHT);

				}
				else SetAni(MARIO_ANI_BE_FALL_LEFT);
			}
			else if (vy < 0) {
				if (vx > 0 || nx > 0)
				{
					SetAni(MARIO_ANI_FLY_RIGHT);
				}
				else
					SetAni(MARIO_ANI_FLY_RIGHT);

			}

		}
	}
}

void Mario::NoCollisionWithAxisY()
{

}

bool Mario::isCollisionWithItem(LPGAMEOBJECT item)
{
	if (item->GetFinish() == true)
		return false;

	float l, t, r, b;
	float l1, t1, r1, b1;
	this->GetBoundingBox(l, t, r, b);  // lấy BBOX của mario

	item->GetBoundingBox(l1, t1, r1, b1);
	if (Game::GetInstance()->checkAABB(l, t, r, b, l1, t1, r1, b1) == true)
	{
		return true; // check with AABB
	}

	return false;
}

void Mario::SetHodingItem(LPGAMEOBJECT item)
{
	if (vx > 0 || nx > 0) {
		item->x = x + MARIO_BIG_BBOX_WIDTH - 5;
		item->y = y + 8;
	}
	else {
		item->x = x-8;
		item->y = y + 8;
	}
}