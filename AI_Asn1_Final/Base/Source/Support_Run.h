#ifndef SUPPORT_RUN_H
#define SUPPORT_RUN_H

#include "State.h"
#include "Vector3.h"
#include <vector>

using std::vector;

class Support;
struct CharacterInfo;

class Support_Run : public State {

private:
	//Variable(s)
	Vector3 direction;
	Support* owner;
	bool b_haveDirection;
	float f_healTimer;


	//Private Function(s)
	bool Think();
	void Act(double dt);
	void Reset();

public:
	//Constructor(s) & Destructor
	Support_Run(Support& owner);
	virtual ~Support_Run();

	//Interface Function(s)
	virtual void Update(double dt);
	virtual void ReceiveMessage(const Message* messagePtr);

};

#endif