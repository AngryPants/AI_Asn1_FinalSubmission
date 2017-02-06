#ifndef TURRET_IDLE_H
#define TURRET_IDLE_H

#include "State.h"
#include "Vector3.h"

class Turret;

class Turret_Idle : public State
{
public:
	Turret_Idle(Turret& owner);
	virtual ~Turret_Idle();

	void SetPosition(Vector3 &pos);
	virtual void Update(double dt);
	virtual void ReceiveMessage(const Message *message);

private:
	Turret *owner;
	float closestEnemyDist;
	Vector3 targetPosition;
	Vector3 direction;
	double directionResetTimer;
	int turretPreviousHP;
	int turretCurrentHP;

	void ResetDirection();

	void Reset();
	bool Think();
	void Act(double dt);
};

#endif