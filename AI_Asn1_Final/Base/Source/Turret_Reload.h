#ifndef TURRET_RELOAD_H
#define TURRET_RELOAD_H

#include "State.h"

class Turret;

class Turret_Reload : public State
{
public:
	Turret_Reload(Turret &owner);
	virtual ~Turret_Reload();

	virtual void Update(double dt);
	virtual void ReceiveMessage(const Message *message);

private:
	Turret *owner;
	float reloadTimer;

	bool Think();
	void Act(double dt);
	void Reset();
};
#endif