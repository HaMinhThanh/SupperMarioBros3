#include "SceneManager.h"

SceneManager* SceneManager::mInstance = NULL;

SceneManager::SceneManager()
{
	
}

SceneManager::~SceneManager()
{

}

SceneManager* SceneManager::GetInstance()
{
	if (!mInstance)
		mInstance = new SceneManager();

	return mInstance;
}

Scene* SceneManager::GetCurrentScene()
{
	return mCurrentScene;
}

void SceneManager::Update(DWORD dt)
{
	mCurrentScene->Update(dt);
}

void SceneManager::ReplaceScene(Scene* scene)
{
	delete mCurrentScene;

	mCurrentScene = scene;
}

void SceneManager::KeyState(BYTE* state)
{
	mCurrentScene->KeyState(state);
}

void SceneManager::OnKeyDown(int KeyCode)
{
	mCurrentScene->OnKeyDown(KeyCode);
}

void SceneManager::OnKeyUp(int KeyCode)
{
	mCurrentScene->OnKeyUp(KeyCode);
}

void SceneManager::LoadContent()
{
	mCurrentScene->LoadContent();
}

void SceneManager::Update(DWORD dt)
{
	mCurrentScene->Update(dt);
}

void SceneManager::Update(DWORD dt)
{
	mCurrentScene->Update(dt);
}

Scene* SceneManager::GetCurrentScene()
{
	return mCurrentScene;
}
