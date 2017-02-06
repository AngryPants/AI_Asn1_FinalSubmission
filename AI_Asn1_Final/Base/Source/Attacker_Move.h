#ifndef ATTACKER_MOVE_H
#define ATTACKER_MOVE_H

#include "Attacker_State.h"
#include "Vector3.h"
#include "Mesh.h"
#include <vector>

using std::vector;

class Attacker_Move : public Attacker_State {

private:
	//Variable(s)
	bool enemyNearby;
	//vector<Vector3> waypoints;
	//int waypointIndex;
	Vector3 destination;

	//Private Function(s)
	bool Think(double deltaTime);
	void Act(double deltaTime);
	
public:
	//Constructor(s) & Destructor
	Attacker_Move(Attacker& owner);
	virtual ~Attacker_Move();

	//Interface Function(s)
	virtual void Update(double deltaTime);
	virtual void ReceiveMessage(const Message* messagePtr);

};

#endif