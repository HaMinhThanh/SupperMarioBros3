#pragma once

#include "Scene.h"

class SceneManager
{
public:
	static SceneManager* GetInstance();
	~SceneManager();

	Scene* GetCurrentScene();
	void Update(DWORD dt);
	void ReplaceScene(Scene* scene);

private:
	SceneManager();
	static SceneManager* mInstance;
	Scene* mCurrentScene;
};

