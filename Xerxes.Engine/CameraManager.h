#pragma once
#include <unordered_map>
#include <memory>

#include "Camera.h"

class CameraManager
{
public:
	CameraManager();

	int							CraeteCamera();
	const Camera&				GetCameraById(int id) const;
	Camera*						GetActiveCamera();
	void						SetOutputSize(float width, float height);
	

private:
	int							GetFirstAvailableId();

	std::unique_ptr<std::unordered_map<int, Camera*>>	cameras;

	int							lastId;
	int							activeId;
};

