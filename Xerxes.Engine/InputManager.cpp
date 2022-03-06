#include "pch.h"
#include "InputManager.h"

#include <iostream>

using namespace std;
using namespace DirectX;

InputManager::InputManager() :
	mouseX(0.f),
	mouseY(0.f),
	mouseDeltaX(0.f),
	mouseDeltaY(0.f)
{
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

DirectX::Mouse::Mode InputManager::GetMouseMode()
{
	return mouseMode;
}

void InputManager::SetMouseMode(DirectX::Mouse::Mode mode)
{
	nextMouseMode = mode;
}

void InputManager::Update()
{
	auto previousMode = mouseMode;
	mouse->SetMode(nextMouseMode);
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
	auto newX = mouseState.x;
	auto newY = mouseState.y;
	if (previousMode != nextMouseMode)
	{
		mouseDeltaX = 0;
		mouseDeltaY = 0;
		if (nextMouseMode == Mouse::Mode::MODE_RELATIVE)
		{
			mouseX = mouseY = 0;
		}

	}
	else
	{
		mouseDeltaX = newX - mouseX;
		mouseDeltaY = newY - mouseY;

		mouseX = newX;
		mouseY = newY;
	}
	mouseMode = mouseState.positionMode;
}

void InputManager::OnInit()
{
	mouse = make_shared<Mouse>();
	keyboard = make_shared<Keyboard>();
}

void InputManager::OnShutdown()
{
}
