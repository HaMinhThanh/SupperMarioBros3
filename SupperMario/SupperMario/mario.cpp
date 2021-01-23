#include <algorithm>
#include "Game.h"
#include "Mario.h"
#include "Sprites.h"


#include "Goomba.h"
#include "BrickGold.h"
#include "BrickQuesion.h"
#include "PlayScene.h"
#include "ParaGoomba.h"
#include "BrickGreen.h"
#include "MovingBrick.h"

Mario* Mario::instance = NULL;

Mario* Mario::GetInstance(float x, float y)
{
	if (instance == NULL)
		instance = new Mario(x, y);

	return instance;
}

Mario::Mario(float x, float y) :GameObject()
{
	level = MARIO_LEVEL_SMALL;
	backupLevel = level;
	untouchable = 0;
	momentable = 0;
	flyable = 0;
	kicking = 0;
	swing = 0;
	drift = 0;
	maxjumping = 0;
	time_maxjumping = 0;
	waitToReset = 0;
	time_to_reset = 0;

	isSpeedUp = false;
	isPrepareSpeedUp = false;
	isPowerUp = false;

	stop = 0;
	time_stop = 0;
	isCollising = false;

	SetState(MARIO_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->vx = 0;
	this->vy = 0;

	score = 0;
	live = 4;
	dola = 0;
	item = 0;
	metter = 0;

	currentNode = NULL;
}

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float xCam, yCam;
	Game::GetInstance()->GetCamPos(xCam, yCam);

	if (Game::GetInstance()->GetCurrentSceneId() != 4)
	{
		if (x <= xCam)
		{
			x = xCam;
		}

		if (y > 500)
		{
			SetState(MARIO_STATE_DIE);
		}
	}

	if (state == MARIO_STATE_DIE )
	{
		if (waitToReset == 0)
		{
			if (live > 0)
			{
				live -= 1;
				StartReset();
			}
			else
			{

				Game::GetInstance()->SwitchScene(3);
				live = 4;
				Reset();
				return;
			}
		}
		
	}

	if (isTurnToBig) {
		y -= MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;
		isTurnToBig = false;
	}
	if (isTurnToTail) {
		y -= MARIO_TAIL_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;
		isTurnToTail = false;
	}

	if (isComeUp)
	{
		SetPosition(2325, 350);

		isComeUp = false;
	}

	if (isAutoGo)
		SetState(MARIO_STATE_WALKING_RIGHT);

	/*if (x >= 2816 && isAutoGo) {
		isAutoGo = false;
		Reset();
	}*/

	if (vy == 0 && isFlying == false)
		isJumping = true;

	if (isDecreaseX)
	{
		if (nx < 0 || vx < 0)
		{
			x -= 9;

			isTurnBackX = true;
		}

		isDecreaseX = false;
	}

	if (vy > 0 && isWagging && level == MARIO_LEVEL_TAIL && isJumping == false)
	{
		vy -= 0.00075f * dt;
	}
	else
		isWagging = false;


	if (vx == 0 || vy == MARIO_JUMP_SPEED_Y || isWagging)
		momentable = 0;


	if (metter >= 7)
	{
		metter = 7;
	}
	else if (metter <= 0)
	{
		metter = 0;
	}

	if (isCrouch)
	{
		SetState(MARIO_STATE_IDLE);
	}
	else if (isTurnBack == 1)
	{
		isTurnBack = 0;
		y -= 10;
	}

	if (isAllowSwing)
	{
		isAllowHold = false;
		isAllowMoment = false;
	}

	if (isHoldingItem)
	{
		isAllowMoment = false;
		isFlying = false;
		momentable = 0;
	}

	if (isDrift)
	{
		vx = 0;
	}

	if (maxjumping == 1)
	{
		if (GetTickCount() - time_maxjumping > 100 && GetTickCount() - time_maxjumping < 200)
		{
			vy = -0.15f;
		}
		else if (GetTickCount() - time_maxjumping >= 200)
		{
			vy = -0.24f;
			maxjumping = 0;
			time_maxjumping = 0;
		}
	}
	else
	{
		maxjumping = 0;
		time_maxjumping = 0;
	}

	if (vy == 0)
	{
		isMaxJumping = true;
	}

	if (isPowerUp)
	{
		//vy = -MARIO_JUMP_MAX_SPEED;
	}

	if (waitToReset == 1)
	{
		if (GetTickCount() - time_to_reset > 2000)
		{
			waitToReset = 0;
			time_to_reset = 0;

			Reset();
			return;
		}
	}

	if (stop == 1)
	{
		
		if (GetTickCount() - time_stop > MARIO_TIME_TO_STOP)
		{
			stop = 0;
			time_stop = 0;
			SetState(MARIO_STATE_IDLE);
		}
	}

	vector<LPGAMEOBJECT> Bricks;
	vector<LPGAMEOBJECT> Enemies;

	for (int i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<Brick*>(coObjects->at(i))
			|| dynamic_cast<BrickColor*>(coObjects->at(i))
			|| dynamic_cast<BrickGold*>(coObjects->at(i))
			|| dynamic_cast<BrickQuesion*>(coObjects->at(i))
			|| dynamic_cast<BrickGreen*>(coObjects->at(i))
			|| dynamic_cast<MovingBrick*>(coObjects->at(i)))
			//|| dynamic_cast<Koopas*>(coObjects->at(i)))
		{
			Bricks.push_back(coObjects->at(i));
		}
		else if (dynamic_cast<Koopas*>(coObjects->at(i))
			|| dynamic_cast<Goomba*>(coObjects->at(i))
			|| dynamic_cast<Venus*>(coObjects->at(i))
			|| dynamic_cast<ParaGoomba*>(coObjects->at(i)))
		{
			Enemies.push_back(coObjects->at(i));
		}
	}

	GameObject::Update(dt);

	if (Game::GetInstance()->GetCurrentSceneId() != 3)
		vy += MARIO_GRAVITY * dt;



	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(&Bricks, coEvents);

	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (momentable == 1) {

		metter += 0.1f;

		if (GetTickCount() - momentable_start > MARIO_MOMENTUM_TIME) {

			momentable_start = 0;
			momentable = 0;
			isMomentum = false;
			metter = 7;

			backupLevel = level;

			if (level == MARIO_LEVEL_SMALL)
			{
				isPowerUp = true;
			}
			else if (level == MARIO_LEVEL_BIG)
			{
				isPowerUp = true;
			}
			else if (level == MARIO_LEVEL_TAIL)
			{
				StartFlyable();
			}

			isSpeedUp = false;
		}
		else if (GetTickCount() - momentable_start > 800)
		{
			isSpeedUp = true;
			isPrepareSpeedUp = false;
		}
		else if (GetTickCount() - momentable_start > 200)
		{
			isPrepareSpeedUp = true;
		}

	}
	else
	{
		if (isFlying == false)
			metter -= 1;

		isSpeedUp = false;
		isPrepareSpeedUp = false;
	}


	if (isFlying)
	{

		if (GetTickCount() - flyable_start > MARIO_FLY_TIME)
		{
			isFlying = false;
			flyable = 0;
			flyable_start = 0;
			isMomentum = true;

			level = MARIO_LEVEL_TAIL;

		}

	}

	if (GetTickCount() - kicking_start > MARIO_KICKING_TIME)
	{
		kicking = 0;
		kicking_start = 0;
		isAllowKick = false;
	}

	if (GetTickCount() - swing_start > MARIO_SWING_TIME)
	{
		swing = 0;
		swing_start = 0;
		isAllowSwing = false;

		if (isTurnBackX)
		{
			x += 9;
			isTurnBackX = false;
		}
	}

	if (GetTickCount() - collisWithMushroom_start > 1100)
	{
		collisWithMushroom = 0;
		collisWithMushroom_start = 0;
		isCollisionWithMushroom = false;
	}

	if (drift == 1)
	{
		if (GetTickCount() - time_drift > 200)
		{
			drift = 0;
			time_drift = 0;
			isDrift = false;
		}
	}

	if (coEvents.size() == 0)
	{
		noCollision = true;

		collision_x = false;
		collision_y = false;

		x += dx;
		y += dy;

	}
	else
	{
		noCollision = false;


		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<BrickColor*>(e->obj))
			{
				//BrickColor* b = dynamic_cast<BrickColor*>(e->obj);

				nx = 0;

				if (e->ny == 1)
				{
					ny = 0;
				}
			}
		}
		for (int i = 0; i < coEvents.size(); i++)
		{
			LPCOLLISIONEVENT e = coEvents[i];

			if (dynamic_cast<Koopas*>(e->obj))
			{
				if (isAllowSwing && isCollisionWithItem(dynamic_cast<Koopas*>(e->obj)))
				{
					score += 100;

					dynamic_cast<Koopas*>(e->obj)->SetState(KOOPAS_STATE_IDLE);
					dynamic_cast<Koopas*>(e->obj)->level = KOOPAS_LEVEL_DIE_UP;
				}

				if (isAllowHold && dynamic_cast<Koopas*>(e->obj)->GetState() == KOOPAS_STATE_IDLE
					&& isCollisionWithItem(dynamic_cast<Koopas*>(e->obj)))
				{
					isHoldingItem = true;

					if (isHoldingItem)
					{
						SetHodingItem(dynamic_cast<Koopas*>(e->obj));

					}
				}
				else  if (dynamic_cast<Koopas*>(e->obj)->GetState() == KOOPAS_STATE_IDLE
					&& isAllowHold == false
					&& isAllowSwing == false
					&& isCollisionWithItem(dynamic_cast<Koopas*>(e->obj)))
				{
					if (kicking == 0)
					{
						StartKick();
					}

					if (vx > 0 || nx > 0)
					{
						dynamic_cast<Koopas*>(e->obj)->SetState(KOOPAS_STATE_WALKING_RIGHT);
					}
					else
					{
						dynamic_cast<Koopas*>(e->obj)->SetState(KOOPAS_STATE_WALKING_LEFT);
					}
				}

				if (e->t > 0 && e->t <= 1) {

					// jump on top >> kill Goomba and deflect a bit 
					if (e->ny < 0)
					{
						score += 100;
						vy = -MARIO_JUMP_DEFLECT_SPEED;

						if (dynamic_cast<Koopas*>(e->obj)->GetState() != KOOPAS_STATE_IDLE)
						{
							if (dynamic_cast<Koopas*>(e->obj)->level == KOOPAS_LEVEL_WING)
							{
								dynamic_cast<Koopas*>(e->obj)->level = KOOPAS_LEVEL_NORMAL;
							}
							else if (dynamic_cast<Koopas*>(e->obj)->level == KOOPAS_LEVEL_NORMAL)
							{
								dynamic_cast<Koopas*>(e->obj)->level = KOOPAS_LEVEL_DIE_DOWN;
							}

							dynamic_cast<Koopas*>(e->obj)->SetState(KOOPAS_STATE_IDLE);

						}
						else
						{
							if (dynamic_cast<Koopas*>(e->obj)->x >= x)
							{
								dynamic_cast<Koopas*>(e->obj)->SetState(KOOPAS_STATE_WALKING_RIGHT);
							}
							else
							{
								dynamic_cast<Koopas*>(e->obj)->SetState(KOOPAS_STATE_WALKING_LEFT);
							}
						}

					}
					else if (e->nx != 0)
					{
						if (untouchable == 0)
						{
							if (dynamic_cast<Koopas*>(e->obj)->GetState() != KOOPAS_STATE_IDLE)
							{
								if (level > MARIO_LEVEL_SMALL)
								{
									level--;
									StartUntouchable();
								}
								else
								{
									SetState(MARIO_STATE_DIE);
								}
							}
						}
					}
				}
			}
		}

		if (rdx != 0 && rdx != dx)
			x += nx * abs(rdx);

		x += min_tx * dx + nx * 0.04f;
		y += min_ty * dy + ny * 0.04f;

		if (nx != 0)
		{
			vx = 0;
			collision_x = true;

			//isPowerUp = false;
		}
		else
			collision_x = false;

		if (nx != 0 && ny == 0)
			isPowerUp = false;

		if (ny != 0 && nx == 0)
		{
			vy = 0;
			collision_y = true;
		}
		else
			collision_y = false;
	}

	//CollisionWithEnemy(dt, Enemies);

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}


void Mario::Render()
{
	changeAni();

	if (isAllowKick)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			if (vx == 0)
			{
				if (nx > 0) SetAni(MARIO_ANI_BIG_KICK_RIGHT);
				else SetAni(MARIO_ANI_BIG_KICK_LEFT);
			}
			else if (vx > 0)
				SetAni(MARIO_ANI_BIG_KICK_RIGHT);
			else SetAni(MARIO_ANI_BIG_KICK_LEFT);
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			if (vx == 0)
			{
				if (nx > 0) SetAni(MARIO_ANI_TAIL_KICK_RIGHT);
				else SetAni(MARIO_ANI_TAIL_KICK_LEFT);
			}
			else if (vx > 0)
				SetAni(MARIO_ANI_TAIL_KICK_RIGHT);
			else SetAni(MARIO_ANI_TAIL_KICK_LEFT);
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (vx == 0)
			{
				if (nx > 0) SetAni(MARIO_ANI_SMALL_KICK_RIGHT);
				else SetAni(MARIO_ANI_SMALL_KICK_LEFT);
			}
			else if (vx > 0)
				SetAni(MARIO_ANI_SMALL_KICK_RIGHT);
			else SetAni(MARIO_ANI_SMALL_KICK_LEFT);
		}
	}
	else if (isHoldingItem)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			if (vx == 0)
			{
				if (nx > 0) SetAni(MARIO_ANI_IDLE_BIG_HOLD_RIGHT);
				else SetAni(MARIO_ANI_IDLE_BIG_HOLD_LEFT);
			}
			else if (vx > 0)
				SetAni(MARIO_ANI_WALKING_BIG_HOLD_RIGHT);
			else SetAni(MARIO_ANI_WALKING_BIG_HOLD_LEFT);
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			if (vx == 0)
			{
				if (nx > 0) SetAni(MARIO_ANI_IDLE_TAIL_HOLD_RIGHT);
				else SetAni(MARIO_ANI_IDLE_TAIL_HOLD_LEFT);
			}
			else if (vx > 0)
				SetAni(MARIO_ANI_WALKING_TAIL_HOLD_RIGHT);
			else SetAni(MARIO_ANI_WALKING_TAIL_HOLD_LEFT);
		}
	}
	else if (isAllowSwing)
	{
		if (vx > 0 || nx > 0)
			SetAni(MARIO_ANI_SWING_RIGHT);
		else
			SetAni(MARIO_ANI_SWING_LEFT);
	}

	else if (isWagging)
	{
		if (vx > 0 || nx > 0)
			SetAni(MARIO_ANI_WAGGING_RIGHT);
		else
			SetAni(MARIO_ANI_WAGGING_LEFT);

	}
	else if (isCrouch)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			if (vx > 0 || nx > 0)
				SetAni(MARIO_ANI_BIG_CROUCH_RIGHT);
			else
				SetAni(MARIO_ANI_BIG_CROUCH_LEFT);
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			if (vx > 0 || nx > 0)
				SetAni(MARIO_ANI_TAIL_CROUCH_RIGHT);
			else
				SetAni(MARIO_ANI_TAIL_CROUCH_LEFT);
		}
	}
	else if (isCollisionWithMushroom)
	{
		SetAni(MARIO_ANI_COLLISION_MUSHROOM);
	}
	else if (isPrepareSpeedUp)
	{
		if (level == MARIO_LEVEL_SMALL)
		{
			if (vx > 0 || nx > 0)
				SetAni(MARIO_ANI_PREPARE_SPEED_UP_SMALL_RIGHT);
			else
				SetAni(MARIO_ANI_PREPARE_SPEED_UP_SMALL_LEFT);
		}
		else if (level == MARIO_LEVEL_BIG)
		{
			if (vx > 0 || nx > 0)
				SetAni(MARIO_ANI_PREPARE_SPEED_UP_BIG_RIGHT);
			else
				SetAni(MARIO_ANI_PREPARE_SPEED_UP_BIG_LEFT);
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			if (vx > 0 || nx > 0)
				SetAni(MARIO_ANI_PREPARE_SPEED_UP_TAIL_RIGHT);
			else
				SetAni(MARIO_ANI_PREPARE_SPEED_UP_TAIL_LEFT);

		}
	}
	else if (isSpeedUp)
	{
		if (level == MARIO_LEVEL_SMALL)
		{
			if (vx > 0 || nx > 0)
				SetAni(MARIO_ANI_SPEED_UP_SMALL_RIGHT);
			else
				SetAni(MARIO_ANI_SPEED_UP_SMALL_LEFT);
		}
		else if (level == MARIO_LEVEL_BIG)
		{
			if (vx > 0 || nx > 0)
				SetAni(MARIO_ANI_SPEED_UP_BIG_RIGHT);
			else
				SetAni(MARIO_ANI_SPEED_UP_BIG_LEFT);
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			if (vx > 0 || nx > 0)
				SetAni(MARIO_ANI_SPEED_UP_RIGHT);
			else
				SetAni(MARIO_ANI_SPEED_UP_LEFT);

		}
	}
	else if (isDrift)
	{
		if (level == MARIO_LEVEL_SMALL)
		{
			if (vx < 0 || nx < 0)
				SetAni(MARIO_ANI_SMALL_DRIFT_RIGHT);
			else
				SetAni(MARIO_ANI_SMALL_DRIFT_LEFT);
		}
		else if (level == MARIO_LEVEL_BIG)
		{
			if (vx < 0 || nx < 0)
				SetAni(MARIO_ANI_BIG_DRIFT_RIGHT);
			else
				SetAni(MARIO_ANI_BIG_DRIFT_LEFT);
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			if (vx < 0 || nx < 0)
				SetAni(MARIO_ANI_TAIL_DRIFT_RIGHT);
			else
				SetAni(MARIO_ANI_TAIL_DRIFT_LEFT);

		}
	}
	else if (isPowerUp)
	{
		if (level == MARIO_LEVEL_SMALL)
		{
			if (vx > 0 || nx > 0)
				SetAni(MARIO_ANI_FLY_SMALL_RIGHT);
			else
				SetAni(MARIO_ANI_FLY_SMALL_LEFT);
		}
		else if (level == MARIO_LEVEL_BIG)
		{
			if (vx > 0 || nx > 0)
				SetAni(MARIO_ANI_FLY_BIG_RIGHT);
			else
				SetAni(MARIO_ANI_FLY_BIG_LEFT);
		}
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
		if (isPrepareSpeedUp)
			vx = 0.15f;
		else if (isSpeedUp|| isPowerUp)
			vx = 0.2f;
		else
			vx = MARIO_WALKING_SPEED;

		isTurnLeft = false;
		//isTurnRight = true;

		if (momentable == 0 && isTurnLeft == false && isAllowMoment)
			StartMomentum();
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:

		if (isPrepareSpeedUp)
			vx = -0.15f;
		else if (isSpeedUp || isPowerUp)
			vx = -0.2f;
		else
			vx = -MARIO_WALKING_SPEED;
		//isTurnLeft = true;
		isTurnRight = false;

		if (momentable == 0 && isTurnRight == false && isAllowMoment)
			StartMomentum();

		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isFlying)
		{
			vy = -MARIO_GRAVITY_TAIL;
		}
		else if (isPowerUp)
		{
			vy = -MARIO_JUMP_MAX_SPEED;
		}
		else 
			vy = -0.25f;
		break;

	case MARIO_STATE_IDLE:
		isMomentum = false;
		//momentable = 0;
		vx = 0;
		//vy = 0;
		break;

	case MARIO_STATE_FLY:
		isFlying = true;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_STOP:
		if (stop == 0) {
			if (vx > 0)
				vx = 0.01;
			else if (vx < 0)
				vx = -0.01;

			StartStop();
		}
		break;
	}

}

void Mario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (GetState() == MARIO_STATE_DIE)
	{
		return;
	}

	left = x;
	top = y;

	if (Game::GetInstance()->GetCurrentSceneId() != 3)
	{
		if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE)
		{
			right = x + MARIO_BIG_BBOX_WIDTH;
			if (isCrouch)
			{
				bottom = y + MARIO_CROUCH_BBOX_HEIGHT;
			}
			else
			{
				bottom = y + MARIO_BIG_BBOX_HEIGHT;
			}
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			right = x + MARIO_SMALL_BBOX_WIDTH;
			bottom = y + MARIO_SMALL_BBOX_HEIGHT;
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			if (isAllowSwing)
			{
				right = x + MARIO_TAIL_SWING_BBOX_WIDTH;
			}
			else
			{
				right = x + MARIO_TAIL_BBOX_WIDTH;
			}


			if (isCrouch)
			{
				bottom = y + MARIO_CROUCH_BBOX_HEIGHT;
			}
			else
			{
				bottom = y + MARIO_TAIL_BBOX_HEIGHT;
			}
		}
		else if (level == MARIO_LEVEL_FLY)
		{
			if (backupLevel == MARIO_LEVEL_SMALL)
			{
				right = x + MARIO_SMALL_BBOX_WIDTH;
				bottom = y + MARIO_SMALL_BBOX_HEIGHT;
			}
			else if (backupLevel == MARIO_LEVEL_BIG)
			{
				right = x + MARIO_BIG_BBOX_WIDTH;
				bottom = y + MARIO_BIG_BBOX_HEIGHT;
			}
			else
			{
				right = x + MARIO_TAIL_BBOX_WIDTH;
				bottom = y + MARIO_TAIL_BBOX_HEIGHT;
			}
		}
	}
	else
	{
		right = x + 14;
		bottom = y + 16;
	}

}

void Mario::Reset()
{
	//Mario* mario = Mario::GetInstance(0, 0);
	//currentNode->SetDirectNode(15, 32, 0, 0, 1, 0, 0);
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(15,32);
	SetSpeed(0, 0);

	int id = Game::GetInstance()->GetCurrentSceneId();
	Game::GetInstance()->SwitchScene(id);
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
			if (vy >= 0) {
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
			else if (vy < 0)
			{
				if (vx > 0 || nx > 0)
				{
					SetAni(MARIO_ANI_BIG_JUMPING_RIGHT);
				}
				else SetAni(MARIO_ANI_BIG_JUMPING_LEFT);
			}
			else if (vy > 0) {

				if (vx > 0 || nx > 0)
				{
					SetAni(MARIO_ANI_BIG_FALLING_RIGHT);
				}
				else SetAni(MARIO_ANI_BIG_FALLING_LEFT);
			}
			else {
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
			if (vy >= 0)
			{
				if (vx == 0)
				{
					if (nx > 0) SetAni(MARIO_ANI_TAIL_IDLE_RIGHT);
					else SetAni(MARIO_ANI_TAIL_IDLE_LEFT);
				}
				else if (vx > 0)
					SetAni(MARIO_ANI_TAIL_WALKING_RIGHT);
				else SetAni(MARIO_ANI_TAIL_WALKING_LEFT);
			}
			else if (vy < 0)
			{
				if (vx == 0)
				{
					if (nx > 0) SetAni(MARIO_ANI_TAIL_JUMPING_RIGHT);
					else SetAni(MARIO_ANI_TAIL_JUMPING_LEFT);
				}
				else if (vx > 0)
					SetAni(MARIO_ANI_TAIL_JUMPING_RIGHT);
				else SetAni(MARIO_ANI_TAIL_JUMPING_LEFT);
			}
		}
		if (level == MARIO_LEVEL_FIRE)
		{
			if (vy >= 0)
			{
				if (vx == 0)
				{
					if (nx > 0) SetAni(MARIO_ANI_IDLE_FIRE_RIGHT);
					else SetAni(MARIO_ANI_IDLE_FIRE_LEFT);
				}
				else if (vx > 0)
					SetAni(MARIO_ANI_WALKING_FIRE_RIGHT);
				else SetAni(MARIO_ANI_WALKING_FIRE_LEFT);
			}

			else if (vy < 0)
			{
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
			if (backupLevel == MARIO_LEVEL_TAIL)
			{
				if (vy >= 0)
				{
					if (vx > 0)
					{
						SetAni(MARIO_ANI_BE_FALL_RIGHT);
					}
					else
					{
						SetAni(MARIO_ANI_BE_FALL_LEFT);
					}
				}
				else if (vy < 0)
				{
					if (vx > 0 || nx > 0)
					{
						SetAni(MARIO_ANI_FLY_RIGHT);
					}
					else
					{
						SetAni(MARIO_ANI_FLY_LEFT);
					}
				}
			}
			else if (backupLevel == MARIO_LEVEL_SMALL)
			{
				if (vy >= 0)
				{
					if (vx > 0)
					{
						SetAni(MARIO_ANI_BE_FALL_SMALL_RIGHT);
					}
					else
					{
						SetAni(MARIO_ANI_BE_FALL_SMALL_LEFT);
					}
				}
				else if (vy < 0)
				{
					if (vx > 0 || nx > 0)
					{
						SetAni(MARIO_ANI_FLY_SMALL_RIGHT);
					}
					else
					{
						SetAni(MARIO_ANI_FLY_SMALL_LEFT);
					}
				}
			}
			else if (backupLevel == MARIO_LEVEL_BIG)
			{
				if (vy >= 0)
				{
					if (vx > 0)
					{
						SetAni(MARIO_ANI_BE_FALL_BIG_RIGHT);
					}
					else
					{
						SetAni(MARIO_ANI_BE_FALL_BIG_LEFT);
					}
				}
				else if (vy < 0)
				{
					if (vx > 0 || nx > 0)
					{
						SetAni(MARIO_ANI_FLY_BIG_RIGHT);
					}
					else
					{
						SetAni(MARIO_ANI_FLY_BIG_LEFT);
					}
				}
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
	{
		return false;
	}

	float l, t, r, b;
	float l1, t1, r1, b1;

	this->GetBoundingBox(l, t, r, b);		// lấy BBOX của mario
	item->GetBoundingBox(l1, t1, r1, b1);	// lấy BBOX của item

	if (Game::GetInstance()->checkAABB(l, t, r, b, l1, t1, r1, b1) == true)
	{
		return true; // check with AABB
	}

	return false;
}

void Mario::SetHodingItem(LPGAMEOBJECT item)
{
	if (vx > 0 || nx > 0)
	{
		item->x = x + MARIO_BIG_BBOX_WIDTH - 3;
		item->y = y + 8;
	}
	else
	{
		item->x = x - 8;
		item->y = y + 8;
	}
}

void Mario::LoadItem(int itm)
{
	LPGAMEOBJECT item = NULL;

	switch (itm)
	{

	}
}

void Mario::GetNodePos(float& x, float& y)
{
	if (currentNode == NULL)
	{
		currentNode = new Node(15, 32, 0, 0, 1, 0, 0, 0);
	}
	x = currentNode->x; 
	y = currentNode->y; 
}

void Mario::SwitchMap()
{
	if (currentNode->SceneId != 0)
		Game::GetInstance()->SwitchScene(currentNode->SceneId);
}
