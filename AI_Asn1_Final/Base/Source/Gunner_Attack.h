#ifndef GUNNER_ATTACK
#define GUNNER_ATTACK

#include "State.h"
#include "Vector3.h"

class Gunner;

class Gunner_Attack :public State
{
public:
	Gunner_Attack(Gunner& owner);
	virtual ~Gunner_Attack();

	virtual void Update(double dt);
	virtual void ReceiveMessage(const Message* message);
private:
	float f_fireDebounceTimer;
	Gunner* owner;
	Vector3 targetPos;
	float f_distanceToTarget;
	float f_AttackRange;
	//int i_targetID;

	bool Think();
	void Act(double dt);
	void Reset();
protected:


};


#endif