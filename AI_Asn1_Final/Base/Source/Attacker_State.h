#ifndef ATTACKER_STATE_H
#define ATTACKER_STATE_H

#include "State.h"

class Attacker;

class Attacker_State : public State {

private:
	//Variable(s)
	Attacker* owner;

public:
	//Constructor(s) & Destructor
	Attacker_State(const string& name, Attacker& owner) : State(name) {
		this->owner = &owner;
	}
	virtual ~Attacker_State() {}

	//Interface
	Attacker& GetOwner() {
		return *owner;
	}

};

#endif