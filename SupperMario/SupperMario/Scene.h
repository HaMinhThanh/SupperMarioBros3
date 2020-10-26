#pragma once

#include <d3dx9.h>
#include <d3d9.h>

#include "KeyEventHandler.h"

class Scene
{
protected:
	int id;	
	KeyEventHandler* keyHandler;
	LPCWSTR sceneFilePath;

	D3DCOLOR  backColor;

public:	

	Scene(int id, LPCWSTR filePath);

	KeyEventHandler* GetKeyEventHandler() { return keyHandler; }

	virtual void Update(DWORD dt) = 0;
	virtual void Load() = 0;
	virtual void Render() = 0;
	virtual void Unload() = 0;
	
	D3DCOLOR GetBackColor();
	
};

typedef Scene* LPSCENE;

class ScenceKeyHandler : public KeyEventHandler
{
protected:
	Scene* scence;

public:
	virtual void KeyState(BYTE* states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	ScenceKeyHandler(Scene* s) :KeyEventHandler() { scence = s; }
};

