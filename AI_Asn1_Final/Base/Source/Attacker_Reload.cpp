#include "Attacker_Reload.h"
#include "Attacker.h"

//Constructor(s) & Destructor
Attacker_Reload::Attacker_Reload(Attacker& owner): Attacker_State("Reload",owner) {
	timeToReload = 0.0f;
}

Attacker_Reload::~Attacker_Reload() {
}

//Interface Function(s)
void Attacker_Reload::Update(double deltaTime) {
	if(GetOwner().GetNumBullets() >= GetOwner().GetMaxBullets()) {
		timeToReload = 0.0;
		GetOwner().SetNextState(GetOwner().GetPreviousStateName());
		return;
	}

	timeToReload -= deltaTime;
	if(timeToReload <= 0.0) {
		GetOwner().AddBullets(1);
		timeToReload = 0.2;
	}
}

void Attacker_Reload::ReceiveMessage(const Message* messagePtr) {
}