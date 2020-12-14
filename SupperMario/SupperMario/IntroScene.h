#pragma once

#include "Game.h"
#include "Scene.h"
#include "PlayScene.h"
#include "Textures.h"
#include "GameObject.h"
#include "mario.h"
#include "Koopas.h"
#include "Goomba.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "Star.h"

class IntroScene: public Scene
{
protected:
	Mario* player1;
	Mario* player2;
	Koopas* koopasGreen;
	Koopas* koopasRed;
	Goomba* goomba;
	Leaf* leaf;

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> background;

	DWORD time_count = 0;
	DWORD time_start;
	bool isStart = false;

	void ParseSection_TEXTURES(string line);
	void ParseSection_SPRITES(string line);
	void ParseSection_ANIMATIONS(string line);
	void ParseSection_ANIMATION_SETS(string line);
	void ParseSection_OBJECTS(string line);

public:
	IntroScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	vector<LPGAMEOBJECT> GetObjects() { return objects; }
	Mario* GetPlayer1() { return player1; }
	Mario* GetPlayer2() { return player2; }
};

class IntroSceneKeyHandler :public ScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* state);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	IntroSceneKeyHandler(Scene* s) : ScenceKeyHandler(s) {};
};

