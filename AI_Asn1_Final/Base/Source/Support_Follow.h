#ifndef Support_Follow_H
#define Support_Follow_H

#include "State.h"
#include "Vector3.h"
#include <vector>

using std::vector;

class Support;
struct CharacterInfo;

class Support_Follow : public State {

private:
	//Variable(s)
	vector<CharacterInfo> attackerInfos;
	vector<CharacterInfo> guardianInfos;
	Vector3 direction;
	Support* owner;
	//Private Function(s)
	bool Think(double deltaTime);
	void Act(double deltaTime);

public:
	//Constructor(s) & Destructor
	Support_Follow(Support& owner);
	virtual ~Support_Follow();

	//Interface Function(s)
	virtual void Update(double deltaTime);
	virtual void ReceiveMessage(const Message* messagePtr);

};

#endif