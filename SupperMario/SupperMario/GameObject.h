#pragma once

#include <Windows.h>
#include<d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"

using namespace std;

#define ID_TEX_BBOX -100

class GameObject;
typedef GameObject* LPGAMEOBJECT;

struct CollisionEvent;
typedef CollisionEvent* LPCOLLISIONEVENT;

struct CollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;

	float dx, dy;

	CollisionEvent(float t, float nx, float ny,float dx=0, float dy=0, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};

enum TypeObject
{
	MARIO = 0,
	GOOMBA = 1,
	KOOPAS = 2
};

class GameObject
{
public:
	float x;
	float y;

	float vx;
	float vy;

	float dx;
	float dy;

	float w, h;

	int nx;

	int state;

	int id;

	bool isFinish;

	DWORD dt;

	LPANIMATION_SET animation_set;

public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }

	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }

	int GetState();

	void SetId(int ID);
	int GetId();

	float GetX();
	float GetY();
	float GetVx();
	float GetVy();
	float GetNx();

	void SetX(float x);
	void SetY(float y);
	void SetVx(float vx);
	void SetVy(float vy);	

	void AddVx(float vx);
	void AddVy(float vy);

	bool GetFinish() { return isFinish; }

	void SetBoundBbox(float a, float b) { this->w = a; this->h = b; }
	void RenderBoundingBox();

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	void ReSetAnimationSet(int re);

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObject, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents,
		vector<LPCOLLISIONEVENT> &coEventsResult,
		float &min_tx,
		float &min_ty,
		float &nx,
		float &ny,
		float& rdx, 
		float& rdy);

	void FilterCollisionKoopa(vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);
	
	GameObject();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects=NULL);
	virtual void Render() = 0;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void SetState(int state) { this->state = state; }

	~GameObject();
};





