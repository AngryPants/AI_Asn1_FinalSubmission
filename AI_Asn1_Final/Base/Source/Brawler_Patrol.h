#ifndef BRAWLER_PATROL_H
#define BRAWLER_PATROL_H

#include "State.h"
#include "Vector3.h"

class Brawler;

class Brawler_Patrol : public State
{
public:
	Brawler_Patrol(Brawler& owner);
	virtual ~Brawler_Patrol();

	void SetPatrolPoint(Vector3 pos);
	Vector3 patrolPoint;
	virtual void Update(double dt);
	virtual void ReceiveMessage(const Message* message);

private:
	Brawler* owner;
	Vector3 targetPosition;
	Vector3 direction;

	float patrolRange;
	float closestEnemy;
	
	float idleTimer;

	void Reset();
	bool Think();
	void Act(double dt);
};

#endif