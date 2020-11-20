#include "Sprites.h"
#include "Game.h"
#include "Utils.h"

Sprite::Sprite(int id, int left, int top,int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}

Sprites* Sprites::_instance = NULL;

Sprites* Sprites::GetInstance()
{
	if (_instance == NULL) _instance = new Sprites();
	return _instance;
}

void Sprite::Draw(float x, float y, int alpha)
{
	Game* game = Game::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom, alpha);
}

void Sprite::DrawCam(float x, float y, int alpha)
{
	Game* game = Game::GetInstance();
	game->DrawFlipX(x, y, texture, left, top, right, bottom, alpha);
}

void Sprite::DrawFlipX(float x, float y, int alpha) {
	LPD3DXSPRITE spriteHandler = Game::GetInstance()->GetSpriteHandler();

	D3DXMATRIX oldMt;
	spriteHandler->GetTransform(&oldMt);

	D3DXMATRIX newMt;

	D3DXVECTOR2 top_left = D3DXVECTOR2(x, y);

	D3DXVECTOR2 rotate = D3DXVECTOR2(-1, 1);

	D3DXMatrixTransformation2D(&newMt, &top_left, 0.0f, &rotate, NULL, 0.0f, NULL);
	D3DXMATRIX finalMt = newMt * oldMt;

	spriteHandler->SetTransform(&finalMt);

	x -= getwidth();

	this->DrawCam(x, y, alpha);

	spriteHandler->SetTransform(&oldMt);
}

void Sprites::AddSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new Sprite(id, left, top, right, bottom, tex);
	sprites[id] = s;

	if (sprites[id] != NULL)
		DebugOut(L"[INFO] sprite added: %d, %d, %d, %d, %d \n", id, left, top, right, bottom);
}

LPSPRITE Sprites::Get(int id)
{
	return sprites[id];
}

void Sprites::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}

