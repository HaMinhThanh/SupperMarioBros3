#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "HidenWall.h"

#include "Koopas.h"
#include "Goomba.h"
#include "Venus.h"
#include "ParaKoopa.h"
#include "ParaGoomba.h"

#include "Leaf.h"
#include "Coin.h"
#include "Mushroom.h"
#include "Star.h"


#include <map>

using namespace std;

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_ITEMS	6
#define SCENE_SECTION_OBJECTS	7
#define SCENE_SECTION_ENEMY 8
#define SCENE_SECTION_BACKGROUND 9

// OBJECT
#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_BRICKQUESION	2
#define OBJECT_TYPE_BRICKGOLD	3
#define OBJECT_TYPE_VENUS	4
#define OBJECT_TYPE_COLORBLOCK	5
#define OBJECT_TYPE_PORTAL		50

// back ground
#define OBJECT_TYPE_BACKGROUND	1

// ITEMS
#define ITEM_TYPE_COIN		1
#define ITEM_TYPE_LEAF		2	
#define ITEM_TYPE_MUSHROOM	3
#define ITEM_TYPE_STAR		4

// ENEMY
#define ENEMY_TYPE_GOOMBA 1
#define ENEMY_TYPE_KOOPAS 2
#define ENEMY_TYPE_VENUS 3
#define ENEMY_TYPE_PARAKOOPAS	4
#define ENEMY_TYPE_PARAGOOMBA	5	

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
	int ani;
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

	int ani_set_id = atoi(tokens[3].c_str());

	AnimationSets* animation_sets = AnimationSets::GetInstance();

	GameObject* obj = NULL;
	GameObject* item = NULL;


	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (mario != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new Mario(x, y);
		mario = (Mario*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_BRICK:
		obj = new Brick();

		break;

	case OBJECT_TYPE_BRICKQUESION:
		obj = new BrickQuesion();
		if (x == 240 && y == 304) {
			item = new Mushroom();
			ani = 37;
		}
		else if (x == 704 && y == 352) {
			item = new Leaf();
			ani = 36;
		}
		else {
			item = new Coin();
			ani = 32;
		}

		hd = true;
		break;
	case OBJECT_TYPE_BRICKGOLD:
		obj = new BrickGold();

		item = new Coin();

		ani = 32;
		hd = true;
		break;

	case OBJECT_TYPE_COLORBLOCK:

		w = atof(tokens[4].c_str());
		h = atof(tokens[5].c_str());

		obj = new BrickColor();

		obj->SetBoundBbox(w, h);

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
	if (hd == true) {
		item->SetPosition(x, y);
		LPANIMATION_SET ani_set1 = animation_sets->Get(ani);
		item->SetAnimationSet(ani_set1);
		Items.push_back(item);
	}

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);

	Objects.push_back(obj);

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

	AnimationSets* animation_sets = AnimationSets::GetInstance();

	GameObject* enemy = NULL;

	switch (object_type)
	{
	case ENEMY_TYPE_GOOMBA:
		enemy = new Goomba();
		break;

	case ENEMY_TYPE_KOOPAS:
		enemy = new Koopas();
		break;
	case ENEMY_TYPE_VENUS:
		enemy = new Venus();
		break;
	case ENEMY_TYPE_PARAKOOPAS:
		enemy = new ParaKoopa();
		break;
	case ENEMY_TYPE_PARAGOOMBA:
		enemy = new ParaGoomba();
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

	for (int i = 0; i < Enemy.size(); i++)
	{
		Enemy[i]->Update(dt, &coObjects);
	}

	for (size_t i = 0; i < Objects.size(); i++)
	{
		Objects[i]->Update(dt, &coObjects);
	}
	useFireBall();

	for (size_t i = 0; i < Weapon.size(); i++)
	{
		Weapon[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (mario == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	mario->GetPosition(cx, cy);

	Game* game = Game::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	//if (cx <= 0)
	//	cx = 0;
	//else if (cx + game->GetScreenWidth() >= 2816)
	//	cx = 2816 - game->GetScreenWidth();

	//if (mario->level == MARIO_LEVEL_FLY || mario->level == MARIO_LEVEL_TAIL) {
	//	if (cy <= 0) cy = 0;
	//	else if (cy + game->GetScreenHeight() >= 432)
	//		cy = 432 - game->GetScreenHeight();
	//}

	//else //if (cy + game->GetScreenHeight() >= 432)
	//	cy = 432 - game->GetScreenHeight();

	Game::GetInstance()->SetCamPosition(cx, cy);

	HUD->Update(dt);

	checkCollisionWithEnemy();
	checkCollisionWithBrick();
	checkCollisionWithItem();
}

void PlayScene::Render()
{

	//Sprites::GetInstance()->Get(1)->Draw(0, 0);	

	for (int i = 0; i < BackGround.size(); i++)
		BackGround[i]->Render();


	for (int i = 0; i < Items.size(); i++)
		Items[i]->Render();

	for (int i = 0; i < Enemy.size(); i++)
		Enemy[i]->Render();	

	for (size_t i = 0; i < Weapon.size(); i++)
	{
		Weapon[i]->Render();
	}

	for (int i = 0; i < Objects.size(); i++)
		Objects[i]->Render();

	HUD->Render();

}

/*
	Unload current scene
*/
void PlayScene::Unload()
{
	for (int i = 0; i < Objects.size(); i++)
		delete Objects[i];

	for (int i = 0; i < Items.size(); i++)
		delete Items[i];

	for (int i = 0; i < Enemy.size(); i++)
		delete Enemy[i];

	for (int i = 0; i < BackGround.size(); i++)
		delete BackGround[i];

	for (int i = 0; i < Weapon.size(); i++)
		delete Weapon[i];

	Objects.clear();
	Items.clear();
	Enemy.clear();
	BackGround.clear();
	Weapon.clear();

	mario = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void PlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Mario* mario = ((PlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (mario->isJumping || mario->isFlying) {
			mario->SetState(MARIO_STATE_JUMP);
			mario->isJumping = false;
		}

		break;

	case DIK_A:
		mario->Reset();
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

	case DIK_S:
		if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			return;

		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;

	/*case DIK_H:
		mario->isAllowHold = true;
		break;*/

	/*case DIK_D:
		mario->isHoldingItem = false;
		mario->isAllowKick = true;
		break;*/

	case DIK_X:
		if (mario->level == MARIO_LEVEL_TAIL) {
			mario->isAllowSwing = true;

			if (mario->swing == 0)
				mario->StartSwing();
		}
		break;

	case DIK_R:
		mario->level = MARIO_LEVEL_FIRE;

		mario->isTurnToBig = true;
		break;

	case DIK_F:
		if (mario->level == MARIO_LEVEL_FIRE)
			mario->isUseFire = true;
		break;
	}
}

void PlayScenceKeyHandler::useWeapon()
{

}

void PlayScenceKeyHandler::KeyState(BYTE* states)
{
	Game* game = Game::GetInstance();
	Mario* mario = ((PlayScene*)scence)->GetPlayer();

	//disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;

	if (game->IsKeyDown(DIK_RIGHT)) {
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
		mario->isPressed = true;
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		mario->SetState(MARIO_STATE_WALKING_LEFT);
		mario->isPressed = true;
	}
	else {
		mario->SetState(MARIO_STATE_IDLE);

	}

	if (game->IsKeyDown(DIK_Z)) {
		mario->isAllowMoment = true;
	}
	else {
		mario->isAllowMoment = false;
	}

	if (game->IsKeyDown(DIK_H)) {
		mario->isAllowHold = true;
	}
	else {
		mario->isAllowHold = false;
		mario->isHoldingItem = false;
	}

	if(game->IsKeyDown(DIK_M)){
		mario->isWagging = true;
	}
	else{
		mario->isWagging = false;
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
				Items[i]->isFinish = true;
				GameObject* obj = dynamic_cast<GameObject*> (Items[i]);

				if (dynamic_cast<Star*>(obj)) {
					mario->isAutoGo = true;
				}
				else if (dynamic_cast<Leaf*>(obj)) {
					if (mario->level != MARIO_LEVEL_TAIL)
						mario->level = MARIO_LEVEL_TAIL;
					mario->score += 1000;
				}
				else if (dynamic_cast<Mushroom*>(obj)) {
					if (mario->level != MARIO_LEVEL_BIG)
						mario->level = MARIO_LEVEL_BIG;
					mario->score += 1000;
				}
				else if (dynamic_cast<Coin*>(obj)) {
					mario->score += 100;
					mario->dola += 1;
				}
			}
		}
	}
}

void PlayScene::checkCollisionWithEnemy()
{

	//bool isCollison;

	if (GetTickCount() - mario->untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		mario->untouchable_start = 0;
		mario->untouchable = false;
	}

	for (UINT i = 0; i < Enemy.size(); i++)
	{
		GameObject* obj = dynamic_cast<GameObject*> (Enemy[i]);

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
			LPCOLLISIONEVENT e = mario->SweptAABBEx(goomba);

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
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
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
		}
		else if (dynamic_cast<Koopas*>(obj)) // if e->obj is Koopa 
		{
			Koopas* koopas = dynamic_cast<Koopas*>(obj);
			LPCOLLISIONEVENT e = mario->SweptAABBEx(koopas);

			if (isCollision) {
				koopas->SetState(KOOPAS_STATE_DIE);
				isCollision = false;
			}			

			if (mario->isAllowSwing && mario->isCollisionWithItem(koopas)) {
				koopas->SetState(KOOPAS_STATE_DIE_UP);
			}
			else  if (koopas->GetState() == KOOPAS_STATE_DIE && mario->isCollisionWithItem(koopas) && mario->isAllowHold == false)
			{
					if (mario->kicking == 0)
						mario->StartKick();

				if (mario->vx > 0 || mario->nx > 0) {
					koopas->SetState(KOOPAS_STATE_DIE_WALKING_RIGHT);
				}
				else {
					koopas->SetState(KOOPAS_STATE_DIE_WALKING_LEFT);
				}
			}

			else if (mario->isAllowHold && koopas->GetState() == KOOPAS_STATE_DIE && mario->isCollisionWithItem(koopas))
			{
				mario->isHoldingItem = true;				
			}

			if (mario->isHoldingItem) {

				mario->SetHodingItem(koopas);
			}			

			if (e->t > 0 && e->t <= 1) {

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					mario->score += 100;
					if (koopas->GetState() != KOOPAS_STATE_DIE)
					{			
						koopas->SetState(KOOPAS_STATE_DIE);

						mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
					}

				}
				else if (e->nx != 0)
				{
					if (mario->untouchable == 0)
					{
						if (koopas->GetState() != KOOPAS_STATE_DIE)
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
		else if (dynamic_cast<ParaKoopa*>(obj)) // if e->obj is Koopa 
		{
			ParaKoopa* para = dynamic_cast<ParaKoopa*>(obj);
			LPCOLLISIONEVENT e = mario->SweptAABBEx(para);

			if (isCollision) {
				para->SetState(KOOPAS_STATE_DIE);
				isCollision = false;
			}			

			if (e->t > 0 && e->t <= 1) {

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					mario->score += 100;
					if (para->GetState() != PARAKOOPA_STATE_DIE)
					{
						para->SetState(PARAKOOPA_STATE_DIE);

						//mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
						mario->vy = -0.3;

						CreateKoopa(para->GetX(), para->GetY());
					}

				}
				else if (e->nx != 0)
				{
					if (mario->untouchable == 0)
					{
						if (para->GetState() != KOOPAS_STATE_DIE)
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
		else if (dynamic_cast<ParaGoomba*>(obj)) // if e->obj is Para Goomba
		{
			ParaGoomba* para = dynamic_cast<ParaGoomba*>(obj);

			LPCOLLISIONEVENT e = mario->SweptAABBEx(para);

			if (isCollision) {
				para->SetState(PARAGOOMBA_STATE_DIE);
				isCollision = false;
			}

			if (e->t > 0 && e->t <= 1) {

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					mario->score += 100;
					if (para->GetState() != PARAGOOMBA_STATE_DIE)
					{
						if (para->GetState() == PARAGOOMBA_STATE_NORMAL)
							para->SetState(PARAGOOMBA_STATE_DIE);
						else if (para->GetState() == PARAGOOMBA_STATE_WING)
							para->SetState(PARAGOOMBA_STATE_NORMAL);

						mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
					}

				}
				else if (e->nx != 0)
				{
					if (mario->untouchable == 0)
					{
						if (para->GetState() != PARAGOOMBA_STATE_DIE)
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
		else if (dynamic_cast<Venus*>(obj))
		{
			Venus* venus = dynamic_cast<Venus*>(obj);
			LPCOLLISIONEVENT e = mario->SweptAABBEx(venus);

			if (mario->GetX() == venus->GetX()&& mario->GetY()==venus->GetY())
				venus->SetState(VENUS_STATE_TOP);
			else if (mario->GetX() < venus->GetX())
			{
				if (mario->GetY() <= venus->GetY())
					venus->SetState(VENUS_STATE_BOT_LEFT);
				else
					venus->SetState(VENUS_STATE_TOP_LEFT);
			}
			else if (mario->GetX() > venus->GetX())
			{
				if (mario->GetY() <= venus->GetY())
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
	if (mario->isUseFire) {
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
	for (UINT i = 0; i < Objects.size(); i++) {

		GameObject* obj = dynamic_cast<GameObject*> (Objects[i]);

		if (dynamic_cast<Brick*>(obj)) {



		}

		else if (dynamic_cast<BrickGold*>(obj)) {

			LPCOLLISIONEVENT e = mario->SweptAABBEx(obj);

			if (e->t > 0 && e->t <= 1) {

				if ((obj->GetFinish() == false && mario->isAllowSwing == true) || e->ny > 0) {
				
					obj->isFinish = true;
				}
			}
		}
		else if (dynamic_cast<BrickQuesion*>(obj)) {

			if (obj->isFinish == false)
			{
				LPCOLLISIONEVENT e = mario->SweptAABBEx(obj);

				if (e->t > 0 && e->t <= 1 && e->ny > 0) {

					obj->isFinish = true;

					for (UINT i = 0; i < Items.size(); i++) {
						if (Items[i]->GetX() == obj->GetX() && Items[i]->GetY() == obj->GetY()) {
							Items[i]->y -= 16;
						}
					}
				}
			}
		}
		else if (dynamic_cast<Portal*>(obj)) {
			Portal* p = dynamic_cast<Portal*>(obj);
			Game::GetInstance()->SwitchScene(p->GetSceneId());
		}

	}
}

void PlayScene::CreateKoopa(float x, float y)
{
	AnimationSets* animation_sets = AnimationSets::GetInstance();

	GameObject* kp = NULL;

	kp = new Koopas();
	kp->SetPosition(x, y);
	kp->SetState(KOOPAS_STATE_WALKING_LEFT);	

	LPANIMATION_SET ani_set = animation_sets->Get(31);

	kp->SetAnimationSet(ani_set);
	Enemy.push_back(kp);

	isNotDie = true;
}