#ifndef GUARDIAN_DEFEND_H
#define GUARDIAN_DEFEND_H

#include "Guardian_State.h"
#include "Vector3.h"
#include <vector>

using std::vector;

//Include Messages
#include "RequestHelp.h"
#include "CharacterInfo.h"

class Guardian_Defend : public Guardian_State {

private:
	//Variable(s)
	vector<CharacterInfo> attackerInfos;
	vector<CharacterInfo> supportInfos;
	vector<RequestHelp> helpRequests;
	vector<CharacterInfo> needsGuarding;
	bool move;

	//Private Function(s)
	bool Think(double deltaTime);
	void Act(double deltaTime);

public:
	//Constructor(s) & Destructor
	Guardian_Defend(Guardian& owner);
	virtual ~Guardian_Defend();

	//Interface Function(s)
	virtual void Update(double deltaTime);
	virtual void ReceiveMessage(const Message* messagePtr);

};

#endif