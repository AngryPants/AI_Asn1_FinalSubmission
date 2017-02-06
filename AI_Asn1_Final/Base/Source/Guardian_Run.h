#ifndef GUARDIAN_RUN_H
#define GUARDIAN_RUN_H

#include "Guardian_State.h"
#include <vector>
#include "Vector3.h"

using std::vector;

class Guardian_Run : public Guardian_State {

private:
	//Variable(s)
	vector<Vector3> destinations;
	Vector3 randomDirection;
	double randomDirectionTimer;
	bool canFollow;
	bool move;

	//Private Function(s)
	bool Think(double deltaTime);
	void Act(double deltaTime);
	void GenerateRandomDirection();

public:
	//Constructor(s) & Destructor
	Guardian_Run(Guardian& owner);
	virtual ~Guardian_Run();

	//Interface Function(s)
	virtual void Update(double deltaTime);
	virtual void ReceiveMessage(const Message* messagePtr);

};

#endif