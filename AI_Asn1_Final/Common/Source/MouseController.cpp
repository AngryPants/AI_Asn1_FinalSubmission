#include "MouseController.h"

MouseController::MouseController() :
curr_posX(0.0), curr_posY(0.0),
prev_posX(0.0), prev_posY(0.0),
currBtnStatus(0), prevBtnStatus(0),
keepMouseCentered(true)
{
}

MouseController::~MouseController()
{
}

void MouseController::UpdateMousePosition(double _x, double _y)
{
	curr_posX = _x;
	curr_posY = _y;
}

void MouseController::UpdateMouseButtonPressed(int _slot)
{
	currBtnStatus |= 1 << _slot;
}

void MouseController::UpdateMouseButtonReleased(int _slot)
{
	currBtnStatus &= ~(1 << _slot);
}

void MouseController::EndFrameUpdate()
{
	prev_posX = curr_posX;
	prev_posY = curr_posY;
	prevBtnStatus = currBtnStatus;
}

bool MouseController::IsButtonDown(unsigned char _slot)
{
	return (currBtnStatus & (1 << _slot)) != 0;
}

bool MouseController::IsButtonUp(unsigned char _slot)
{
	return !(IsButtonDown(_slot));
}

bool MouseController::IsButtonPressed(unsigned char _slot)
{
	// True if currently button is down, previously is up
	return IsButtonDown(_slot) && !(prevBtnStatus & (1 << _slot));
}

bool MouseController::IsButtonReleased(unsigned char _slot)
{
	// True if currently button is up, previously is down
	return IsButtonUp(_slot) && (prevBtnStatus & (1 << _slot));
}

void MouseController::GetMousePosition(float& _resultX, float& _resultY)
{
	_resultX = (float)curr_posX;
	_resultY = (float)curr_posY;
}

void MouseController::GetMousePosition(double& _resultX, double& _resultY)
{
	_resultX = curr_posX;
	_resultY = curr_posY;
}

void MouseController::GetMouseDelta(float& _resultX, float& _resultY)
{
	_resultX = float(curr_posX - prev_posX);
	_resultY = float(curr_posY - prev_posY);
}

void MouseController::GetMouseDelta(double& _resultX, double& _resultY)
{
	_resultX = curr_posX - prev_posX;
	_resultY = curr_posY - prev_posY;
}