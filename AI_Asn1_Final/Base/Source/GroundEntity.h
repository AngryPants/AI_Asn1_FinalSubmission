#ifndef GROUND_ENTITY_H
#define GROUND_ENTITY_H

#include "EntityBase.h"
#include "Vector3.h"
#include <string>

class Mesh;

class GroundEntity : public EntityBase
{
public:
	GroundEntity(Mesh* _modelMesh1, Mesh* _modelMesh2);
	virtual ~GroundEntity();

	virtual void Update(double _dt);
	virtual void Render();

	inline void SetPosition(const Vector3& _value){ position = _value; };
	inline Vector3 GetPosition(){ return position; };

	inline void SetScale(const Vector3& _value){ scale = _value; };
	inline Vector3 GetScale(){ return scale; };

private:
	Vector3 position;
	Vector3 scale;
	Mesh* modelMesh[2];
};

namespace Create
{
	GroundEntity* Ground(const std::string& _meshName1, const std::string& _meshName2);
};

#endif // GROUND_ENTITY_H