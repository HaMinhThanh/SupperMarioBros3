#include "IntroScene.h"
#include "Utils.h"
#include "PlayScene.h"
#include "HidenWall.h"

#include <iostream>
#include <fstream>

using namespace std;

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

#define OBJECT_TYPE_RED_MARIO			 0
#define OBJECT_TYPE_GREEN_MARIO			 10

#define OBJECT_TYPE_BACKGROUND	1
#define OBJECT_TYPE_KOOPAS_GREEN	2
#define OBJECT_TYPE_KOOPAS_RED	3
#define OBJECT_TYPE_GOOMBA	4
#define OBJECT_TYPE_LEAF	5
#define OBJECT_TYPE_MUSHROOM	6
#define OBJECT_TYPE_STAR	7
#define OBJECT_TYPE_BRICK	8

#define OBJECT_TYPE_PORTAL	50


#define MARIO_TIME_COUNT  1000

#define MAX_SCENE_LINE 1024
#define MARIO_TYPE_RED	0
#define MARIO_TYPE_GREEN	1

IntroScene* IntroScene::instance = NULL;

IntroScene* IntroScene::GetInstance(int id, LPCWSTR filePath)
{
	if (instance == NULL)
		instance = new IntroScene(id, filePath);

	return instance;
}

IntroScene::IntroScene(int id, LPCWSTR filePath) : Scene(id, filePath)
{
	keyHandler = new IntroSceneKeyHandler(this);
}

void IntroScene::ParseSection_TEXTURES(string line)
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

void IntroScene::ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

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

void IntroScene::ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

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

void IntroScene::ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

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

/*
	Parse a line in section [OBJECTS]
*/
void IntroScene::ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int type, level;
	bool object = false;

	int ani_set_id = atoi(tokens[3].c_str());

	AnimationSets* animation_sets = AnimationSets::GetInstance();

	GameObject* obj = NULL;

	bool isBackground = false;

	switch (object_type)
	{
	case OBJECT_TYPE_RED_MARIO:
		obj = new Mario(x, y);
		player1 = (Mario*)obj;
		object = true;
		//DebugOut(L"[INFO] Player1 object created!\n");
		break;
	case OBJECT_TYPE_GREEN_MARIO:
		obj = new Mario(x, y);
		player2 = (Mario*)obj;
		object = true;
		//DebugOut(L"[INFO] Player2 object created!\n");
		break;

	case OBJECT_TYPE_BACKGROUND: {
		type = atoi(tokens[4].c_str());
		obj = new HidenWall(x, y, type);
		break;
	}
	case OBJECT_TYPE_KOOPAS_GREEN:
		level = atof(tokens[4].c_str());
		obj = new Koopas(x, y, level,1);
		//koopasGreen = (Koopas*)obj;
		break;
	case OBJECT_TYPE_KOOPAS_RED:
		level = atof(tokens[4].c_str());
		obj = new Koopas(x, y, level,2);
		//koopasBlack = (Koopas*)obj;
		break;
	case OBJECT_TYPE_GOOMBA:
		obj = new Goomba();
		//goomba = (Goomba*)obj;
		break;
	case OBJECT_TYPE_LEAF:
		obj = new Leaf();
		//leaf = (Leaf*)obj;
		break;
	case OBJECT_TYPE_MUSHROOM:
		obj = new Mushroom();
		break;
	case OBJECT_TYPE_STAR:
		obj = new Star();
		break;
	case OBJECT_TYPE_BRICK:
		obj = new Brick();
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	if (object == false)
		objects.push_back(obj);
}

void IntroScene::Load()
{
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

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	Textures::GetInstance()->AddTexture(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void IntroScene::Update(DWORD dt)
{
	if (isSwitch == 2)
	{

		if (!isStart)
		{
			player1->nx = -1;
			player1->SetState(MARIO_STATE_IDLE);
			player1->SetLevel(MARIO_LEVEL_BIG);

			player2->SetState(MARIO_STATE_IDLE);
			player2->SetLevel(MARIO_LEVEL_BIG);

			time_start = GetTickCount();
			isStart = true;
		}
		else if (GetTickCount() - time_start > 1000)
		{
			/*player1->SetState(MARIO_STATE_WALKING_LEFT);
			player2->SetState(MARIO_STATE_WALKING_RIGHT);

			player1->vx = 0;
			player2->vx = 0;*/

		}

		if (GetTickCount() - time_start > 2000)
		{
			/*player1->SetState(MARIO_STATE_WALKING_LEFT);
			player2->SetState(MARIO_STATE_WALKING_RIGHT);*/
			player1->vx = -0.15f;
			player2->vx = 0.15f;

		}

		if (GetTickCount() - time_start > 2200 && GetTickCount() - time_start < 2600)
		{
			player2->vy = -0.3;
		}

		if (GetTickCount() - time_start > 2600 && GetTickCount() - time_start < 3500)
		{
			player2->SetState(MARIO_STATE_IDLE);
			player2->vy = 0.2f;
			player2->SetState(MARIO_STATE_WALKING_RIGHT);
			player2->vx = 0.15f;
			//player1->SetState(MARIO_STATE_IDLE);

			// Create function for Collision mario with mario

			// Create condition if mairo walking through side limit
		}

		if (GetTickCount() - time_start < 3500 && GetTickCount() - time_start > 3000)
		{
			if (player1->SweptAABBEx(player2))
			{
				player2->vy = -0.32f;
				player1->isCrouch = true;
				player1->isTurnBack = 1;
			}
		}

		else if (GetTickCount() - time_start >= 3500 && GetTickCount() - time_start < 5300)
		{
			player2->vy = 0.3f;
			player1->isCrouch = false;
			player1->vx = 0;

		}
		else if (GetTickCount() - time_start > 5300)// && GetTickCount() - time_start < 14000)
		{
			player2->vx = 0;
			player2->nx = -1;
		}

		if (GetTickCount() - time_start > 5000)
		{
			player1->vx = 0;
			// So 3
			if (addition == 11)
			{
				HidenWall* hd = new HidenWall(146, 106, 0, 24);

				objects.push_back(hd);
				addition -= 1;
			}

			// Mushroom
			if (addition == 10)
			{
				Mushroom* mr = new Mushroom();
				mr->SetAnimationSet(AnimationSets::GetInstance()->Get(37));
				mr->SetPosition(50, 0);
				objects.push_back(mr);
				addition -= 1;
			}

			// Goomba
			if (addition == 9)
			{
				Goomba* gb = new Goomba();
				gb->vx = 0;
				gb->SetAnimationSet(AnimationSets::GetInstance()->Get(4));
				gb->SetPosition(100, 0);
				objects.push_back(gb);
				addition -= 1;

				goomba = gb;
			}

			// Leaf
			if (addition == 8)
			{
				Leaf* leaf1 = new Leaf();
				leaf1->SetAnimationSet(AnimationSets::GetInstance()->Get(36));
				leaf1->SetPosition(160, 0);
				objects.push_back(leaf1);

				addition -= 1;

				leaf = leaf1;
			}

			// Star
			if (addition == 7)
			{
				Mushroom* st = new Mushroom();
				st->SetAnimationSet(AnimationSets::GetInstance()->Get(38));
				st->SetPosition(280, 0);
				objects.push_back(st);
				addition -= 1;
			}
		}

		if (GetTickCount() - time_start > 5200)
		{
			// Koopa
			if (addition == 6)
			{
				Koopas* kp = new Koopas(160, 0, 2,1);
				kp->vy = 0.2f;
				kp->SetState(KOOPAS_STATE_IDLE);
				kp->SetAnimationSet(AnimationSets::GetInstance()->Get(31));
				//kp->SetPosition(160, 0);
				//objects.push_back(kp);
				addition -= 1;

				koopasGreen = kp;
			}

			if (addition == 5)
			{
				Koopas* kp = new Koopas(220, 0, 2,1);
				kp->vy = 0.2f;
				kp->SetState(KOOPAS_STATE_IDLE);
				kp->SetAnimationSet(AnimationSets::GetInstance()->Get(311));
				//kp->SetPosition(200, 0);
				//objects.push_back(kp);
				addition -= 1;

				koopasBlack = kp;
			}
		}
		if (GetTickCount() - time_start > 5300 && GetTickCount() - time_start < 6050)
		{
			if (player1->isCollisionWithItem(koopasGreen))
			{
				koopasGreen->vy = -0.05f;
				koopasGreen->vx = -0.05f;

				player1->StartCollisionWithMushroom();
			}
		}

		if (GetTickCount() - time_start > 6050 && GetTickCount() - time_start < 6500)
		{
			koopasGreen->vy = 0.2f;
			koopasGreen->vx = 0;
		}

		if (GetTickCount() - time_start > 6500)
		{

		}

		if (GetTickCount() - time_start > 7500 && GetTickCount() - time_start < 8000)
		{
			player1->vy = -0.2f;
		}

		if (GetTickCount() - time_start > 8000)
		{
			if (player1->isCollisionWithItem(leaf))
			{
				player1->level = MARIO_LEVEL_TAIL;
				leaf->isFinish = true;

				player1->isJumping = false;
				player1->isWagging = true;

			}
		}

		if (GetTickCount() - time_start > 8100)
		{
			player1->vx = -0.05f;
		}

		if (GetTickCount() - time_start > 9000)
		{
			goomba->vx = -0.02f;
		}

		if (GetTickCount() - time_start > 9500)
		{
			player1->vx = 0;
		}

		if (GetTickCount() - time_start > 9500)
		{
			if (player1->isCollisionWithItem(goomba))
			{
				player1->vy = -0.1f;
				player1->vx = -0.2f;
				goomba->SetState(GOOMBA_STATE_DIE);
			}
		}

		if (GetTickCount() - time_start > 10000)
		{
			player1->vx = -0.1f;
		}

		if (GetTickCount() - time_start > 10300)
		{
			player1->vx = 0;
			player1->nx = 1;
		}

		if (GetTickCount() - time_start > 11000 && GetTickCount() - time_start < 12000)
		{
			player1->vx = 0.1f;

			if (player1->isCollisionWithItem(koopasGreen))
			{
				koopasGreen->vx = 0.2f;
				player1->StartKick();
			}
		}

		if (GetTickCount() - time_start > 12000)
		{
			player1->vx = 0.1f;

			if (koopasGreen->SweptAABBEx(koopasBlack))
			{
				koopasBlack->vy = -0.1f;
				koopasBlack->SetAnimationSet(AnimationSets::GetInstance()->Get(312));
			}
		}

		if (GetTickCount() - time_start > 12500)
		{
			koopasBlack->isFinish = true;
		}

		if (GetTickCount() - time_start > 13000)
		{
			player1->vx = 0;
			koopasBlack->vy = 0.2f;
		}

		if (GetTickCount() - time_start > 14000 && GetTickCount() - time_start < 14100)
		{
			player2->SetPosition(320, 170);
			player2->isHoldingItem = true;

			/*player2->SetHodingItem(koopasGreen);
			player2->vx = -0.2f;*/
		}

		if (GetTickCount() - time_start > 14000 && GetTickCount() - time_start < 14800)
		{
			koopasGreen->vx = 0;
			koopasGreen->vy = 0;
			/*player2->nx = -1;
			player2->SetHodingItem(koopasGreen);*/
			player2->vx = -0.05f;
			koopasGreen->x = player2->x - 10;
			koopasGreen->y = player2->y + 8;
		}

		if (GetTickCount() - time_start > 14800)
		{
			player2->vx = 0;
			player2->nx = -1;
		}

		if (GetTickCount() - time_start > 15000)
		{
			player2->isHoldingItem = false;

			if (GetTickCount() - time_start <= 15700)
			{
				player1->vx = -0.2f;
			}

			koopasGreen->vx = -0.25f;
			koopasGreen->vy = 0.2f;
		}

		if (GetTickCount() - time_start > 15000 && GetTickCount() - time_start < 15200)
		{
			player2->StartKick();
		}

		if (GetTickCount() - time_start > 15500 && GetTickCount() - time_start <= 15700)
		{
			player1->vy = -0.2f;
		}

		if (GetTickCount() - time_start > 15700 && GetTickCount() - time_start < 15800)
		{
			player1->vy = 0.2f;
			player1->vx = -0.05f;
		}

		if (GetTickCount() - time_start > 15800)// && GetTickCount() - time_start < 16000)
		{
			if (player1->isCollisionWithItem(koopasGreen))
			{
				/*koopasGreen->vx = 0;
				player1->nx = 1;
				player1->vy = -0.1f;*/
			}
			koopasGreen->vx = 0;

			if (GetTickCount() - time_start < 16000)
			{
				player1->vy = -0.1f;
				player1->vx = -0.2f;
			}
		}

		if (GetTickCount() - time_start > 16000 && GetTickCount() - time_start < 16500)
		{
			player1->vy = 0.2f;
			player1->vx = 0;
		}

		if (GetTickCount() - time_start > 16500)
		{
			//player1->vy = -0.2f;
			player1->vx = 0.2f;
		}

		if (GetTickCount() - time_start > 16650)
		{
			if (player1->isCollisionWithItem(koopasGreen) && GetTickCount() - time_start < 17000)
			{
				player1->StartKick();
				koopasGreen->vx = 0.2f;
				player1->vx = 0;
			}

			player1->vx = 0;
			koopasGreen->vx = 0.2f;
		}

		if (GetTickCount() - time_start > 17500)
		{
			player2->vx = 0.2f;
		}

		if (GetTickCount() - time_start > 18000 && GetTickCount() - time_start < 18100)
		{
			koopasGreen->SetPosition(0, 180);

		}

		if (GetTickCount() - time_start > 18100)
		{
			if (player1->isCollisionWithItem(koopasGreen) && player1->level != MARIO_LEVEL_SMALL)
			{
				koopasGreen->vx = 0;

				player1->level = MARIO_LEVEL_BIG;
			}
		}

		if (GetTickCount() - time_start > 19000)
		{
			if (player1->level == MARIO_LEVEL_BIG)
			{
				player1->level = MARIO_LEVEL_SMALL;
			}
		}

		if (GetTickCount() - time_start > 20000)
		{
			player1->vx = 0.15f;

			if (GetTickCount() - time_start > 21400)
			{
				player1->vx = 0;
			}

			if (GetTickCount() - time_start > 21600)
			{
				player1->vx = -0.15f;
			}

			if (GetTickCount() - time_start > 22100)
			{
				player1->vx = 0;
				isHiden = true;
			}

			if (GetTickCount() - time_start > 23000)
			{
				player1->vx = 0.15f;
			}
		}
	}

	if (GetTickCount() - time_start > 25000)
	{
		isHiden = false;
	}

	if (GetTickCount() - time_start > 26000)
	{
		if (addition == 4) {
			HidenWall* arrow = new HidenWall(90, 157, 0, 40);
			objects.push_back(arrow);

			Koopas* kp = new Koopas(0, 170, 3,1);
			kp->SetAnimationSet(AnimationSets::GetInstance()->Get(31));
			kp->SetState(KOOPAS_STATE_WALKING_RIGHT);
			kp->vx = 0.04f;
			objects.push_back(kp);
			addition -= 1;
		}
	}

	if (GetTickCount() - time_start > 27000)
	{
		if (addition == 3) {
			Koopas* kp = new Koopas(0, 170, 3,1);
			kp->SetAnimationSet(AnimationSets::GetInstance()->Get(31));
			kp->SetState(KOOPAS_STATE_WALKING_RIGHT);
			kp->vx = 0.04f;
			objects.push_back(kp);
			addition -= 1;
		}
	}

	if (GetTickCount() - time_start > 28000)
	{
		if (addition == 2) {
			Koopas* kp = new Koopas(0, 170, 3, 1);
			kp->SetAnimationSet(AnimationSets::GetInstance()->Get(31));
			kp->SetState(KOOPAS_STATE_WALKING_RIGHT);
			kp->vx = 0.04f;
			objects.push_back(kp);
			addition -= 1;
		}
	}

	if (GetTickCount() - time_start > 33000)
	{
		if (addition == 1) {
			Koopas* kp = new Koopas(0, 170, 3,1);
			kp->SetAnimationSet(AnimationSets::GetInstance()->Get(31));
			kp->SetState(KOOPAS_STATE_WALKING_RIGHT);
			kp->vx = 0.08f;
			objects.push_back(kp);
			addition -= 1;
		}
	}

	if (GetTickCount() - time_start > 38000)
	{
		HidenWall* arrow = new HidenWall(90, 157, 0, 41);
		objects.push_back(arrow);
	}

	vector<LPGAMEOBJECT> coObjects;

	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
	if (player1 != NULL)
		player1->Update(dt, &coObjects);

	if (player2 != NULL)
		player2->Update(dt, &coObjects);

	if (koopasGreen != NULL)
		koopasGreen->Update(dt, &coObjects);

	if (koopasBlack != NULL)
		koopasBlack->Update(dt, &coObjects);

	Game::GetInstance()->SetCamPosition(0, 0);

	if (GetTickCount() - time_start > 40000)
	{
		time_start = 0;
		isStart = false;
		addition = 10;

		time_switch = 0;
		Switchable = 0;
		isSwitch = 2;

		Game::GetInstance()->SwitchScene(4);
	}


	if (Game::GetInstance()->IsKeyDown(DIK_W))
	{
		if (GetTickCount() - time_start < 25000)
		{

			isSwitch -= 1;
			if (isSwitch == 1)
			{
				delete player1;
				delete player2;
				delete koopasGreen;
				delete koopasBlack;

				player1 = NULL;
				player2 = NULL;
				koopasGreen = NULL;
				koopasBlack = NULL;

				HidenWall* hd = new HidenWall(0, 0, 0, 51);
				objects.push_back(hd);

				HidenWall* arrow = new HidenWall(90, 157, 0, 40);
				objects.push_back(arrow);

				addition == 4;
				time_start = GetTickCount() - 25000;

			}
			else if (isSwitch == 0)
			{
				HidenWall* arrow = new HidenWall(90, 157, 0, 41);
				objects.push_back(arrow);
				SwitchScene();
			}
		}
	}

	if (isSwitch == 1)
	{
		if (GetTickCount() - time_switch > 2000)
		{
			isSwitch == 0;
			time_switch = 0;
			Game::GetInstance()->SwitchScene(3);
		}
	}

}

void IntroScene::Render()
{
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}
	if (player1 != NULL)
		player1->Render();

	if (player2 != NULL)
		player2->Render();

	if (koopasGreen != NULL)
		koopasGreen->Render();

	if (koopasBlack != NULL)
		koopasBlack->Render();

	if (isHiden)
	{
		Sprites::GetInstance()->Get(20024)->Draw(245, 96);
	}
}

void IntroScene::Unload()
{
	for (int i = 0; i < background.size(); i++)
	{
		delete background[i];
	}

	for (int i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}

	delete player1;
	delete player2;
	delete koopasGreen;
	delete koopasBlack;

	background.clear();
	objects.clear();
	player1 = NULL;
	player2 = NULL;
	koopasGreen = NULL;
	koopasBlack = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void IntroSceneKeyHandler::OnKeyDown(int KeyCode)
{

}

void IntroSceneKeyHandler::OnKeyUp(int KeyCode)
{

}

void IntroSceneKeyHandler::KeyState(BYTE* states)
{

}