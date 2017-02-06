#ifndef GUARDIAN_STATE_H
#define GUARDIAN_STATE_H

#include "State.h"

class Guardian;

class Guardian_State : public State {

private:
	//Variable(s)
	Guardian* owner;

public:
	//Constructor(s) & Destructor
	Guardian_State(const string& name, Guardian& owner);
	virtual ~Guardian_State();

	//Interface
	Guardian& GetOwner();
	virtual void ReceiveMessage(const Message* messagePtr);

};

#endif