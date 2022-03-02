#include "pch.h"
#include "InputManager.h"

using namespace std;
using namespace DirectX;

InputManager::InputManager() :
	mouseX(0.f),
	mouseY(0.f),
	mouseDeltaX(0.f),
	mouseDeltaY(0.f)
{
	mouse = unique_ptr<Mouse>();
	keyboard = make_shared<Keyboard>();
}

DirectX::Mouse* InputManager::GetMouse()
{
	return mouse.get();
}

std::shared_ptr<DirectX::Keyboard> InputManager::GetKeyboard()
{
	return keyboard;
}

float InputManager::GetMouseX()
{
	return mouseX;
}

float InputManager::GetMouseY()
{
	return mouseY;
}

float InputManager::GetMouseDeltaX()
{
	return mouseDeltaX;
}

float InputManager::GetMouseDeltaY()
{
	return mouseDeltaY;
}

bool InputManager::GetLeftButton()
{
	return leftButton;
}

bool InputManager::GetRightButton()
{
	return rightButton;
}

bool InputManager::GetMiddleButton()
{
	return middleButton;
}

bool InputManager::GetLeftButtonUp()
{
	return leftButtonUp;
}

bool InputManager::GetRightButtonUp()
{
	return rightButtonUp;
}

bool InputManager::GetMiddleButtonUp()
{
	return middleButtonUp;
}

bool InputManager::GetLeftButtonDown()
{
	return leftButtonDown;
}

bool InputManager::GetRightButtonDown()
{
	return rightButtonDown;
}

bool InputManager::GetMiddleButtonDown()
{
	return middleButtonDown;
}

void InputManager::Update()
{
	auto mouseState = mouse->GetState();
	mouseTracker.Update(mouseState);
	leftButton = mouseState.leftButton;
	rightButton = mouseState.rightButton;
	middleButton = mouseState.middleButton;
	leftButtonUp = mouseTracker.leftButton == Mouse::ButtonStateTracker::RELEASED;
	rightButtonUp = mouseTracker.rightButton == Mouse::ButtonStateTracker::RELEASED;
	middleButtonUp = mouseTracker.middleButton == Mouse::ButtonStateTracker::RELEASED;
	leftButtonDown = mouseTracker.leftButton == Mouse::ButtonStateTracker::PRESSED;
	rightButtonDown = mouseTracker.rightButton == Mouse::ButtonStateTracker::PRESSED;
	middleButtonDown = mouseTracker.middleButton == Mouse::ButtonStateTracker::PRESSED;
	if (mouseState.positionMode == Mouse::MODE_ABSOLUTE)
	{
		mouse->SetMode(Mouse::MODE_RELATIVE);
	}
	else
	{
		auto newX = mouseState.x;
		auto newY = mouseState.y;
		mouseDeltaX = newX - mouseX;
		mouseDeltaY = newY - mouseY;
		mouseX = newX;
		mouseY = newY;
	}
}
