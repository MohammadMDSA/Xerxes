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

inline CameraManager* RootManager::GetCameraManager()
{
	return cameraManager.get();
}

inline InputManager* RootManager::GetInputManager()
{
	return inputManager.get();
}

inline ResourceManager* RootManager::GetResourceManager()
{
	return resourceManager.get();
}

inline SceneManager* RootManager::GetSceneManager()
{
	return sceneManager.get();
}

inline SelectionManager* RootManager::GetSelectionManager()
{
	return selectionManager.get();
}

inline LightManager* RootManager::GetLightManager()
{
	return lightManager.get();
}

inline Renderer* RootManager::GetRenderer()
{
	return renderer.get();
}

void RootManager::Update(float deltaTime)
{
	inputManager->Update();
}

void RootManager::Init()
{
	renderer->OnInit();
	cameraManager->OnInit();
	inputManager->OnInit();
	resourceManager->OnInit();
	sceneManager->OnInit();
	selectionManager->OnInit();
	lightManager->OnInit();
}

void RootManager::Shutdown()
{
	lightManager->OnShutdown();
	selectionManager->OnShutdown();
	sceneManager->OnShutdown();
	resourceManager->OnShutdown();
	inputManager->OnShutdown();
	cameraManager->OnShutdown();
	renderer->OnShutdown();
}


RootManager::RootManager()
{
	instance = (RootManager*)this;
	cameraManager = shared_ptr<CameraManager>(new CameraManager());
	inputManager = shared_ptr<InputManager>(new InputManager());
	resourceManager = shared_ptr<ResourceManager>(new ResourceManager());
	sceneManager = shared_ptr<SceneManager>(new SceneManager());
	selectionManager = shared_ptr<SelectionManager>(new SelectionManager());
	lightManager = shared_ptr<LightManager>(new LightManager());
	renderer = shared_ptr<Renderer>(new Renderer());
	Init();
}

RootManager::~RootManager()
{
	
}
