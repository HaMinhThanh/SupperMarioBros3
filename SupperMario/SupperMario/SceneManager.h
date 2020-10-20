#pragma once

#include "Scene.h"
#include "Camera.h"

class SceneManager
{
public:
	static SceneManager* GetInstance();
	SceneManager();
	~SceneManager();

	Scene* GetCurrentScene();

	void Update(DWORD dt);
	void Render();
	void LoadContent();
	void ReplaceScene(Scene* scene);

	void KeyState(BYTE* state);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);

private:	
	static SceneManager* mInstance;
	Scene* mCurrentScene;
};

