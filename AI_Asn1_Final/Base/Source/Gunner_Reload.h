#ifndef GUNNER_RELOAD_H
#define GUNNER_RELOAD_H

#include "State.h"


class Gunner;

class Gunner_Reload :public State
{
public:
	Gunner_Reload(Gunner& owner);
	virtual ~Gunner_Reload();

	virtual void Update(double dt);
	virtual void ReceiveMessage(const Message* message);
private:
	float f_reloadTimer;
	Gunner* owner;

	bool Think();
	void Act(double dt);
	void Reset();
protected:


};

#endif