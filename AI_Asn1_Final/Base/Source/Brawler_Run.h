#ifndef BRAWLER_RUN_H
#define BRAWLER_RUN_H

#include "State.h"
#include "Vector3.h"

class Brawler;

class Brawler_Run : public State
{
public:
	Brawler_Run(Brawler& owner);
	virtual ~Brawler_Run();

	virtual void Update(double dt);
	virtual void ReceiveMessage(const Message* message);

private:
	Brawler* owner;
	Vector3 enemyPos;
	Vector3 direction;
	Vector3 targetPosition;

	float closestEnemy;
	bool reset;
	float angleToRun;
	float rangeToRun;

	void Reset();
	bool Think();
	void Act(double dt);
};

#endif