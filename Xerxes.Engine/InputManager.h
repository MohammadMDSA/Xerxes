#pragma once
#include <memory>
#include "IManager.h"

class InputManager : public IManager
{
public:
	InputManager();

	DirectX::Mouse*						GetMouse();
	std::shared_ptr<DirectX::Keyboard>	GetKeyboard();
	float								GetMouseX();
	float								GetMouseY();
	float								GetMouseDeltaX();
	float								GetMouseDeltaY();
	bool								GetLeftButton();
	bool								GetRightButton();
	bool								GetMiddleButton();
	bool								GetLeftButtonUp();
	bool								GetRightButtonUp();
	bool								GetMiddleButtonUp();
	bool								GetLeftButtonDown();
	bool								GetRightButtonDown();
	bool								GetMiddleButtonDown();
	
	DirectX::Mouse::Mode				GetMouseMode();
	void								SetMouseMode(DirectX::Mouse::Mode mode);

	void								Update();

	// Inherited via IManager
	virtual void OnInit() override;
	virtual void OnShutdown() override;
private:
	std::shared_ptr<DirectX::Mouse>		mouse;
	std::shared_ptr<DirectX::Keyboard>	keyboard;
	DirectX::Mouse::ButtonStateTracker	mouseTracker;

	float								mouseX;
	float								mouseY;
	float								mouseDeltaX;
	float								mouseDeltaY;

	bool								leftButton;
	bool								leftButtonUp;
	bool								leftButtonDown;
	bool								rightButton;
	bool								rightButtonUp;
	bool								rightButtonDown;
	bool								middleButton;
	bool								middleButtonUp;
	bool								middleButtonDown;
	DirectX::Mouse::Mode				mouseMode;
	DirectX::Mouse::Mode				nextMouseMode;
};

