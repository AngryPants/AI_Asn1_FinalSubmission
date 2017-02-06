#ifndef TURRET_ATTACK_H
#define TURRET_ATTACK_H

#include "State.h"
#include "Vector3.h"
#include "CharacterInfo.h"
#include <vector>

using std::vector;

class Turret;

class Turret_Attack : public State
{
public:
	Turret_Attack(Turret &owner);
	virtual ~Turret_Attack();

	virtual void Update(double dt);
	virtual void ReceiveMessage(const Message *message);

private:
	Turret* owner;
	Vector3 targetPos;
	float closestEnemyDist;
	float fireDebounce;	
	float attackRange;

	vector<CharacterInfo> enemyInfos;

	bool Think();
	void Act(double dt);
	void Reset();
};

#endif