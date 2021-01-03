#include <d3dx9.h>
#include <algorithm>

#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Sprites.h"

//vector<LPANIMATION> GameObject::animations;

GameObject::GameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;
}

void GameObject::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}

LPCOLLISIONEVENT GameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;	//static
	float ml, mt, mr, mb;	//move
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	float rdx = this->dx - sdx;
	float rdy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	Game::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CollisionEvent* e = new CollisionEvent(t, nx, ny,rdx, rdy, coO);
	return e;
}

void GameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i< coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		
		if (e->t > 0 && e->t < 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CollisionEvent::compare);
}

void GameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;

	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) 
		{
			min_tx = c->t;
			nx = c->nx;
			min_ix = i;
			rdx = c->dx;
		}
		if (c->t < min_ty && c->ny != 0)
		{
			min_ty = c->t;
			ny = c->ny;
			min_iy = i;
			rdy = c->dy;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}

void GameObject::FilterCollisionKoopa(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;

	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0)
		{
			min_tx = c->t;
			nx = c->nx;
			min_ix = i;
			rdx = c->dx;
		}
		if (c->t < min_ty && c->ny != 0)
		{
			min_ty = c->t;
			ny = c->ny;
			min_iy = i;
			rdy = c->dy;
		}		
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);

}

GameObject::~GameObject()
{
	
}

void GameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = Textures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	Game::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, 32);
}

void GameObject::SetId(int ID)
{
	this->id = ID;
}

int GameObject::GetId()
{
	return id;
}

float GameObject::GetX()
{
	return x;
}

float GameObject::GetY()
{
	return y;
}

float GameObject::GetVx()
{
	return vx;
}

float GameObject::GetVy()
{
	return vy;
}

float GameObject::GetNx()
{
	return nx;
}

void GameObject::SetX(float X)
{
	this->x = X;
}

void GameObject::SetY(float Y)
{
	this->y = Y;
}

void GameObject::SetVx(float VX)
{
	this->vx = VX;
}

void GameObject::SetVy(float VY)
{
	this->vy = VY;
}

void GameObject::AddVx(float vx)
{
	this->vx += vx;
}

void GameObject::AddVy(float vy)
{
	this->vy += vy;
}

int GameObject::GetState() 
{
	if (this == NULL)
		return -1;
	return state;
}

void GameObject::ReSetAnimationSet(int re)
{
	AnimationSets* animation_sets = AnimationSets::GetInstance();

	LPANIMATION_SET ani_set = animation_sets->Get(re);

	delete this->animation_set;

	this->SetAnimationSet(ani_set);
}