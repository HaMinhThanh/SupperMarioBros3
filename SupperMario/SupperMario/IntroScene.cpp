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

	int type;

	int ani_set_id = atoi(tokens[3].c_str());

	AnimationSets* animation_sets = AnimationSets::GetInstance();

	GameObject* obj = NULL;

	bool isBackground = false;

	switch (object_type)
	{
	case OBJECT_TYPE_RED_MARIO:
		obj = new Mario(x, y);
		player1 = (Mario*)obj;
		//DebugOut(L"[INFO] Player1 object created!\n");
		break;
	case OBJECT_TYPE_GREEN_MARIO:
		obj = new Mario(x, y);
		player2 = (Mario*)obj;
		//DebugOut(L"[INFO] Player2 object created!\n");
		break;

	case OBJECT_TYPE_BACKGROUND: {
		type = atoi(tokens[4].c_str());
		obj = new HidenWall(x,y,type);
		isBackground = true;
		break;
	}
	case OBJECT_TYPE_KOOPAS_GREEN:
		obj = new Koopas(x, y, 1);
		koopasGreen = (Koopas*)obj;
		break;
	case OBJECT_TYPE_KOOPAS_RED:
		obj = new Koopas(x, y, 1);
		koopasRed = (Koopas*)obj;
		break;
	case OBJECT_TYPE_GOOMBA:
		obj = new Goomba();
		goomba = (Goomba*)obj;
		break;
	case OBJECT_TYPE_LEAF:
		obj = new Leaf();
		leaf = (Leaf*)obj;
		break;
	case OBJECT_TYPE_MUSHROOM:
		obj = new Mushroom();
		isBackground = true;
		break;
	case OBJECT_TYPE_STAR:
		obj = new Star();
		isBackground = true;
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

	//if (!isStart)
	//{
	//	player1->nx = -1;
	//	player1->SetState(MARIO_STATE_IDLE);
	//	player1->SetLevel(MARIO_LEVEL_BIG);

	//	player2->SetState(MARIO_STATE_IDLE);
	//	player2->SetLevel(MARIO_LEVEL_BIG);

	//	HidenWall* hd = new HidenWall();

	//	time_start = GetTickCount();
	//	isStart = true;
	//}
	//else if (GetTickCount() - time_start > 800)
	//{
	//	player1->SetState(MARIO_STATE_WALKING_RIGHT);
	//	player2->SetState(MARIO_STATE_WALKING_LEFT);

	//	player1->vx = 0;
	//	player2->vx = 0;

	//}

	//if (GetTickCount() - time_start > 2000)
	//{
	//	player1->SetState(MARIO_STATE_WALKING_RIGHT);
	//	player2->SetState(MARIO_STATE_WALKING_LEFT);

	//}

	//if (GetTickCount() - time_start > 2500)
	//{
	//	player2->SetState(MARIO_STATE_JUMP);
	//	player2->SetState(MARIO_STATE_WALKING_RIGHT);

	//	// Create function for Collision mario with mario

	//	// Create condition if mairo walking through side limit
	//}

	//if (GetTickCount() - time_start > 3000)
	//{
	//	// rot chu
	//}

	//if (GetTickCount() - time_start > 3500)
	//{
	//	// rot cac item va enemy
	//}

	//if (GetTickCount() - time_start > 5000)
	//{
	//	player1->SetState(MARIO_STATE_JUMP);
	//}

	vector<LPGAMEOBJECT> coObjects;

	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < background.size(); i++)
	{
		background[i]->Update(dt);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	Game::GetInstance()->SetCamPosition(20, 0);
}

void IntroScene::Render()
{
	for (int i = 0; i < background.size(); i++)
	{
		background[i]->Render();
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
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

	background.clear();
	objects.clear();
	player1 = NULL;
	player2 = NULL;

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