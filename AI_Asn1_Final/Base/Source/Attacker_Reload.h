#ifndef ATTACKER_RELOAD_H
#define ATTACKER_RELOAD_H

#include "Attacker_State.h"
#include "Message.h"
#include <string>

using std::string;

class Attacker_Reload : public Attacker_State {

private:
	//Variable(s)
	double timeToReload;

public:
	//Constructor(s) & Destructor
	Attacker_Reload(Attacker& owner);
	virtual ~Attacker_Reload();

	//Interface Function(s)
	virtual void Update(double deltaTime);
	virtual void ReceiveMessage(const Message* messagePtr);

};

#endif