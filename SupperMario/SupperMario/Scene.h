#pragma once

#include <d3dx9.h>
#include <d3d9.h>

#include "Game.h"

class Scene
{
public:
	virtual void Update(DWORD dt);
	virtual void LoadContent();
	virtual void Draw();

	virtual void OnKeyDown(int keyCode);
	virtual void OnKeyUp(int keyUp);
	
	D3DCOLOR GetBackColor();

	~Scene();
protected:
	Scene();

	D3DCOLOR  backColor;
};

