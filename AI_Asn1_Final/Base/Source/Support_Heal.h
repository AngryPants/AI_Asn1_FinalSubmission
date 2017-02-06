#ifndef SUPPORT_HEAL_H
#define SUPPORT_HEAL_H

#include "State.h"
#include "Vector3.h"
#include <vector>

using std::vector;

class Support;
struct CharacterInfo;

class Support_Heal : public State {

private:
	//Variable(s)
	Vector3 direction;
	Support* owner;
	bool b_canSwitchTarget;
	float f_closestDistance;
	int i_nxtTarget;
	float f_healTimer;


	//Private Function(s)
	bool Think();
	void Act(double dt);
	void Reset();

public:
	//Constructor(s) & Destructor
	Support_Heal(Support& owner);
	virtual ~Support_Heal();

	//Interface Function(s)
	virtual void Update(double dt);
	virtual void ReceiveMessage(const Message* messagePtr);

};

#endif