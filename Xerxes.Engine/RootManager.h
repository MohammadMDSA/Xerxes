#pragma once

#include <memory>
#include "CameraManager.h"
#include "InputManager.h"

class RootManager
{
public:
	static RootManager*						GetInstance();
	static void								Destroy();

	std::shared_ptr<CameraManager>			GetCameraManager();
	std::shared_ptr<InputManager>			GetInputManager();

private:

	static RootManager*						instance;

	std::shared_ptr<CameraManager>			cameraManager;
	std::shared_ptr<InputManager>			inputManager;
	
	RootManager(RootManager const&) {};
	RootManager&							operator=(RootManager const&) {};


	RootManager();
	~RootManager();
};

