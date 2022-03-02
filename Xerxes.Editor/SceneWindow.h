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
};

