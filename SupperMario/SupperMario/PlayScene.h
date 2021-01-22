#pragma once

#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "HUD.h"
#include "Grid.h"

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
	Grid* grid;

	vector<LPGAMEOBJECT> coObjectGrid;	// quan li cac object trong cam

	vector<LPGAMEOBJECT> BackGround;
	vector<LPGAMEOBJECT> Objects;
	vector<LPGAMEOBJECT> Items;
	vector<LPGAMEOBJECT> Enemy;	
	vector<LPGAMEOBJECT> Weapon;
	vector<LPGAMEOBJECT> Bicks;
	vector<LPGAMEOBJECT> Effect;
	vector<LPGAMEOBJECT> Nodes;
	vector<LPGAMEOBJECT> Coins;

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
	void ParseSection_Camera(string line);

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
	void checkEndScene();
	void checkTurnGoldBrick();

	void useFireBall();

	void checkMarioWorldMap();
	bool isNoWeight = false;

	Mario* GetPlayer() { return mario; }
	Node* GetNode() { return currentNode; }

	int xLeft, xRight;
	float xCam4 = 0;

	bool isCollision = false;
	//bool isNotDie;
	//bool isEndScene = false;

	DWORD time_endscene = 0;
	int endscene = 0;
	int addtext = 3;

	DWORD time_turngb = 0;
	int turngb = 0;

	void StartEndScene() { endscene = 1; time_endscene = GetTickCount(); }
	void TurnGoldBrick() { turngb = 1; time_turngb = GetTickCount(); }
	
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

