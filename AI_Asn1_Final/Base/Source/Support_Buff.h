#ifndef SUPPORT_BUFF_H
#define SUPPORT_BUFF_H

#include "State.h"
#include "Vector3.h"
#include <vector>

using std::vector;

class Support;
struct CharacterInfo;

class Support_Buff : public State {

private:
	//Variable(s)
	Vector3 direction;
	Support* owner;
	bool b_changeToHeal;
	int i_nxtTarget;
	float f_closestDistance;


	//Private Function(s)
	bool Think();
	void Act(double dt);
	void Reset();

public:
	//Constructor(s) & Destructor
	Support_Buff(Support& owner);
	virtual ~Support_Buff();

	//Interface Function(s)
	virtual void Update(double dt);
	virtual void ReceiveMessage(const Message* messagePtr);

};

#endif