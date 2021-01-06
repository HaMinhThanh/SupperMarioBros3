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
	static IntroScene* instance;

protected:
	Mario* player1;
	Mario* player2;
	Koopas* koopasGreen;
	Koopas* koopasBlack;
	Goomba* goomba;
	Leaf* leaf;	
	Mushroom* mr1;
	Mushroom* mr2;

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> background;

	DWORD time_count = 0;
	DWORD time_start;
	bool isStart = false;

	DWORD time_switch = 0;
	int Switchable = 0;
	int isSwitch = 2;

	void ParseSection_TEXTURES(string line);
	void ParseSection_SPRITES(string line);
	void ParseSection_ANIMATIONS(string line);
	void ParseSection_ANIMATION_SETS(string line);
	void ParseSection_OBJECTS(string line);

public:
	static IntroScene* GetInstance(int id, LPCWSTR filePath);
	IntroScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	vector<LPGAMEOBJECT> GetObjects() { return objects; }
	Mario* GetPlayer1() { return player1; }
	Mario* GetPlayer2() { return player2; }

	int addition = 11;
	bool isHiden = false;
	bool isChangeMap = false;
	bool swichScene = false;

	void SwitchScene() { Switchable = 1; time_switch = GetTickCount(); }
};

class IntroSceneKeyHandler :public ScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* state);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	IntroSceneKeyHandler(Scene* s) : ScenceKeyHandler(s) {};
};

