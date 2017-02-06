#ifndef GUARDIAN_FOLLOW_H
#define GUARDIAN_FOLLOW_H

#include "Guardian_State.h"
#include "Vector3.h"
#include <vector>

using std::vector;

class Guardian;
struct CharacterInfo;

class Guardian_Follow : public Guardian_State {

private:
	//Variable(s)
	vector<CharacterInfo> attackerInfos;
	vector<CharacterInfo> supportInfos;
	Vector3 direction;

	//Private Function(s)
	bool Think(double deltaTime);
	void Act(double deltaTime);

public:
	//Constructor(s) & Destructor
	Guardian_Follow(Guardian& owner);
	virtual ~Guardian_Follow();

	//Interface Function(s)
	virtual void Update(double deltaTime);
	virtual void ReceiveMessage(const Message* messagePtr);

};

#endif