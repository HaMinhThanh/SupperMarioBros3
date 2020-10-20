#include "Sprites.h"
#include "Game.h"
#include "debug.h"

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

void Sprites::AddSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new Sprite(id, left, top, right, bottom, tex);
	sprites[id] = s;
}

LPSPRITE Sprites::Get(int id)
{
	return sprites[id];
}

