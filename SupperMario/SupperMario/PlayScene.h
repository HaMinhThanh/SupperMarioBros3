#pragma once

#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Venus.h"
#include "BrickColor.h"

#include "Coin.h"
#include "Leaf.h"

#include "Utils.h"

#include <map>

class PlayScene: public Scene
{
public:
	
	Mario* mario;

	vector<LPGAMEOBJECT> Objects;
	vector<LPGAMEOBJECT> Items;
	vector<LPGAMEOBJECT> Enemy;
	vector<LPGAMEOBJECT> ColorBlock;

	std::map<int, bool> keys;

	void ParseSection_Textures(string line);
	void ParseSection_Sprites(string line);
	void ParseSection_Animations(string line);
	void ParseSection_Animation_Sets(string line);
	void ParseSection_Objects(string line);
	void ParseSection_Items(string line);
	void ParseSection_Enemy(string line);
	void ParseSection_ColorBlock(string line);

public:

	PlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void checkCollisionWithItem();
	void checkCollisionWithEnemy();
	void checkCollisionWithColorBlock();

	Mario* GetPlayer() { return mario; }
	
};

class PlayScenceKeyHandler : public ScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	PlayScenceKeyHandler(Scene* s) :ScenceKeyHandler(s) {};
};

