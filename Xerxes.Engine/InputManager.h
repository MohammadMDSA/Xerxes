#pragma once
#include <memory>

class InputManager
{
public:
	InputManager();

	std::shared_ptr<DirectX::Mouse>		GetMouse();
	std::shared_ptr<DirectX::Keyboard>	GetKeyboard();

private:
	std::shared_ptr<DirectX::Mouse>		mouse;
	std::shared_ptr<DirectX::Keyboard>	keyboard;
};

