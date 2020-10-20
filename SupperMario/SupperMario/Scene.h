#pragma once

#include <d3dx9.h>
#include <d3d9.h>

#include "Game.h"

class Scene
{
public:
	virtual void Update(DWORD dt) = 0;
	virtual void LoadContent() = 0;
	virtual void Render() = 0;

	virtual void OnKeyDown(int keyCode);
	virtual void OnKeyUp(int keyUp);
	virtual void KeyState(BYTE* state) = 0;
	
	D3DCOLOR GetBackColor();

	~Scene();
protected:
	Scene();

	D3DCOLOR  backColor;
};

