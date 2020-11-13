#include <algorithm>
#include "Game.h"
#include "Mario.h"
#include "Sprites.h"
#include "MarioState.h"
#include "MarioData.h"
#include "MarioFalling.h"
#include "MarioJumping.h"
#include "MarioWalking.h"
#include "MarioDie.h"
#include "MarioIdle.h"
//#include "MarioRunning.h"

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

	SetState(MARIO_LEVEL_SMALL);
	/*this->marioData = new MarioData();
	this->marioData->mario = this;*/

	start_x = x;
	start_y = y;
	this->vx = x;
	this->vy = y;

	StartMomentum();
	//StartFlyable();
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

	/*if (vx > 0)
		if (momentable == 0)
			StartMomentum();

	if (vx < 0)
		if (momentable == 0)
			StartMomentum();*/

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
		}
	}

	
	if (coEvents.size() == 0)
	{		
		collision = false;

		x += dx;
		y += dy;
		
	}
	else
	{
		noCollision = true;
		collision = true;

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
		}
		if (ny != 0)
		{
			vy = 0;
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	
}

void Mario::Render()
{
	changeAni();

	if (isAllowKick) {
		SetAni(MARIO_ANI_BIG_KICK_RIGHT);
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


	int alpha = 255;
	if (untouchable) alpha = 128;

 	animation_set->at(currentAni)->Render(x, y, alpha);

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

		if (momentable == 0)
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

void Mario::SetStateName(MarioState* newState)
{
	allowMoveLeft = true;
	allowMoveRight = true;

	//if (this->marioData->state != NULL)
	delete this->marioData->state;

	this->marioData->state = newState;

	this->changeAnimation(newState);

	CurrentState = newState;
	
	mCurrentState = newState->GetState();

}

void Mario::changeAnimation(MarioState *state)
{
	this->marioData->state->changeAnimation();
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
		if (level == MARIO_LEVEL_BIG|| level == MARIO_LEVEL_FIRE)
		{
			if (vy >= 0 || collision) {
				if (vx == 0)
				{
					if (nx > 0) SetAni(MARIO_ANI_BIG_IDLE_RIGHT);
					else SetAni(MARIO_ANI_BIG_IDLE_LEFT);
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


			else if (vy > 0 && collision == false) {

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

			else if (vy > 0 && collision == false)
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

			else if (vy > 0 && collision == false)
			{
				if (vx > 0 || nx > 0)
				{
					SetAni(MARIO_ANI_TAIL_FALLING_RIGHT);					
				}	
				else SetAni(MARIO_ANI_TAIL_FALLING_LEFT);
			}
		}
		/*if (level == MARIO_LEVEL_FIRE)
		{
			if (vy >= 0 || collision) {
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
					SetAni(MARIO_ANI_BIG_JUMPING_RIGHT);
				}
				else SetAni(MARIO_ANI_BIG_JUMPING_LEFT);
			}

			else if (vy > 0 && collision == false) {

				if (vx > 0 || nx > 0)
				{
					SetAni(MARIO_ANI_BIG_FALLING_RIGHT);

				}
				else SetAni(MARIO_ANI_BIG_FALLING_LEFT);
			}
		}
		else if (level = MARIO_LEVEL_FLY)
		{
		if (vy >= 0) {
			if (vx > 0 || nx > 0)
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

		else if (vy > 0 && collision == false)
		{
			if (vx > 0 || nx > 0)
			{
				SetAni(MARIO_ANI_TAIL_FALLING_RIGHT);
			}
			else SetAni(MARIO_ANI_TAIL_FALLING_LEFT);
		}

		}*/
	}
}


void Mario::OnKeyDown(int key)
{
	if (key == DIK_SPACE)
	{		
		if (mCurrentState == MarioState::Walking || mCurrentState == MarioState::Idle)
			{
			this->SetStateName(new MarioJumping(this->marioData));
			}		
		
	}
	this->marioData->state->HandleKeyboard(key);
}

void Mario::OnKeyUp(int key)
{
	if (key == VK_SPACE)
		allowJump = true;
}

void Mario::KeyState(BYTE* states)
{
	Game* game = Game::GetInstance();

	if (mCurrentState == MarioState::Die) return;
	if (game->IsKeyDown(DIK_RIGHT)) 
	{
		if (mCurrentState == MarioState::Idle)
		{
			//vx = MARIO_WALKING_SPEED;
			SetStateName(new MarioWalking(this->marioData));
		}
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (mCurrentState == MarioState::Idle)
		{
			//vx = -MARIO_WALKING_SPEED;
			SetStateName(new MarioWalking(this->marioData));
		}
	}
	else
		SetStateName(new MarioIdle(this->marioData));
}

void Mario::NoCollisionWithAxisY()
{
	if (mCurrentState != MarioState::Jumping && mCurrentState != MarioState::Falling)
	{
		this->SetStateName(new MarioFalling(this->marioData));
	}
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