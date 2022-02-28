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

std::shared_ptr<CameraManager> RootManager::GetCameraManager()
{
	return cameraManager;
}

std::shared_ptr<InputManager> RootManager::GetInputManager()
{
	return inputManager;
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
