#ifndef GUNNER_PATROL_H
#define GUNNER_PATROL_H

#include "State.h"
#include "Vector3.h"

class Gunner;

class Gunner_Patrol :public State
{
public:
	Gunner_Patrol(Gunner& owner);
	virtual ~Gunner_Patrol();

	void SetPatrolPoint(Vector3& pos);
	Vector3 patrolPoint;
	virtual void Update(double dt);
	virtual void ReceiveMessage(const Message* message);
private:
	float f_patrolRange;

	float f_closestEnemyDistance;
	Gunner* owner;
	int i_nearestID;

	const float cf_derpTime;
	float f_derpTimer; // idle timer upon reaching a patrol point

	void Reset();
	bool Think();
	void Act(double dt);
protected:


};

#endif