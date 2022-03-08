#pragma once
#include "EditorWindow.h"
#include "Camera.h"

class SceneWindow : public EditorWindow
{
public:
	SceneWindow(int id);

	void					SetCamera(Camera* camera);
	Camera*					GetCamera();

	virtual void			OnGUI() override;
	virtual void			Update(float deltaTime) override;

protected:
	// Inherited via EditorWindow
	virtual int				GetCustomWindowFlags() override;


private:

	Camera*					camera;
	const float				ROTATION_GAIN = 1.f;
	const float				MOVEMENT_GAIN = 5.f;
	const float				ROTATION_AROUND_FORWARD = 2.f;
	const float				ROTATION_AROUND_ROTATION_GAIN = 0.1f;
	const float				PAN_GAIN = 6.f;

	bool					moveingCamera;
};

