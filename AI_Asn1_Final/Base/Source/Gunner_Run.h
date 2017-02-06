#ifndef Gunner_Run_H
#define Gunner_Run_H

#include "State.h"
#include "Vector3.h"

class Gunner;

class Gunner_Run:public State
{
public:
	Gunner_Run(Gunner& owner);
	virtual ~Gunner_Run();

	virtual void Update(double dt);
	virtual void ReceiveMessage(const Message* message);
private:
	float f_distanceToTarget;
	Vector3 enemyPos;
	Gunner* owner;
	bool b_reset;
	float f_angleToRun;
	float f_rangeToRun;
	Vector3 direction;


	bool Think();
	void Act(double dt);
	void Reset();
protected:



};

#endif