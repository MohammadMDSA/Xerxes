#pragma once

#include <memory>
#include "CameraManager.h"
#include "InputManager.h"
#include "ResourceManager.h"

class RootManager
{
public:
	static RootManager*						GetInstance();
	static void								Destroy();

	CameraManager*							GetCameraManager();
	InputManager*							GetInputManager();
	ResourceManager*						GetResourceManager();

	void									Update();

private:

	static RootManager*						instance;

	std::shared_ptr<CameraManager>			cameraManager;
	std::shared_ptr<InputManager>			inputManager;
	std::shared_ptr<ResourceManager>		resourceManager;
	
	RootManager(RootManager const&) {};
	RootManager&							operator=(RootManager const&) {};


	RootManager();
	~RootManager();
};

