#pragma once

#include <memory>
#include "CameraManager.h"
#include "InputManager.h"

class RootManager
{
public:
	static RootManager*						GetInstance();
	static void								Destroy();

	CameraManager*							GetCameraManager();
	InputManager*							GetInputManager();

	void									Update();

private:

	static RootManager*						instance;

	std::shared_ptr<CameraManager>			cameraManager;
	std::shared_ptr<InputManager>			inputManager;
	
	RootManager(RootManager const&) {};
	RootManager&							operator=(RootManager const&) {};


	RootManager();
	~RootManager();
};

