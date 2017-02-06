#ifndef GUARDIAN_H
#define GUARDIAN_H

#include "Character.h"
#include "CharacterInfo.h"

class Guardian : public Character {

private:
	//Variable(s)
	int shield;
	int maxShield;
	Mesh* shieldMesh;
	float shieldRadius;
	float movementSpeed;
	double shieldRechargeTimer;
	double buffTimer;
	CharacterInfo info;
	float priorityRange;

	//Mesh
	Mesh* healthMesh;
	Mesh* redCircleMesh;
	Mesh* blueCircleMesh;

public:
	//Constructor(s) & Destructor
	Guardian();
	virtual ~Guardian();

	//Health & Shield
	void SetShield(const int shield);
	int GetShield() const;
	int GetMaxShield() const;
	virtual void Heal(const int health);
	virtual void TakeDamage(const int health);
	virtual float GetRadius() const;
	virtual void Buff();
	float GetPriorityRange() const {
		return priorityRange;
	}

	//Movement
	float GetMovementSpeed() const;

	//Others
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void RenderUI();
	virtual void HandleMessage();

	//States
	virtual void SetNextState(const string& nextState);
	virtual void Reset();

};

#endif