#include "Guardian_Follow.h"
#include "Guardian.h"

//Include Messages
#include "CharacterInfo.h"
#include "Guardian.h"

//Constructor(s) & Destructor
Guardian_Follow::Guardian_Follow(Guardian& owner) : Guardian_State("Follow", owner) {
}

Guardian_Follow::~Guardian_Follow() {
}

//Private Function(s)
bool Guardian_Follow::Think(double deltaTime) {
	//If we are low HP, run.
	Character& owner = GetOwner();
	if (static_cast<float>(GetOwner().GetHealth()) / static_cast<float>(GetOwner().GetMaxHealth()) <= 0.3f
		&& static_cast<float>(GetOwner().GetShield()) / static_cast<float>(GetOwner().GetMaxShield()) <= 0.1f) {
		GetOwner().SetNextState("Run");
		return false;
	}

	//Check if there's anyone to follow, or is everyone dead.
	if (attackerInfos.empty() && supportInfos.empty()) {
		GetOwner().SetNextState("Run");
	}

	if (static_cast<float>(GetOwner().GetShield()) / static_cast<float>(GetOwner().GetMaxShield()) > 0.8f) {
		//Does any attacker need our help?
		//for (unsigned int i = 0; i < attackerInfos.size(); ++i) {
		for (vector<CharacterInfo>::iterator vectorIter = attackerInfos.begin(); vectorIter != attackerInfos.end(); ++vectorIter) {
			if ((*vectorIter).currentState == "Run" || (*vectorIter).currentState == "Attack") {
				GetOwner().SetNextState("Defend");
				return false;
			}			
		}
		//Does any support need our help?
		//for (unsigned int i = 0; i < supportInfos.size(); ++i) {
		for (vector<CharacterInfo>::iterator vectorIter = supportInfos.begin(); vectorIter != supportInfos.end(); ++vectorIter) {
			if((*vectorIter).currentState == "Run") {
				GetOwner().SetNextState("Defend");
				return false;
			}
		}		
	}

	return true;
}

void Guardian_Follow::Act(double deltaTime) {
	direction.SetZero();
	//Find the closest guy and follow him. //If there's an attacker we follow him.
	if (attackerInfos.empty() == false) {
		float closestDistance = (attackerInfos[0].position - GetOwner().GetPosition()).LengthSquared();
		Vector3 closestPosition = attackerInfos[0].position;
		for (unsigned int i = 1; i < attackerInfos.size(); ++i) {
			float distance = (attackerInfos[i].position - GetOwner().GetPosition()).LengthSquared();
			if (distance < closestDistance) {
				closestDistance = distance;
				closestPosition = attackerInfos[i].position;
			}
		}
		direction = closestPosition - GetOwner().GetPosition();
	} else if (supportInfos.empty() == false) { //Otherwise we follow a support.
		float closestDistance = (supportInfos[0].position - GetOwner().GetPosition()).LengthSquared();
		Vector3 closestPosition = supportInfos[0].position;
		for (unsigned int i = 1; i < supportInfos.size(); ++i) {
			float distance = (supportInfos[i].position - GetOwner().GetPosition()).LengthSquared();
			if(distance < closestDistance) {
				closestDistance = distance;
				closestPosition = supportInfos[i].position;
			}
		}
		direction = closestPosition - GetOwner().GetPosition();		
	}

	float followDistance = 8.0f;
	if (direction.LengthSquared() > followDistance * followDistance) {
		direction.Normalize();
		GetOwner().Translate(direction * deltaTime * GetOwner().GetMovementSpeed());
		//GetOwner().SetRotation(Math::RadianToDegree(atan2(direction.y,direction.x)));
	}
}

//Interface Function(s)
void Guardian_Follow::Update(double deltaTime) {
	if (Think(deltaTime)) {
		Act(deltaTime);
	}
	attackerInfos.clear();
	supportInfos.clear();
}

void Guardian_Follow::ReceiveMessage(const Message* messagePtr) {
	//cout << "Guardian_Follow received a message." << endl;
	const CharacterInfo* charInfo = dynamic_cast<const CharacterInfo*>(messagePtr);
	if (charInfo != nullptr && charInfo->isAlive) {
		if (charInfo->inParty == GetOwner().InParty()) {
			if (charInfo->name == "Attacker") {
				this->attackerInfos.push_back(*charInfo->Copy());
			} else if (charInfo->name == "Support") {
				this->supportInfos.push_back(*charInfo->Copy());
			}
		}
	}
}