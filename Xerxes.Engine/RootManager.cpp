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

void RootManager::Update()
{
	inputManager->Update();
}

RootManager::RootManager()
{
	instance = (RootManager*)this;
	cameraManager = shared_ptr<CameraManager>(new CameraManager());
	inputManager = shared_ptr<InputManager>(new InputManager());
}

RootManager::~RootManager()
{
	
}
