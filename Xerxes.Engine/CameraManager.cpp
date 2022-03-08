#include "pch.h"
#include "CameraManager.h"
#include <stdexcept>

using namespace std;

CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
}

int CameraManager::CraeteCamera()
{
	cameras.insert_or_assign(++lastId, new Camera());
	if (cameras.size() == 1)
		activeId = lastId;
	return lastId;
}

Camera CameraManager::GetCameraById(int id)
{
	if (!cameras.contains(id))
		throw std::runtime_error("Key doesn\'t exists.");
	return *cameras[id];

}

int CameraManager::GetFirstAvailableId()
{
	if (cameras.size() <= 0)
		return -1;

	return cameras.cbegin()->first;
}

void CameraManager::OnInit()
{
	lastId = 1;
	activeId = -1;
}

void CameraManager::OnShutdown()
{
	for (auto& it : cameras) {
		delete it.second;
	}
	cameras.clear();
}

Camera* CameraManager::GetActiveCamera()
{
	if (activeId <= 0)
	{
		throw std::runtime_error("Active camera not set.");
	}
	return cameras.at(activeId);
}

void CameraManager::SetOutputSize(float width, float height)
{
	for (auto& it : cameras) {
		it.second->SetOutputSize(width, height);
	}
}
