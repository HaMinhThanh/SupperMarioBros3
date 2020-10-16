#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Mario.h"
#include "Textures.h"
#include "Sprites.h"
#include "Brick.h"
#include "Goomba.h"
#include "Koopas.h"

using namespace std;

#define WINDOW_CLASS_NAME L"Game Window"
#define MAIN_WINDOW_TITLE L"02 - Animation"
#define WINDOW_ICON_PATH L"mario.ico"

#define BRICK_TEXTURE_PATH L"brick.png"
#define MARIO_TEXTURE_PATH L"mario.png"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255,255,200)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define MAX_FRAME_RATE 90

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_TEX_ENEMY_2 30

Game* game;

Mario *mario;
GameObject *brick;
Goomba* goomba;
Koopas* koopas;

vector<LPGAMEOBJECT> objects;
LPDIRECT3DTEXTURE9 texMario = NULL;
LPDIRECT3DTEXTURE9 texBrick = NULL;
// class example
class SampleKeyHandler : public KeyEventHandler
{
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

SampleKeyHandler* keyHandler;

void SampleKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_A:
		mario->SetState(MARIO_STATE_IDLE);
		mario->SetLevel(MARIO_LEVEL_BIG);
		mario->SetPosition(50.0f, 0.0f);
		mario->SetSpeed(0, 0);
		break;
	}
}

void SampleKeyHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void SampleKeyHandler::KeyState(BYTE* states)
{
	if (mario->GetState() == MARIO_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		mario->SetState(MARIO_STATE_WALKING_LEFT);
	else mario->SetState(MARIO_STATE_IDLE);
}

//---------------------
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}
	return 0;
}

void LoadResources()
{
	/*Game* game = Game::GetInstance();
	texBrick = game->LoadTexture(BRICK_TEXTURE_PATH);
	texMario = game->LoadTexture(MARIO_TEXTURE_PATH);

	mario = new Mario(MARIO_START_X, MARIO_START_Y, MARIO_START_VX, texMario);
	brick = new GameObject(BRICK_X,BRICK_Y, texBrick);*/

	Textures* textures = Textures::GetInstance();

	textures->AddTexture(ID_TEX_MARIO, L"textures\\mario.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->AddTexture(ID_TEX_ENEMY, L"textures\\enemies.png", D3DCOLOR_XRGB(155, 219, 239));
	textures->AddTexture(ID_TEX_MISC, L"textures\\misc.png", D3DCOLOR_XRGB(156, 219, 239));
	textures->AddTexture(ID_TEX_ENEMY_2, L"textures\\enemies-2.png", D3DCOLOR_XRGB(156, 219, 239));

	Sprites* sprites = Sprites::GetInstance();
	Animations* animations = Animations::GetInstance();

	LPDIRECT3DTEXTURE9 texMario = textures->Get(ID_TEX_MARIO);

	// big mario
	sprites->AddSprite(10001, 246, 154, 259, 181, texMario);
	sprites->AddSprite(10002, 275, 154, 290, 181, texMario);
	sprites->AddSprite(10003, 304, 154, 321, 181, texMario);

	sprites->AddSprite(10011, 186, 154, 199, 181, texMario);
	sprites->AddSprite(10012, 155, 154, 170, 181, texMario);
	sprites->AddSprite(10013, 125, 154, 140, 181, texMario);

	// die
	sprites->AddSprite(10099, 215, 120, 231, 135, texMario);
	
	// small mario
	sprites->AddSprite(10021, 247, 0, 259, 15, texMario);
	sprites->AddSprite(10022, 275, 0, 291, 15, texMario);
	sprites->AddSprite(10023, 306, 0, 320, 15, texMario);

	sprites->AddSprite(10031, 187, 0, 198, 15, texMario);
	sprites->AddSprite(10032, 155, 0, 170, 15, texMario);
	sprites->AddSprite(10033, 125, 0, 139, 15, texMario);
	
	LPDIRECT3DTEXTURE9 texMisc = textures->Get(ID_TEX_MISC);
	// Brick
	sprites->AddSprite(20001, 408, 225, 424, 241, texMisc);

	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ENEMY);
	// Goomba
	sprites->AddSprite(30001, 5, 14, 21, 29, texEnemy);
	sprites->AddSprite(30002, 25, 14, 41, 29, texEnemy);
	sprites->AddSprite(30003, 45, 21, 61, 29, texEnemy);

	LPDIRECT3DTEXTURE9 texEnemy_2 = textures->Get(ID_TEX_ENEMY_2);
	// Koopas
	sprites->AddSprite(40001,170,192,185,217, texEnemy_2 );
	sprites->AddSprite(40002, 187, 192, 200, 217, texEnemy_2);

	sprites->AddSprite(40011,222 , 192, 237, 217, texEnemy_2);
	sprites->AddSprite(40012, 205, 192, 220, 217, texEnemy_2);


	LPANIMATION ani;

	ani = new Animation(100);		// idle big right
	ani->AddAnimation(10001);
	animations->AddAnimations(400, ani);

	ani = new Animation(100);		// idle big left
	ani->AddAnimation(10011);
	animations->AddAnimations(401, ani);

	ani = new Animation(100);		// idle small right
	ani->AddAnimation(10021);
	animations->AddAnimations(402, ani);

	ani = new Animation(100);		// idle small left
	ani->AddAnimation(10031);
	animations->AddAnimations(403, ani);

	ani = new Animation(100);		// walk right big
	ani->AddAnimation(10001);
	ani->AddAnimation(10002);
	ani->AddAnimation(10003);
	animations->AddAnimations(500, ani);

	ani = new Animation(100);		// walk left big
	ani->AddAnimation(10011);
	ani->AddAnimation(10012);
	ani->AddAnimation(10013);
	animations->AddAnimations(501, ani);

	ani = new Animation(100);		// walk right small
	ani->AddAnimation(10021);
	ani->AddAnimation(10022);
	ani->AddAnimation(10023);
	animations->AddAnimations(502, ani);

	ani = new Animation(100);		// walk left small
	ani->AddAnimation(10031);
	ani->AddAnimation(10032);
	ani->AddAnimation(10033);
	animations->AddAnimations(503, ani);

	ani = new Animation(100);		// Mario die
	ani->AddAnimation(10099);
	animations->AddAnimations(599, ani);
	
	ani = new Animation(100);		// Brick
	ani->AddAnimation(20001);
	animations->AddAnimations(601, ani);

	ani = new Animation(300);		// Goomba walk
	ani->AddAnimation(30001);
	ani->AddAnimation(30002);
	animations->AddAnimations(701, ani);

	ani = new Animation(1000);		// Goomba dead
	ani->AddAnimation(30003);
	animations->AddAnimations(702, ani);

	ani = new Animation(300);		// Koopa walk right
	ani->AddAnimation(40001);
	ani->AddAnimation(40002);
	animations->AddAnimations(801, ani);

	ani = new Animation(300);		// Koopa walk left
	ani->AddAnimation(40011);
	ani->AddAnimation(40012);
	animations->AddAnimations(802, ani);

	mario = new Mario();
	mario->AddAni(400);		// idle right big
	mario->AddAni(401);		// idle left big
	mario->AddAni(402);		// idle right small
	mario->AddAni(403);		// idle left small;

	mario->AddAni(500);		// walk right big
	mario->AddAni(501);		// walk left big
	mario->AddAni(502);		// walk right small
	mario->AddAni(503);		// walk left small

	mario->AddAni(599);		// die

	mario->SetPosition(50.0f, 0);
	objects.push_back(mario);

	/* Brick move
	sprites->AddSprite(20001, 300, 117, 315, 132, texMisc);
	sprites->AddSprite(20002, 318, 117, 333, 132, texMisc);
	sprites->AddSprite(20003, 336, 117, 351, 132, texMisc);
	sprites->AddSprite(20004, 354, 117, 369, 132, texMisc);
	

	ani = new Animation(100);
	ani->AddAnimation(20001, 1000);
	ani->AddAnimation(20002);
	ani->AddAnimation(20003);
	ani->AddAnimation(20004);
	animations->AddAnimations(510, ani);*/

	// Brick
	for (int i = 0; i < 30; i++)
	{
		Brick* brick = new Brick();
		brick->AddAni(601);
		brick->SetPosition(0 + i * 16.0f, 150);
		objects.push_back(brick);
	}
	for (int i = 0; i < 10; i++)
	{
		Brick* brick = new Brick();
		brick->AddAni(601);
		brick->SetPosition(50 + i * 16.0f, 100);
		objects.push_back(brick);
	}

	// Goomba
	for (int i = 0; i < 4; i++)
	{
		goomba = new Goomba();
		goomba->AddAni(701);
		goomba->AddAni(702);
		goomba->SetPosition(200 + i * 60, 135);
		goomba->SetState(GOOMBA_STATE_WALKING);
		objects.push_back(goomba);
	}

	// Koopas
	for (int i = 0; i < 4; i++)
	{
		koopas = new Koopas();
		koopas->AddAni(802);
		koopas->AddAni(801);
		koopas->SetPosition(200+ i*60, 85);
		koopas->SetState(KOOPAS_STATE_WALKING);
		objects.push_back(koopas);
	}
}

void Update(DWORD dt)
{
	vector <LPGAMEOBJECT> coObjects;

	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	float cx, cy;
	mario->GetPosition(cx, cy);

	cx -= SCREEN_WIDTH / 2;
	cy -= SCREEN_HEIGHT / 2;

	Game::GetInstance()->SetCamPosition(cx, 0.0f);
	
}

void Render()
{
	//Game* game = Game::GetInstance();
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Render();
		}

		spriteHandler->End();
		d3ddv->EndScene();
	}
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		DWORD ErrCode = GetLastError();
		DebugOut(L"[ERROR] CreateWindow failed!");// ErrCode:% d\nAt : % s% d \n", ErrCode, _W(__FILE__), __LINE__);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SetDebugWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				done = 1;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyBoard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}
	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = Game::GetInstance();
	game->Init(hWnd);

	keyHandler = new SampleKeyHandler();
	game->InitKeyBoard(keyHandler);

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}