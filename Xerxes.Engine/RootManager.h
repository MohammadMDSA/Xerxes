#pragma once

#include <memory>
#include "CameraManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "SelectionManager.h"
#include "LightManager.h"
#include "XPreprocessors.h"
#include "Renderer.h"

using namespace Xerxes::Engine::SubsystemManager;

class RootManager
{
public:
	static RootManager*						GetInstance();
	static void								Destroy();

	inline CameraManager*					GetCameraManager() { return cameraManager.get(); }
	inline InputManager*					GetInputManager() { return inputManager.get(); }
	inline ResourceManager*					GetResourceManager() { return resourceManager.get(); }
	inline SceneManager*					GetSceneManager() { return sceneManager.get(); }
	inline SelectionManager*				GetSelectionManager() { return selectionManager.get(); }
	inline LightManager*					GetLightManager() { return lightManager.get(); }
	inline Renderer*						GetRenderer() { return renderer.get(); }

	void									Update(float deltaTime);

private:

	static RootManager*						instance;

	std::shared_ptr<CameraManager>			cameraManager;
	std::shared_ptr<InputManager>			inputManager;
	std::shared_ptr<ResourceManager>		resourceManager;
	std::shared_ptr<SceneManager>			sceneManager;
	std::shared_ptr<SelectionManager>		selectionManager;
	std::shared_ptr<LightManager>			lightManager;
	std::shared_ptr<Renderer>				renderer;
	
	RootManager(RootManager const&) {};
	RootManager&							operator=(RootManager const&) {};

	void									Init();
	void									Shutdown();

	RootManager();
	~RootManager();
};

