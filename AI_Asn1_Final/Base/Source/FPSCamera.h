#ifndef FPS_CAMERA_H
#define FPS_CAMERA_H

#include "CameraBase.h"

class FPSCamera : public CameraBase
{
public:
	FPSCamera();
	virtual ~FPSCamera();

	virtual Mtx44 GetViewMatrix();

	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();

private:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
};

#endif