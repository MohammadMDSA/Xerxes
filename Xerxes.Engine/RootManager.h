#pragma once

#include <memory>
#include "CameraManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"

class RootManager
{
public:
	static RootManager*						GetInstance();
	static void								Destroy();

	CameraManager*							GetCameraManager();
	InputManager*							GetInputManager();
	ResourceManager*						GetResourceManager();
	SceneManager*							GetSceneManager();

	void									Update(float deltaTime);

private:

	static RootManager*						instance;

	std::shared_ptr<CameraManager>			cameraManager;
	std::shared_ptr<InputManager>			inputManager;
	std::shared_ptr<ResourceManager>		resourceManager;
	std::shared_ptr<SceneManager>			sceneManager;
	
	RootManager(RootManager const&) {};
	RootManager&							operator=(RootManager const&) {};

	void									Init();
	void									Shutdown();

	RootManager();
	~RootManager();
};

