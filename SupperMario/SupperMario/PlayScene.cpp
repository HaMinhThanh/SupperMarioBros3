#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "HidenWall.h"

#include "Brick.h"
#include "BrickColor.h"
#include "BrickGold.h"
#include "BrickGreen.h"
#include "BrickQuesion.h"
#include "MovingBrick.h"

#include "Koopas.h"
#include "Goomba.h"
#include "Venus.h"
#include "ParaGoomba.h"
#include "BoomerangBro.h"
#include "Boomerang.h"

#include "Leaf.h"
#include "Coin.h"
#include "Mushroom.h"
#include "Star.h"
#include "Button.h"

// world map
#include "Node.h"
#include "person.h"

#include <map>
#include <iostream>
#include "BrickBreak.h"

using namespace std;

#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_ITEMS				6
#define SCENE_SECTION_OBJECTS			7
#define SCENE_SECTION_ENEMY				8
#define SCENE_SECTION_BACKGROUND		9
#define SCENE_SECTION_CAMERA			10

// Player and Brick
#define OBJECT_TYPE_MARIO			0
#define OBJECT_TYPE_BRICK			1
#define OBJECT_TYPE_BRICKQUESION	2
#define OBJECT_TYPE_BRICKGOLD		3
#define OBJECT_TYPE_BRICKGREEN		4
#define OBJECT_TYPE_COLORBLOCK		5
#define OBJECT_TYPE_MOVINGBRICK		6
#define OBJECT_TYPE_PORTAL			50

// back ground
#define OBJECT_TYPE_BACKGROUND		1

// ITEMS
#define ITEM_TYPE_COIN		1
#define ITEM_TYPE_LEAF		2	
#define ITEM_TYPE_MUSHROOM	3
#define ITEM_TYPE_STAR		4
#define ITEM_TYPE_BUTTON	5	

// ENEMY
#define ENEMY_TYPE_GOOMBA		1
#define ENEMY_TYPE_KOOPAS		2
#define ENEMY_TYPE_VENUS		3
#define ENEMY_TYPE_PARAKOOPAS	4
#define ENEMY_TYPE_PARAGOOMBA	5
#define ENEMY_TYPE_BOOMERANGBRO 6

#define OBJECT_TYPE_NODES	40
#define OBJECT_TYPE_PERSON	41

#define MAX_SCENE_LINE 1024

PlayScene::PlayScene(int id, LPCWSTR filePath) :
	Scene(id, filePath)
{
	keyHandler = new PlayScenceKeyHandler(this);

}


void PlayScene::ParseSection_Textures(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	Textures::GetInstance()->AddTexture(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));

}

void PlayScene::ParseSection_Sprites(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = Textures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	Sprites::GetInstance()->AddSprite(ID, l, t, r, b, tex);
}

void PlayScene::ParseSection_Animations(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	LPANIMATION ani = new Animation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->AddAnimation(sprite_id, frame_time);
	}

	Animations::GetInstance()->AddAnimations(ani_id, ani);
}

void PlayScene::ParseSection_Animation_Sets(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new AnimationSet();

	Animations* animations = Animations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	AnimationSets::GetInstance()->AddAnimationSet(ani_set_id, s);
}

void PlayScene::ParseSection_BackGround(string line)
{
	//int ani;
	vector<string> tokens = split(line);

	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() == 3) {
		int R = atoi(tokens[0].c_str());
		int G = atoi(tokens[1].c_str());
		int B = atoi(tokens[2].c_str());

		Game::GetInstance()->background = D3DCOLOR_XRGB(R, G, B);
	}
	else {

		int object_type = atoi(tokens[0].c_str());
		float x = atof(tokens[1].c_str());
		float y = atof(tokens[2].c_str());

		int ani_set_id = atoi(tokens[3].c_str());

		AnimationSets* animation_sets = AnimationSets::GetInstance();

		GameObject* obj = NULL;

		switch (object_type)
		{
		case OBJECT_TYPE_BACKGROUND:
			obj = new HidenWall();
			break;

		default:
			DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
			return;
		}

		// General object setup
		obj->SetPosition(x, y);

		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);

		BackGround.push_back(obj);
	}
}

void PlayScene::ParseSection_Camera(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 2)return;	// skip invalid lines
	xLeft = atoi(tokens[0].c_str());
	xRight = atoi(tokens[1].c_str());
}

/*
	Parse a line in section [OBJECTS]
*/
void PlayScene::ParseSection_Objects(string line)
{
	bool hd = false;

	bool b = false;

	int ani;
	vector<string> tokens = split(line);

	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	float w, h;
	int item;
	bool isNode = false;

	int ani_set_id = atoi(tokens[3].c_str());

	AnimationSets* animation_sets = AnimationSets::GetInstance();

	GameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		obj = Mario::GetInstance(x, y);
		if (mario != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		//obj = new Mario(x, y);
		//mario = (Mario*)obj;
		mario = Mario::GetInstance(x, y);

		DebugOut(L"[INFO] Player object created!\n");
		break;

	case OBJECT_TYPE_BRICK:

		obj = new Brick();
		break;

	case OBJECT_TYPE_BRICKQUESION:

		item = atoi(tokens[4].c_str());
		obj = new BrickQuesion(x, y, item);
		break;

	case OBJECT_TYPE_BRICKGOLD:
	{
		item = atoi(tokens[4].c_str());
		obj = new BrickGold(x, y, item);
	}
	break;

	case OBJECT_TYPE_BRICKGREEN:

		obj = new BrickGreen();
		break;

	case OBJECT_TYPE_COLORBLOCK:

		w = atof(tokens[4].c_str());
		h = atof(tokens[5].c_str());

		obj = new BrickColor();
		obj->SetBoundBbox(w, h);

		break;

	case OBJECT_TYPE_MOVINGBRICK:

		obj = new MovingBrick();
		break;

	case OBJECT_TYPE_NODES:
	{
		int l = atof(tokens[4].c_str());
		int t = atof(tokens[5].c_str());
		int r = atof(tokens[6].c_str());
		int b = atof(tokens[7].c_str());
		int tp = atof(tokens[8].c_str());
		int id = atof(tokens[9].c_str());

		obj = new Node(x, y, l, t, r, b, tp, id);
		isNode = true;
	}
	break;

	case OBJECT_TYPE_PERSON:
	{
		float max = atof(tokens[4].c_str());
		float min = atof(tokens[5].c_str());

		obj = new person(max, min);

	}
	break;

	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());

		obj = new Portal(x, y, r, b, scene_id);
	}

	break;

	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);

	if (isNode)
	{
		Nodes.push_back(obj);
	}
	else if (dynamic_cast<Mario*>(obj))
	{

	}
	else
	{
		Objects.push_back(obj);
	}
}

void PlayScene::ParseSection_Items(string line)
{
	vector<string> tokens = split(line);

	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	AnimationSets* animation_sets = AnimationSets::GetInstance();

	GameObject* item = NULL;

	switch (object_type)
	{
	case ITEM_TYPE_COIN:
		item = new Coin();
		break;

	case ITEM_TYPE_LEAF:
		item = new Leaf();
		break;

	case ITEM_TYPE_STAR:
		item = new Star();
		break;

	case ITEM_TYPE_BUTTON:
		item = new Button();
		break;

	default:
		//DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	item->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	item->SetAnimationSet(ani_set);
	Items.push_back(item);
}

void PlayScene::ParseSection_Enemy(string line)
{
	vector<string> tokens = split(line);

	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	int level, type;
	int max, min, fire;

	float maxX;
	float minX;

	AnimationSets* animation_sets = AnimationSets::GetInstance();

	GameObject* enemy = NULL;

	switch (object_type)
	{
	case ENEMY_TYPE_GOOMBA:
		maxX = atoi(tokens[4].c_str());
		minX = atoi(tokens[5].c_str());
		enemy = new Goomba(maxX, minX);
		break;

	case ENEMY_TYPE_KOOPAS:
		level = atoi(tokens[4].c_str());
		type = atoi(tokens[5].c_str());
		enemy = new Koopas(x, y, level, type);
		break;
	case ENEMY_TYPE_VENUS:
		max = atoi(tokens[4].c_str());
		min = atoi(tokens[5].c_str());
		fire = atoi(tokens[6].c_str());
		enemy = new Venus(max, min, fire);
		break;
	case ENEMY_TYPE_PARAGOOMBA:
		enemy = new ParaGoomba();
		break;
	case ENEMY_TYPE_BOOMERANGBRO:
		enemy = new BoomerangBro();
		break;
	default:
		//DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General enemy setup
	enemy->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	enemy->SetAnimationSet(ani_set);
	Enemy.push_back(enemy);
}


void PlayScene::Load()
{
	xLeft = xRight = -1;

	HUD = HUD::GetInstance();

	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[BACKGROUND]") {
			section = SCENE_SECTION_BACKGROUND; continue;
		}
		if (line == "[TEXTURES]") {
			section = SCENE_SECTION_TEXTURES; continue;
		}
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[ITEMS]") {
			section = SCENE_SECTION_ITEMS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[ENEMY]") {
			section = SCENE_SECTION_ENEMY; continue;
		}
		if (line == "[CAMERA]")
		{
			section = SCENE_SECTION_CAMERA; continue;
		}

		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: ParseSection_Textures(line); break;
		case SCENE_SECTION_SPRITES: ParseSection_Sprites(line); break;
		case SCENE_SECTION_ANIMATIONS: ParseSection_Animations(line); break;
		case SCENE_SECTION_ANIMATION_SETS: ParseSection_Animation_Sets(line); break;
		case SCENE_SECTION_ITEMS: ParseSection_Items(line); break;
		case SCENE_SECTION_OBJECTS: ParseSection_Objects(line); break;
		case SCENE_SECTION_ENEMY: ParseSection_Enemy(line); break;
		case SCENE_SECTION_BACKGROUND: ParseSection_BackGround(line); break;
		case SCENE_SECTION_CAMERA: ParseSection_Camera(line); break;
		}
	}

	f.close();

	Textures::GetInstance()->AddTexture(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void PlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;


	for (size_t i = 0; i < Objects.size(); i++)
	{
		coObjects.push_back(Objects[i]);
	}
	for (size_t i = 0; i < Enemy.size(); i++)
	{
		coObjects.push_back(Enemy[i]);
	}

	/*for (int i = 0; i < Enemy.size(); i++)
	{
		Enemy[i]->Update(dt, &coObjects);
	}*/
	for (int i = 0; i < Items.size(); i++)
	{
		Items[i]->Update(dt, &coObjects);
	}
	if (Game::GetInstance()->GetCurrentSceneId() == 3)
		for (int i = 0; i < Nodes.size(); i++)
		{
			Nodes[i]->Update(dt, &coObjects);
		}
	checkCollisionWithEnemy();

	for (size_t i = 0; i < coObjects.size(); i++)
	{
		coObjects[i]->Update(dt, &coObjects);
	}
	useFireBall();

	for (size_t i = 0; i < Weapon.size(); i++)
	{
		Weapon[i]->Update(dt, &coObjects);
	}

	for (int i = 0; i < Effect.size(); i++)
	{
		Effect[i]->Update(dt, &coObjects);
	}

	mario->Update(dt, &coObjects);

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (mario == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	mario->GetPosition(cx, cy);

	Game* game = Game::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	if (game->GetCurrentSceneId() == 3)
	{
		cx = -20;
		cy = -10;
	}
	else
	{
		/*if (cx <= 0)
			cx = 0;
		else if (cx + game->GetScreenWidth() >= 2816)
			cx = 2816 - game->GetScreenWidth();*/

		if (cx < xLeft) cx = xLeft; if (cx > xRight - 336) cx = xRight- 336;

		if (mario->level == MARIO_LEVEL_FLY || mario->level == MARIO_LEVEL_TAIL) {
			if (cy <= 0) cy = 0;
			else if (cy + game->GetScreenHeight() >= 432)
				cy = 432 - game->GetScreenHeight() + 42;
		}
		else //if (cy + game->GetScreenHeight() >= 432)
			cy = (float)(432 - game->GetScreenHeight() + 42);
	}

	Game::GetInstance()->SetCamPosition(cx, cy);

	HUD->Update(dt);


	checkCollisionWithBrick();
	checkCollisionWithItem();
	checkCollisionEnemyWithBrick();
	checkCollisionEnemyWithEnemy();
	checkEndScene();

	if (game->GetCurrentSceneId() == 3)
		checkMarioWorldMap();

}

void PlayScene::Render()
{

	if (Game::GetInstance()->GetCurrentSceneId() == 3)
	{
		Sprites::GetInstance()->Get(11111)->Draw(0, 0);
	}

	vector<LPGAMEOBJECT> venus;
	vector<LPGAMEOBJECT> enemies;	// enemy khong phai venus de vẽ venus sau gach

	for (int i = 0; i < Enemy.size(); i++)
	{
		if (dynamic_cast<Venus*>(Enemy[i]))
		{
			venus.push_back(Enemy[i]);
		}
		else
		{
			enemies.push_back(Enemy[i]);
		}
	}

	for (int i = 0; i < BackGround.size(); i++)
		BackGround[i]->Render();

	for (int i = 0; i < venus.size(); i++)
		venus[i]->Render();

	for (int i = 0; i < Objects.size(); i++)
		Objects[i]->Render();

	for (int i = 0; i < enemies.size(); i++)
		enemies[i]->Render();

	for (int i = 0; i < Items.size(); i++)
		Items[i]->Render();

	for (size_t i = 0; i < Weapon.size(); i++)
	{
		Weapon[i]->Render();
	}
	if (Game::GetInstance()->GetCurrentSceneId() == 3)
		for (int i = 0; i < Nodes.size(); i++)
		{
			Nodes[i]->Render();
		}

	for (int i = 0; i < Effect.size(); i++)
	{
		Effect[i]->Render();
	}

	mario->Render();

	HUD->Render();

}

/*
	Unload current scene
*/
void PlayScene::Unload()
{
	for (int i = 0; i < Items.size(); i++)
		delete Items[i];

	for (int i = 0; i < Objects.size(); i++)
		delete Objects[i];

	for (int i = 0; i < BackGround.size(); i++)
		delete BackGround[i];

	for (int i = 0; i < Enemy.size(); i++)
		delete Enemy[i];

	for (int i = 0; i < Weapon.size(); i++)
		delete Weapon[i];

	for (int i = 0; i < Effect.size(); i++)
	{
		delete Effect[i];
	}

	for (int i = 0; i < Nodes.size(); i++)
	{
		delete Nodes[i];
	}

	//delete mario;

	Objects.clear();
	Items.clear();
	Enemy.clear();
	BackGround.clear();
	Weapon.clear();
	Nodes.clear();
	Effect.clear();

	mario = NULL;
	//delete mario;

	HUD = NULL;
	delete HUD;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void PlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Mario* mario = ((PlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		if (mario->isJumping || mario->isFlying)
		{
			mario->SetState(MARIO_STATE_JUMP);
			mario->isJumping = false;
		}
		else if (mario->level == MARIO_LEVEL_TAIL)
		{
			mario->isWagging = true;
		}

		break;
	case DIK_Q:
		mario->Reset();
		break;
	case DIK_A:

		if (mario->level == MARIO_LEVEL_TAIL)
		{
			mario->isAllowSwing = true;

			if (mario->swing == 0)
				mario->StartSwing();
		}
		if (mario->level == MARIO_LEVEL_FIRE)
			mario->isUseFire = true;
		break;

	case DIK_B:
		if (mario->GetLevel() == MARIO_LEVEL_BIG)
			break;

		if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
			mario->isTurnToBig = true;
		}

		mario->SetLevel(MARIO_LEVEL_BIG);
		break;

	case DIK_T:
		if (mario->GetLevel() == MARIO_LEVEL_TAIL)
			return;

		if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
			mario->isTurnToTail = true;
		}

		mario->SetLevel(MARIO_LEVEL_TAIL);
		break;

	case DIK_N:
		if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			return;

		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;

	case DIK_R:
		mario->level = MARIO_LEVEL_FIRE;

		mario->isTurnToBig = true;
		break;

	case DIK_P:
		mario->isComeUp = true;
		break;
	case DIK_F:
		if (mario->level == MARIO_LEVEL_FIRE)
			mario->isUseFire = true;
	}

	if (Game::GetInstance()->GetCurrentSceneId() == 3)
	{
		switch (KeyCode)
		{
		case DIK_DOWN:
			/*if (mario->isGoDown)
			{
				mario->vy = 0.1f;
			}*/
			mario->isGoDown = true;
			break;
		case DIK_UP:
			/*if (mario->isGoUp)
			{
				mario->vy = -0.1f;
			}*/
			mario->isGoUp = true;
			break;
		case DIK_RIGHT:

			/*if (mario->isGoRight)
			{
				mario->vx = 0.1f;
			}*/
			mario->isGoRight = true;
			break;
		case DIK_LEFT:
			/*if (mario->isGoLeft)
			{
				mario->vx = -0.1f;
			}*/
			mario->isGoLeft = true;
			break;
		default:
			mario->isGoDown = false;
			mario->isGoUp = false;
			mario->isGoRight = false;
			mario->isGoLeft = false;
			break;
		}
	}
}

void PlayScenceKeyHandler::useWeapon()
{

}

void PlayScenceKeyHandler::KeyState(BYTE* states)
{
	Game* game = Game::GetInstance();
	//Mario* mario = ((PlayScene*)scence)->GetPlayer();
	Mario* mario = Mario::GetInstance(0, 0);

	//disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;

	if (game->GetCurrentSceneId() != 3)
	{
		if (game->IsKeyDown(DIK_RIGHT))
		{
			if (mario->vx < 0 && mario->vy == 0)
			{
				mario->StartDrift();
			}
			if (mario->isDrift == false)
			{
				mario->SetState(MARIO_STATE_WALKING_RIGHT);
				mario->isPressed = true;
			}
		}
		else if (game->IsKeyDown(DIK_LEFT))
		{
			if (mario->vx > 0 && mario->vy == 0)
			{
				mario->StartDrift();
			}
			if (mario->isDrift == false)
			{
				mario->SetState(MARIO_STATE_WALKING_LEFT);
				mario->isPressed = true;
			}
		}
		else
		{
			if (mario->vy == 0)
				mario->SetState(MARIO_STATE_IDLE);
		}

		if (game->IsKeyDown(DIK_S))
		{
			if (mario->isMaxJumping && mario->maxjumping == 0)
			{
				mario->StartJumpingMax();
				mario->isMaxJumping = false;
			}
		}
		else
		{
			mario->maxjumping = 0;
		}
	}

	if (game->IsKeyDown(DIK_A))
	{
		mario->isAllowMoment = true;
		mario->isAllowHold = true;
	}
	else
	{
		mario->isAllowMoment = false;
		mario->isAllowHold = false;
		mario->isHoldingItem = false;
	}

	if (game->IsKeyDown(DIK_S) && game->GetCurrentSceneId() == 3)
	{
		mario->SwitchMap();
	}

	if (game->IsKeyDown(DIK_DOWN) && game->GetCurrentSceneId() != 3)
	{
		mario->isCrouch = true;
		mario->isTurnBack = 1;
	}
	else
	{
		mario->isCrouch = false;
	}
}

void PlayScenceKeyHandler::OnKeyUp(int KeyCode)
{

}

void PlayScene::checkCollisionWithItem()
{
	for (UINT i = 0; i < Items.size(); i++) {
		if (Items[i]->GetFinish() == false) // chưa kết thúc 
		{
			if (mario->isCollisionWithItem(Items[i]) == true) // có va chạm
			{
				//Items[i]->isFinish = true;
				GameObject* obj = dynamic_cast<GameObject*> (Items[i]);

				if (dynamic_cast<Star*>(obj)) {
					Star* star = dynamic_cast<Star*>(obj);

					mario->item = rand() % 3 + 1;
					mario->numItem += 1;
					mario->vy = 0;
					mario->score += 10000;

					star->item = mario->item;
					star->Start_Fly();

					mario->isAutoGo = true;

					StartEndScene();
				}
				else if (dynamic_cast<Leaf*>(obj)) {
					dynamic_cast<Leaf*>(obj)->isFinish = true;

					if (mario->level == MARIO_LEVEL_SMALL) {
						mario->isTurnToTail = true;
					}

					if (mario->level != MARIO_LEVEL_TAIL)
						mario->level = MARIO_LEVEL_TAIL;

					mario->score += 1000;
				}
				else if (dynamic_cast<Mushroom*>(obj)) {

					dynamic_cast<Mushroom*>(obj)->isFinish = true;

					if (mario->level == MARIO_LEVEL_SMALL) {
						mario->isTurnToBig = true;
						mario->level = MARIO_LEVEL_BIG;
					}
					else if (mario->level > MARIO_LEVEL_BIG)
					{
						mario->live += 1;
					}

					mario->score += 1000;
				}
				else if (dynamic_cast<Coin*>(obj)) {

					if (dynamic_cast<Coin*>(obj)->isNoCollision == false)
						dynamic_cast<Coin*>(obj)->isFinish = true;

					mario->score += 100;
					mario->dola += 1;

				}
				else if (dynamic_cast<Button*>(obj))
				{
					if (dynamic_cast<Button*>(obj)->isFinish == false)
					{
						mario->vy = -0.2;

						dynamic_cast<Button*>(obj)->isFinish = true;

						for (int i = 0; i < Objects.size(); i++)
						{
							if (dynamic_cast<BrickGold*>(Objects[i])
								&& (dynamic_cast<BrickGold*>(Objects[i])->item == 0)
								&& dynamic_cast<BrickGold*>(Objects[i])->isFinish == false)
							{
								Coin* coin = new Coin();
								coin->SetPosition(Objects[i]->x, Objects[i]->y);
								coin->isNoCollision = false;
								coin->SetAnimationSet(AnimationSets::GetInstance()->Get(32));
								Items.push_back(coin);

								Objects[i]->isFinish = true;
							}
						}
					}
				}
			}
		}
	}
}

void PlayScene::checkCollisionWithEnemy()
{
	Mario* mario = Mario::GetInstance(0, 0);

	float l, t, r, b;

	mario->GetBoundingBox(l, t, r, b);

	if (GetTickCount() - mario->untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		mario->untouchable_start = 0;
		mario->untouchable = false;
	}

	for (UINT i = 0; i < Enemy.size(); i++)
	{
		GameObject* obj = dynamic_cast<GameObject*> (Enemy[i]);

		LPCOLLISIONEVENT e = mario->SweptAABBEx(Enemy[i]);

		if (mario->isCollisionWithItem(Enemy[i]) && r <= Enemy[i]->y)
		{
			mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
		}

		for (INT i = 0; i < Weapon.size(); i++)
		{
			if ((Weapon[i])->GetFinish() == false) {

				//FireBall *f= dynamic_cast<FireBall*> (Weapon[i]);

				//if ( f->GetFinish()==false) {

				LPCOLLISIONEVENT e = obj->SweptAABBEx(Weapon[i]);

				if (e->t > 0 && e->t <= 1) {
					isCollision = true;
				}
				//}
			}
		}

		if (dynamic_cast<Goomba*>(obj)) // if obj is Goomba 
		{
			Goomba* goomba = dynamic_cast<Goomba*>(obj);
			//LPCOLLISIONEVENT e = mario->SweptAABBEx(goomba);

			if (isCollision) {
				goomba->SetState(GOOMBA_STATE_DIE);
				isCollision = false;
			}

			if (mario->isAllowSwing && mario->isCollisionWithItem(goomba)) {
				goomba->SetState(GOOMBA_STATE_DIE);
			}

			if (e->t > 0 && e->t <= 1) {

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					mario->score += 100;

					goomba->SetState(GOOMBA_STATE_DIE);
					mario->vy = -MARIO_JUMP_DEFLECT_SPEED;

				}
				else if (e->nx != 0)
				{
					if (mario->untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (mario->level > MARIO_LEVEL_SMALL)
							{
								mario->level--;
								mario->StartUntouchable();
							}
							else
								mario->SetState(MARIO_STATE_DIE);
						}
					}
				}
			}
			//goomba = NULL;
		}
		else if (dynamic_cast<Koopas*>(obj)) // if e->obj is Koopa 
		{
			/*Koopas* koopas = NULL;
			koopas = dynamic_cast<Koopas*>(obj);*/

			//LPCOLLISIONEVENT e = mario->SweptAABBEx(dynamic_cast<Koopas*>(obj));

			if (isCollision)
			{
				dynamic_cast<Koopas*>(obj)->level = KOOPAS_LEVEL_DIE_DOWN;
				dynamic_cast<Koopas*>(obj)->SetState(KOOPAS_STATE_IDLE);
				isCollision = false;
			}
			float l1, t1, r1, b1;
			dynamic_cast<Koopas*>(obj)->GetBoundingBox(l1, t1, r1, b1);

			if (mario->isAllowSwing && Game::GetInstance()->checkAABB(l, t, r, b, l1, t1, r1, b1))//&& mario->isCollisionWithItem(dynamic_cast<Koopas*>(obj)))
			{
				mario->score += 100;

				dynamic_cast<Koopas*>(obj)->SetState(KOOPAS_STATE_IDLE);
				dynamic_cast<Koopas*>(obj)->level = KOOPAS_LEVEL_DIE_UP;
			}



			if (mario->isAllowHold && dynamic_cast<Koopas*>(obj)->GetState() == KOOPAS_STATE_IDLE
				&& mario->isCollisionWithItem(dynamic_cast<Koopas*>(obj))
				&& dynamic_cast<Koopas*>(obj)->level < KOOPAS_LEVEL_NORMAL)
			{
				mario->isHoldingItem = true;

				dynamic_cast<Koopas*>(obj)->isWaiting = 0;

				mario->SetHodingItem(dynamic_cast<Koopas*>(obj));

			}
			/*if (mario->isHoldingItem)
			{
				mario->SetHodingItem(dynamic_cast<Koopas*>(obj));

			}*/
			else  if (dynamic_cast<Koopas*>(obj)->GetState() == KOOPAS_STATE_IDLE
				&& mario->isAllowHold == false
				&& mario->isAllowSwing == false
				&& mario->isCollisionWithItem(dynamic_cast<Koopas*>(obj)))
			{
				if (mario->kicking == 0)
				{
					mario->StartKick();
				}

				if (mario->vx > 0 || mario->nx > 0)
				{
					dynamic_cast<Koopas*>(obj)->SetState(KOOPAS_STATE_WALKING_RIGHT);
				}
				else
				{
					dynamic_cast<Koopas*>(obj)->SetState(KOOPAS_STATE_WALKING_LEFT);
				}
			}

			else if (e->t > 0 && e->t <= 1)
			{
				if (e->ny < 0)
				{
					mario->score += 100;
					mario->vy = -MARIO_JUMP_DEFLECT_SPEED;

					if (dynamic_cast<Koopas*>(obj)->GetState() == KOOPAS_STATE_IDLE)
					{

						if (dynamic_cast<Koopas*>(obj)->x >= mario->x)
						{
							dynamic_cast<Koopas*>(obj)->SetState(KOOPAS_STATE_WALKING_RIGHT);
						}
						else
						{
							dynamic_cast<Koopas*>(obj)->SetState(KOOPAS_STATE_WALKING_LEFT);
						}
					}
					else if (dynamic_cast<Koopas*>(obj)->GetState() != KOOPAS_STATE_IDLE)
					{
						if (dynamic_cast<Koopas*>(obj)->level == KOOPAS_LEVEL_WING)
						{
							dynamic_cast<Koopas*>(obj)->level = KOOPAS_LEVEL_NORMAL;

							dynamic_cast<Koopas*>(obj)->SetState(KOOPAS_STATE_WALKING_LEFT);
						}
						else if (dynamic_cast<Koopas*>(obj)->level == KOOPAS_LEVEL_NORMAL)
						{
							dynamic_cast<Koopas*>(obj)->level = KOOPAS_LEVEL_DIE_DOWN;

							dynamic_cast<Koopas*>(obj)->SetState(KOOPAS_STATE_IDLE);
						}
						else
						{
							dynamic_cast<Koopas*>(obj)->SetState(KOOPAS_STATE_IDLE);
						}
					}
				}
				else if (e->nx != 0)
				{
					mario->noCollision = true;

					if (mario->untouchable == 0)
					{
						if (dynamic_cast<Koopas*>(obj)->GetState() != KOOPAS_STATE_IDLE && mario->isAllowHold == false)
						{
							if (mario->level > MARIO_LEVEL_SMALL)
							{
								mario->level--;
								mario->StartUntouchable();
							}
							else
							{
								mario->SetState(MARIO_STATE_DIE);
							}
						}
					}
				}
			}

			else if (mario->isCollisionWithItem(dynamic_cast<Koopas*>(obj)) && (mario->vy > 0))
			{
				/*if (mario->noCollision)
				{
					mario->noCollision = false;
					break;
				}*/

				mario->score += 100;
				mario->vy = -MARIO_JUMP_DEFLECT_SPEED;

				if (dynamic_cast<Koopas*>(obj)->GetState() != KOOPAS_STATE_IDLE)
				{
					if (dynamic_cast<Koopas*>(obj)->level == KOOPAS_LEVEL_WING)
					{
						dynamic_cast<Koopas*>(obj)->level = KOOPAS_LEVEL_NORMAL;

						dynamic_cast<Koopas*>(obj)->SetState(KOOPAS_STATE_WALKING_LEFT);
					}
					else if (dynamic_cast<Koopas*>(obj)->level == KOOPAS_LEVEL_NORMAL)
					{
						dynamic_cast<Koopas*>(obj)->level = KOOPAS_LEVEL_DIE_DOWN;

						dynamic_cast<Koopas*>(obj)->SetState(KOOPAS_STATE_IDLE);
					}
					else
					{
						dynamic_cast<Koopas*>(obj)->SetState(KOOPAS_STATE_IDLE);
					}

				}
				/*else
				{
					if (dynamic_cast<Koopas*>(obj)->x >= mario->x)
					{
						dynamic_cast<Koopas*>(obj)->SetState(KOOPAS_STATE_WALKING_RIGHT);
					}
					else
					{
						dynamic_cast<Koopas*>(obj)->SetState(KOOPAS_STATE_WALKING_LEFT);
					}
				}*/
			}

			//if (e->t > 0 && e->t <= 1)
			//{
			//	// jump on top >> kill Goomba and deflect a bit 
			//	if (e->ny != 0)
			//	{
			//		mario->score += 100;
			//		mario->vy = -MARIO_JUMP_DEFLECT_SPEED;

			//		if (dynamic_cast<Koopas*>(obj)->GetState() != KOOPAS_STATE_IDLE)
			//		{
			//			if (dynamic_cast<Koopas*>(obj)->level == KOOPAS_LEVEL_WING)
			//			{
			//				dynamic_cast<Koopas*>(obj)->level = KOOPAS_LEVEL_NORMAL;
			//			}
			//			else if (dynamic_cast<Koopas*>(obj)->level == KOOPAS_LEVEL_NORMAL)
			//			{
			//				dynamic_cast<Koopas*>(obj)->level = KOOPAS_LEVEL_DIE_DOWN;
			//			}

			//			dynamic_cast<Koopas*>(obj)->SetState(KOOPAS_STATE_IDLE);

			//		}
			//		else
			//		{
			//			if (dynamic_cast<Koopas*>(obj)->x >= mario->x)
			//			{
			//				dynamic_cast<Koopas*>(obj)->SetState(KOOPAS_STATE_WALKING_RIGHT);
			//			}
			//			else
			//			{
			//				dynamic_cast<Koopas*>(obj)->SetState(KOOPAS_STATE_WALKING_LEFT);
			//			}
			//		}

			//	}
			//	else //if (e->nx != 0 )
			//	{
			//		if (mario->untouchable == 0)
			//		{
			//			if (dynamic_cast<Koopas*>(obj)->GetState() != KOOPAS_STATE_IDLE)
			//			{
			//				if (mario->level > MARIO_LEVEL_SMALL)
			//				{
			//					mario->level--;
			//					mario->StartUntouchable();
			//				}
			//				else
			//				{
			//					mario->SetState(MARIO_STATE_DIE);
			//				}
			//			}
			//		}
			//	}

			//}

		}
		else if (dynamic_cast<ParaGoomba*>(obj)) // if e->obj is Para Goomba
		{
			ParaGoomba* para = dynamic_cast<ParaGoomba*>(obj);

			LPCOLLISIONEVENT e = mario->SweptAABBEx(para);

			if (isCollision) {
				mario->score += 100;

				para->SetLevel(PARAGOOMBA_LEVEL_DIE);
				isCollision = false;
			}

			if (mario->isAllowSwing && mario->isCollisionWithItem(para)) {
				mario->score += 100;

				para->SetLevel(PARAGOOMBA_LEVEL_DIE);
			}

			/*if (mario->isCollisionWithItem(para) && mario->y < para->y + 16)
			{
				mario->score += 100;
				if (para->level != PARAGOOMBA_LEVEL_DIE)
				{
					if (para->level == PARAGOOMBA_LEVEL_WING)
						para->SetLevel(PARAGOOMBA_LEVEL_NORMAL);
					else if (para->level == PARAGOOMBA_LEVEL_NORMAL)
						para->SetLevel(PARAGOOMBA_LEVEL_DIE);

					mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
					para->vy = 0;
				}
			}*/

			if (e->t > 0 && e->t <= 1) {

				// jump on top >> kill ParaGoomba and deflect a bit 
				if (e->ny < 0)
				{
					mario->score += 100;
					if (para->level != PARAGOOMBA_LEVEL_DIE)
					{
						if (para->level == PARAGOOMBA_LEVEL_NORMAL)
							para->SetLevel(PARAGOOMBA_LEVEL_DIE);
						/*else if (para->level == PARAGOOMBA_LEVEL_WING)
							para->SetLevel(PARAGOOMBA_LEVEL_NORMAL);*/

						mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
					}

				}
				else if (e->nx != 0)
				{
					if (mario->untouchable == 0)
					{
						if (para->level != PARAGOOMBA_LEVEL_DIE)
						{
							if (mario->level > MARIO_LEVEL_SMALL)
							{
								mario->level--;
								mario->StartUntouchable();
							}
							else
								mario->SetState(MARIO_STATE_DIE);
						}
					}
				}
			}
			else if (mario->isCollisionWithItem(para))
			{
				if (para->level == PARAGOOMBA_LEVEL_WING)
					para->SetLevel(PARAGOOMBA_LEVEL_NORMAL);

				mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
		else if (dynamic_cast<Venus*>(obj))
		{
			Venus* venus = dynamic_cast<Venus*>(obj);
			LPCOLLISIONEVENT e = mario->SweptAABBEx(venus);

			if (mario->GetX() == venus->GetX() && mario->GetY() == venus->GetY())
			{
				venus->SetState(VENUS_STATE_TOP);
			}
			else if (mario->GetX() < venus->GetX())
			{
				if (mario->GetY() >= venus->GetY())
					venus->SetState(VENUS_STATE_BOT_LEFT);
				else
					venus->SetState(VENUS_STATE_TOP_LEFT);
			}
			else if (mario->GetX() > venus->GetX())
			{
				if (mario->GetY() >= venus->GetY())
					venus->SetState(VENUS_STATE_BOT_RIGHT);
				else
					venus->SetState(VENUS_STATE_TOP_RIGHT);
			}


			if (isCollision) {
				venus->SetState(VENUS_STATE_DIE);
				isCollision = false;
			}

			if (e->t > 0 && e->t <= 1) {

				if (mario->untouchable == 0)
				{
					if (venus->GetState() != VENUS_STATE_DIE)
					{
						if (mario->level > MARIO_LEVEL_SMALL)
						{
							mario->level--;
							mario->StartUntouchable();
						}
						else
							mario->SetState(MARIO_STATE_DIE);
					}
				}
			}
		}
	}
}

void PlayScene::useFireBall()
{
	Mario* mario = Mario::GetInstance(0, 0);

	if (mario->isUseFire)
	{
		float x = mario->GetX();
		float y = mario->GetY();

		GameObject* weapon = NULL;
		weapon = new FireBall();

		weapon->SetPosition(x, y);


		AnimationSets* animation_sets = AnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(35);

		weapon->SetAnimationSet(ani_set);

		Weapon.push_back(weapon);

		if (mario->vx > 0 || mario->nx > 0)
			weapon->SetState(FIREBALL_GOING_RIGHT);
		else
			weapon->SetState(FIREBALL_GOING_LEFT);

		mario->isUseFire = false;
	}
}

void PlayScene::checkCollisionWithBrick()
{
	Mario* mario = Mario::GetInstance(0, 0);

	for (UINT i = 0; i < Objects.size(); i++) {

		GameObject* obj = dynamic_cast<GameObject*> (Objects[i]);

		if (dynamic_cast<Brick*>(obj))
		{
			// 
		}

		else if (dynamic_cast<BrickGold*>(obj))
		{

			LPCOLLISIONEVENT e = mario->SweptAABBEx(obj);
			BrickGold* bg = dynamic_cast<BrickGold*>(obj);

			if (e->t > 0 && e->t <= 1)
			{

				if (obj->GetFinish() == false && (mario->isAllowSwing == true || e->ny > 0))
				{

					obj->isFinish = true;

					if (bg->item == 0)
					{
						//obj->isFinish = true;

						Coin* coin = new Coin();
						coin->SetPosition(obj->x, obj->y);
						coin->isNoCollision = false;
						coin->SetAnimationSet(AnimationSets::GetInstance()->Get(32));
						Items.push_back(coin);

						BrickBreak* bb = new BrickBreak(Objects[i]->x, Objects[i]->y);
						bb->isRender = true;
						Effect.push_back(bb);
					}
					else if (bg->item == 1)
					{
						Mushroom* mr = new Mushroom();
						mr->SetPosition(obj->x, obj->y - 36);
						mr->SetAnimationSet(AnimationSets::GetInstance()->Get(38));
						Items.push_back(mr);
					}
					else if (bg->item == 2)
					{
						Button* bt = new Button();
						bt->SetPosition(obj->x, obj->y - 16);
						bt->SetAnimationSet(AnimationSets::GetInstance()->Get(39));
						Items.push_back(bt);
					}
					else if (bg->item == 3)
					{
						Leaf* leaf = new Leaf();
						leaf->SetPosition(obj->x, obj->y - 36);
						leaf->SetAnimationSet(AnimationSets::GetInstance()->Get(36));
						Items.push_back(leaf);
					}
					else if (bg->item == 4)
					{
						Coin* coin = new Coin();
						coin->SetPosition(obj->x, obj->y - 16);
						coin->isNoCollision = true;
						coin->SetAnimationSet(AnimationSets::GetInstance()->Get(32));
						Items.push_back(coin);
					}
					else
					{
						obj->isFinish = false;
					}
				}
			}
		}
		else if (dynamic_cast<BrickQuesion*>(obj)) {

			if (obj->isFinish == false)
			{
				LPCOLLISIONEVENT e = mario->SweptAABBEx(obj);
				BrickQuesion* bq = dynamic_cast<BrickQuesion*>(obj);

				if (e->t > 0 && e->t <= 1 && e->ny > 0 || (mario->isAllowSwing && mario->isCollisionWithItem(bq)))
				{

					bq->isFinish = true;
					bq->isMovingDown = true;

					if (bq->item == 0)
					{
						if (mario->level >= MARIO_LEVEL_BIG)
						{
							Leaf* leaf = new Leaf();
							leaf->SetPosition(obj->x, obj->y - 36);

							AnimationSets* animation_sets = AnimationSets::GetInstance();
							LPANIMATION_SET ani_set = animation_sets->Get(36);

							leaf->SetAnimationSet(ani_set);
							Items.push_back(leaf);
						}
						else if (mario->level < MARIO_LEVEL_BIG)
						{
							Mushroom* mr = new Mushroom();
							mr->SetPosition(obj->x, obj->y - 17);

							AnimationSets* animation_sets = AnimationSets::GetInstance();
							LPANIMATION_SET ani_set = animation_sets->Get(37);

							mr->SetAnimationSet(ani_set);
							Items.push_back(mr);
						}

					}
					else if (bq->item == 1)
					{
						mario->score += 100;

						Coin* coin = new Coin();
						coin->SetPosition(obj->x, obj->y - 16);
						coin->isNoCollision = true;

						//AnimationSets* animation_sets = AnimationSets::GetInstance();
						//LPANIMATION_SET ani_set = animation_sets->Get(32);

						coin->SetAnimationSet(AnimationSets::GetInstance()->Get(32));
						Items.push_back(coin);
					}
				}
			}
		}
		else if (dynamic_cast<MovingBrick*>(obj))
		{
			LPCOLLISIONEVENT e = mario->SweptAABBEx(obj);

			if (e->t > 0 && e->t <= 1)
			{
				MovingBrick* mb = dynamic_cast<MovingBrick*>(obj);

				mb->isFalling = true;
			}
		}
		else if (dynamic_cast<Portal*>(obj))
		{

			LPCOLLISIONEVENT e = mario->SweptAABBEx(obj);

			if (e->t > 0 && e->t <= 1)
			{
				Portal* p = dynamic_cast<Portal*>(obj);

				if (p->GetSceneId() == 2 || p->GetSceneId() == 6)
				{
					if (Game::GetInstance()->IsKeyDown(DIK_DOWN))
					{
						Game::GetInstance()->SwitchScene(p->GetSceneId());
					}
				}
				else if (p->GetSceneId() == 1)
				{

					Game::GetInstance()->SwitchScene(p->GetSceneId());

					Mario::GetInstance(0, 0)->isComeUp = true;


				}

				//Game::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}

	}
}

void PlayScene::checkCollisionEnemyWithBrick()
{
	for (int i = 0; i < Enemy.size(); i++)
	{
		GameObject* obj = dynamic_cast<GameObject*> (Enemy[i]);

		if (dynamic_cast<Koopas*>(obj)) // if obj is Goomba 
		{
			Koopas* koopa = dynamic_cast<Koopas*>(obj);
			//LPCOLLISIONEVENT e = mario->SweptAABBEx(goomba);

			for (int i = 0; i < Objects.size(); i++)
			{
				LPCOLLISIONEVENT e = koopa->SweptAABBEx(Objects[i]);
				//if (e->t > 0 && e->t <= 1)
				if (koopa->isCollisionWithObject(Objects[i]))
				{
					if (koopa->vx > 0)
					{
						koopa->SetState(KOOPAS_STATE_WALKING_LEFT);
					}
					else
					{
						koopa->SetState(KOOPAS_STATE_WALKING_RIGHT);
					}

					if (dynamic_cast<BrickQuesion*>(Objects[i]) && dynamic_cast<BrickQuesion*>(Objects[i])->isFinish == false)
					{
						BrickQuesion* bq = dynamic_cast<BrickQuesion*>(Objects[i]);

						bq->isFinish = true;
						bq->isMovingDown = true;

						if (bq->item == 0) {
							if (mario->level >= MARIO_LEVEL_BIG) {
								Leaf* leaf = new Leaf();
								leaf->SetPosition(obj->x, obj->y - 36);

								AnimationSets* animation_sets = AnimationSets::GetInstance();
								LPANIMATION_SET ani_set = animation_sets->Get(36);

								leaf->SetAnimationSet(ani_set);
								Items.push_back(leaf);
							}
							else if (mario->level < MARIO_LEVEL_BIG) {
								Mushroom* mr = new Mushroom();
								mr->SetPosition(obj->x, obj->y - 36);

								AnimationSets* animation_sets = AnimationSets::GetInstance();
								LPANIMATION_SET ani_set = animation_sets->Get(37);

								mr->SetAnimationSet(ani_set);
								Items.push_back(mr);
							}

						}
						else if (bq->item == 1) {
							mario->score += 100;

							Coin* coin = new Coin();
							coin->SetPosition(obj->x, obj->y - 16);
							coin->isNoCollision = true;

							//AnimationSets* animation_sets = AnimationSets::GetInstance();
							//LPANIMATION_SET ani_set = animation_sets->Get(32);

							coin->SetAnimationSet(AnimationSets::GetInstance()->Get(32));
							Items.push_back(coin);
						}
					}
					else if (dynamic_cast<BrickGold*>(Objects[i]) && dynamic_cast<BrickGold*>(Objects[i])->isFinish == false)
					{
						BrickGold* bg = dynamic_cast<BrickGold*>(Objects[i]);

						if (bg->item == 0)
						{
							dynamic_cast<BrickGold*>(Objects[i])->isFinish = true;

							BrickBreak* bb = new BrickBreak(bg->x, bg->y);
							bb->StartRender();
							Effect.push_back(bb);
						}
					}

				}
			}

		}
		else if (dynamic_cast<Goomba*>(obj))
		{
			Goomba* goomba = dynamic_cast<Goomba*>(obj);

			for (int i = 0; i < Objects.size(); i++)
			{
				if (goomba->isCollisionWithObject(Objects[i]))
				{
					if (dynamic_cast<BrickGreen*>(Objects[i]))
					{
						//goomba->vx *= -1;
					}
				}
			}
		}
	}
}

void PlayScene::checkCollisionEnemyWithEnemy()
{

	for (int i = 0; i < Enemy.size(); i++)
	{
		vector<LPCOLLISIONEVENT>  coEvents;
		vector<LPCOLLISIONEVENT>  coEventsResult;

		coEvents.clear();

		Enemy[i]->CalcPotentialCollisions(&Enemy, coEvents);

		if (coEvents.size() == 0)
		{

		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;
			float rdx, rdy;

			float max = 0, min = 999999999;

			float l, t, r, b;
			float l1, t1, r1, b1;

			Enemy[i]->FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			if (dynamic_cast<Koopas*>(Enemy[i]))
			{
				if (dynamic_cast<Koopas*>(Enemy[i])->level < KOOPAS_LEVEL_NORMAL
					&& dynamic_cast<Koopas*>(Enemy[i])->GetState() != KOOPAS_STATE_IDLE)
				{

					for (int j = 0; j < coEventsResult.size(); j++)
					{
						/*LPCOLLISIONEVENT e = coEventsResult[i];

						if (e->t > 0 && e->t <= 1)
						{*/

						if (dynamic_cast<Koopas*>(coEventsResult[j]->obj) && dynamic_cast<Koopas*>(Enemy[i]) != dynamic_cast<Koopas*>(coEventsResult[j]->obj))
						{
							dynamic_cast<Koopas*>(coEventsResult[j]->obj)->SetState(KOOPAS_STATE_IDLE);
							dynamic_cast<Koopas*>(coEventsResult[j]->obj)->level = KOOPAS_LEVEL_DIE_UP;
						}
						else if (dynamic_cast<Goomba*>(coEventsResult[j]->obj))
						{
							dynamic_cast<Goomba*>(coEventsResult[j]->obj)->SetState(GOOMBA_STATE_DIE);
						}
						else if (dynamic_cast<ParaGoomba*>(coEventsResult[j]->obj))
						{
							dynamic_cast<ParaGoomba*>(coEventsResult[j]->obj)->SetLevel(PARAGOOMBA_LEVEL_DIE);
						}
						//}
					}
				}
			}
		}
		for (UINT i = 0; i < coEvents.size(); i++)
			delete coEvents[i];
	}
}

void PlayScene::checkMarioWorldMap()
{
	Mario* mario = Mario::GetInstance(15, 32);

	//mario->vx = 0.1f;

	if (mario->currentNode == NULL)
	{
		mario->currentNode = new Node(15, 32, 0, 0, 1, 0, 0, 0);
		//Nodes.push_back(currentNode);

		DebugOut(L"currentNode == NULL\n");
	}
	else //if (mario->isCollisionWithItem(currentNode))
	{
		//DebugOut(L"currentNode->x = %d\n", currentNode->x);

		if (mario->currentNode->bottom && mario->isGoDown)
		{
			mario->vy = 0.1f;
		}
		else if (mario->currentNode->top && mario->isGoUp)
		{
			mario->vy = -0.1f;
		}
		else if (mario->currentNode->right && mario->isGoRight)
		{
			mario->vx = 0.1f;
		}
		else if (mario->currentNode->left && mario->isGoLeft)
		{
			mario->vx = (float)-0.1;
		}
		else
		{
			mario->vx = 0;
			mario->vy = 0;

			mario->isGoDown = false;
			mario->isGoUp = false;
			mario->isGoRight = false;
			mario->isGoLeft = false;
		}
	}

	for (int i = 0; i < Nodes.size(); i++)
	{

		GameObject* obj = dynamic_cast<GameObject*> (Nodes[i]);

		if (dynamic_cast<Node*>(obj)) {

			LPCOLLISIONEVENT e = mario->SweptAABBEx(obj);
			Node* node = dynamic_cast<Node*>(obj);

			if (e->t > 0 && e->t <= 1) /*&& currentNode->x != bg->x && currentNode->y != bg->y) */
			{

				mario->currentNode->SetDirectNode(node->x, node->y, node->left, node->top, node->right, node->bottom, node->SceneId);

				mario->vx = 0;
				mario->vy = 0;

				mario->SetPosition(Nodes[i]->x, Nodes[i]->y);

				mario->isGoDown = false;
				mario->isGoUp = false;
				mario->isGoRight = false;
				mario->isGoLeft = false;

				if (node->type == 1)
				{
					HidenWall* hd = new HidenWall(node->x, node->y, 0, 72);
					BackGround.push_back(hd);

					node->type = 0;
				}

				//break;
			}
		}

	}
}

void PlayScene::checkEndScene()
{
	Mario* mario = Mario::GetInstance(0, 0);

	float cx, cy;

	int ani = 42;

	Game::GetInstance()->GetCamPos(cx, cy);

	if (endscene == 1)
	{
		if (GetTickCount() - time_endscene > 1000)
		{
			if (addtext == 3)
			{
				HidenWall* hd1 = new HidenWall(cx + 100, cy + 30, 0, 40);
				BackGround.push_back(hd1);

				addtext -= 1;
			}
		}

		if (GetTickCount() - time_endscene > 2000)
		{
			if (addtext == 2)
			{
				HidenWall* hd2 = new HidenWall(cx + 80, cy + 50, 0, 41);
				BackGround.push_back(hd2);

				addtext -= 1;
			}
		}

		if (GetTickCount() - time_endscene > 2000)
		{
			if (addtext == 1)
			{
				if (mario->item == 1)
				{
					ani = 42;
				}
				else if (mario->item == 2)
				{
					ani = 43;
				}
				else if (mario->item == 3)
				{
					ani = 44;
				}

				HidenWall* hd3 = new HidenWall(cx + 209, cy + 57, 0, ani);
				BackGround.push_back(hd3);

				addtext -= 1;
			}
		}

		if (GetTickCount() - time_endscene > 6000)
		{
			time_endscene = 0;
			endscene = 0;
			addtext = 3;

			mario->isAutoGo = false;

			Game::GetInstance()->SwitchScene(3);
		}
	}
}