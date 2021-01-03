#pragma once

#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "HUD.h"

#include "Brick.h"
#include "BrickGold.h"
#include "BrickQuesion.h"
#include "Portal.h"
#include "BrickColor.h"

#include "mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Venus.h"


#include "Coin.h"
#include "Leaf.h"

#include "FireBall.h"

#include "Node.h"

#include "Utils.h"

#include <map>

class PlayScene: public Scene
{
public:
	
	Mario* mario;

	HUD* HUD;

	vector<LPGAMEOBJECT> BackGround;
	vector<LPGAMEOBJECT> Objects;
	vector<LPGAMEOBJECT> Items;
	vector<LPGAMEOBJECT> Enemy;	
	vector<LPGAMEOBJECT> Weapon;
	vector<LPGAMEOBJECT> Bicks;

	vector<LPGAMEOBJECT> Nodes;
	Node* currentNode = NULL;

	std::map<int, bool> keys;

	void ParseSection_Textures(string line);
	void ParseSection_Sprites(string line);
	void ParseSection_Animations(string line);
	void ParseSection_Animation_Sets(string line);
	void ParseSection_Objects(string line);
	void ParseSection_Items(string line);
	void ParseSection_Enemy(string line);
	void ParseSection_BackGround(string line);

	D3DCOLOR *background;

public:

	PlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void checkCollisionWithItem();
	void checkCollisionWithEnemy();
	void checkCollisionWithBrick();
	void checkCollisionEnemyWithBrick();
	void checkCollisionEnemyWithEnemy();

	void useFireBall();

	void checkMarioWorldMap();
	bool isNoWeight = false;

	Mario* GetPlayer() { return mario; }

	bool isCollision;
	bool isNotDie;
	
};

class PlayScenceKeyHandler : public ScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	PlayScenceKeyHandler(Scene* s) :ScenceKeyHandler(s) {};

	void useWeapon();
};

