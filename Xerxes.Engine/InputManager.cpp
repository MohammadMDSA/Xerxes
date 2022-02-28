#include "pch.h"
#include "InputManager.h"

using namespace std;
using namespace DirectX;

InputManager::InputManager()
{
	mouse = make_shared<Mouse>();
	keyboard = make_shared<Keyboard>();
}

std::shared_ptr<DirectX::Mouse> InputManager::GetMouse()
{
	return mouse;
}

std::shared_ptr<DirectX::Keyboard> InputManager::GetKeyboard()
{
	return keyboard;
}
