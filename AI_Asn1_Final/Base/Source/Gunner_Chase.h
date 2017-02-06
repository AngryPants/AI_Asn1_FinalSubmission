#ifndef GUNNER_CHASE_H
#define GUNNER_CHASE_H

#include "State.h"
#include "Vector3.h"

class Gunner;

class Gunner_Chase :public State
{
public:
	Gunner_Chase(Gunner& owner);
	virtual ~Gunner_Chase();

	virtual void Update(double dt);
	virtual void ReceiveMessage(const Message* message);
private:
	float f_distanceToTarget;
	Vector3 targetPos;
	Gunner* owner;
	//int i_targetID;

	bool Think();
	void Act(double dt);
	void Reset();
protected:

};

#endif