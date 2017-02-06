#ifndef BULLET_H
#define BULLET_H

#include "EntityBase.h"
#include "Vector3.h"

class Mesh;

class Bullet : public EntityBase {

private:
	bool isActive;

public:
	Vector3 direction;
	Vector3 position;
	int damage;
	bool inParty;
	double lifeTime;
	Mesh* mesh;
	float radius;
	float speed;

	bool IsActive() {
		return isActive;
	}
	void SetActive() {
		isActive = true;
		lifeTime = 5.0f;
	}
	void SetInactive() {
		isActive = false;
	}

	Bullet();
	virtual ~Bullet();

	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void RenderUI();

};

#endif