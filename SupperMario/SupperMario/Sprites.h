#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

using namespace std;

class Sprite
{
	int id;

	int left;
	int top;
	int right;
	int bottom;

	LPDIRECT3DTEXTURE9 texture;
public:
	Sprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);

	void Draw(float x, float y, int alpha = 255);
	void DrawFlipX(float x, float y, int alpha = 255);
	void DrawCam(float x, float y, int alpha = 255);

	int getHeight() { return bottom - top; }
	int getwidth() { return right - left; }
};

typedef Sprite* LPSPRITE;

class Sprites
{
	static Sprites* _instance;

	unordered_map<int, LPSPRITE> sprites;

public:
	void AddSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	LPSPRITE Get(int id);

	void Clear();

	static Sprites* GetInstance();
};



