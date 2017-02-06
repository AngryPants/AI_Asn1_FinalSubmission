#ifndef BRAWLER_ATTACK_H
#define BRAWLER_ATTACK_H

#include "State.h"
#include "Vector3.h"
#include "CharacterInfo.h"
#include <vector>

using std::vector;

class Brawler;

class Brawler_Attack : public State
{
public:
	Brawler_Attack(Brawler& owner);
	virtual ~Brawler_Attack();

	virtual void Update(double dt);
	virtual void ReceiveMessage(const Message* message);

private:
	Brawler* owner;
	Vector3 targetPos;

	float attackTimer;
	float closestEnemy;
	float attackRange;

	void Reset();
	bool Think();
	void Act(double dt);
	vector<CharacterInfo> enemyInfos;
};

#endif