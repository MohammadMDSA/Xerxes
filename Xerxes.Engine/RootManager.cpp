#include "pch.h"
#include "RootManager.h"

using namespace std;

RootManager* RootManager::instance = 0;

RootManager* RootManager::GetInstance()
{
	if (!instance)
		RootManager::instance = new RootManager();

	return instance;
}

void RootManager::Destroy()
{
	delete instance;
	instance = 0;
}

CameraManager* RootManager::GetCameraManager()
{
	return cameraManager.get();
}

InputManager* RootManager::GetInputManager()
{
	return inputManager.get();
}

ResourceManager* RootManager::GetResourceManager()
{
	return resourceManager.get();
}

SceneManager* RootManager::GetSceneManager()
{
	return sceneManager.get();
}

void RootManager::Update(float deltaTime)
{
	inputManager->Update();
}

void RootManager::Init()
{
	cameraManager->OnInit();
	inputManager->OnInit();
	resourceManager->OnInit();
	sceneManager->OnInit();
}

void RootManager::Shutdown()
{
	cameraManager->OnShutdown();
	inputManager->OnShutdown();
	resourceManager->OnShutdown();
	sceneManager->OnShutdown();
}


RootManager::RootManager()
{
	instance = (RootManager*)this;
	cameraManager = shared_ptr<CameraManager>(new CameraManager());
	inputManager = shared_ptr<InputManager>(new InputManager());
	resourceManager = shared_ptr<ResourceManager>(new ResourceManager());
	sceneManager = shared_ptr<SceneManager>(new SceneManager());
	Init();
}

RootManager::~RootManager()
{
	
}
