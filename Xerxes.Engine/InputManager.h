#pragma once
#include <memory>

class InputManager
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

	void								Update();

private:
	std::unique_ptr<DirectX::Mouse>		mouse;
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

};

