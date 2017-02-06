#include "Attacker_Run.h"
#include "Attacker.h"
#include "RequestHelp.h"

//Constructor(s) & Destructor
Attacker_Run::Attacker_Run(Attacker& owner) : Attacker_State("Run", owner) {
	canRun = false;
	move = false;
}

Attacker_Run::~Attacker_Run() {
}

//Private Function(s)
void Attacker_Run::Sense(double deltaTime) {
}

bool Attacker_Run::Think(double deltaTime) {
	if (GetOwner().GetHealthPercentage() > 60.f) {
		GetOwner().SetNextState("Move");
		return false;
	} else if (!canRun) {
		GetOwner().SetNextState("Move");
		return false;
	}
	return true;
}

void Attacker_Run::Act(double deltaTime) {	
	Vector3 direction = destination - GetOwner().GetPosition();
	if(move == false && direction.LengthSquared() > 5.0f * 5.0f) {
		move = true;
	} else if(move == true && direction.LengthSquared() < 3.0f * 3.0f) {
		move = false;
	}
	if (move && direction.LengthSquared() > Math::EPSILON) {
		GetOwner().SetRotation(Math::RadianToDegree(atan2(direction.y, direction.x)));
		GetOwner().Translate(direction.Normalize() * GetOwner().GetMovementSpeed() * deltaTime);
	}
	RequestHelp helpMessage(GetOwner().GetID());
	helpMessage.inParty = GetOwner().InParty();
	helpMessage.name = GetOwner().name;
	helpMessage.position = GetOwner().GetPosition();
	GetOwner().SendMessageToObservers(&helpMessage);
}

//Interface Function(s)
void Attacker_Run::Update(double deltaTime) {
	if (Think(deltaTime)) {
		Act(deltaTime);
	}
	canRun = false;
}

void Attacker_Run::ReceiveMessage(const Message* messagePtr) {
	const CharacterInfo* charInfo = dynamic_cast<const CharacterInfo*>(messagePtr);
	if (charInfo!= nullptr) {
		if (charInfo->name == "Support" && charInfo->isAlive) {
			if (!canRun) {
				canRun = true;
				destination = charInfo->position;
			} else {
				if ((destination - GetOwner().GetPosition()).LengthSquared() > (charInfo->position - GetOwner().GetPosition()).LengthSquared()) {
					destination = charInfo->position;
				}
			}
		}
	}
}