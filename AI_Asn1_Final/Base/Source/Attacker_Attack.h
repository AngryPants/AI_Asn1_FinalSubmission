#ifndef ATTACKER_ATTACK_H
#define ATTACKER_ATTACK_H

#include "Attacker_State.h"
#include "Vector3.h"
#include <map>
#include <vector>
#include "CharacterInfo.h"

using std::vector;

class Attacker_Attack : public Attacker_State {

public:
	enum ATTACK_MODE {
		ATTACK_FURTHEST,
		ATTACK_CLOSEST,
		ATTACK_HIGHEST_HP,
		ATTACK_LOWEST_HP,		

		NUM_ATTACK_MODE,
	};

private:
	//Variable(s)
	bool resetRandom;
	bool hasSupport;
	vector<CharacterInfo> enemyInfos;
	ATTACK_MODE currentMode;
	int currentTargetID;
	Vector3 targetPosition;
	float modeChance[NUM_ATTACK_MODE];

	//Private Function(s)
	bool Think(double deltaTime);
	void Act(double deltaTime);

	bool TargetCurrent();
	void TargetClosest();
	void TargetFurthest();
	void TargetLowestHP();
	void TargetHighestHP();

public:
	//Constructor(s) & Destructor
	Attacker_Attack(Attacker& owner);
	virtual ~Attacker_Attack();

	//Interface Function(s)
	virtual void Update(double deltaTime);
	virtual void ReceiveMessage(const Message* messagePtr);

	ATTACK_MODE GetAttackMode() const;

};

#endif