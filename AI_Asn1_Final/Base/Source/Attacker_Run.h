#ifndef ATTACKER_RUN_H
#define ATTACKER_RUN_H

#include "Attacker_State.h"
#include "Vector3.h"

class Attacker_Run : public Attacker_State {
	
private:
	Vector3 destination;
	bool canRun;
	bool move;

	void Sense(double deltaTime);
	bool Think(double deltaTime);
	void Act(double deltaTime);

public:
	//Constructor(s) & Destructor
	Attacker_Run(Attacker& owner);
	virtual ~Attacker_Run();

	//Interface Function(s)
	virtual void Update(double deltaTime);
	virtual void ReceiveMessage(const Message* messagePtr);

};

#endif