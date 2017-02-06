#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include "CameraBase.h"
#include "Vector3.h"

class Camera2D : public CameraBase {

public:
	Vector3 position;
	float width;
	float height;

	Mtx44 GetViewMatrix() {
		Mtx44 result;
		result.SetToLookAt(position.x, position.y, position.z,
		position.x, position.y, position.z - 1,	0, 1, 0);
		return result;
	}
	
	Camera2D() {
		width = 40.0f;
		height = 30.0f;
	}
	virtual ~Camera2D() {}

};

#endif