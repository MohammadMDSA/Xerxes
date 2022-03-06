#pragma once
#include <unordered_map>
#include "IManager.h"
#include "Camera.h"

class CameraManager : public IManager
{
public:
	CameraManager();
	~CameraManager();

	int							CraeteCamera();
	Camera						GetCameraById(int id);
	Camera*						GetActiveCamera();
	void						SetOutputSize(float width, float height);
	
	// Inherited via IManager
	virtual void OnInit() override;
	virtual void OnShutdown() override;

private:
	int							GetFirstAvailableId();

	std::unordered_map<int, Camera*>	cameras;

	int							lastId;
	int							activeId;

};

