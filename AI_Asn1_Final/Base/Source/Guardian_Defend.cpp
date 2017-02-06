#include "Guardian_Defend.h"
#include "Guardian.h"

//Constructor(s) & Destructor
Guardian_Defend::Guardian_Defend(Guardian& owner) : Guardian_State("Defend", owner) {
	move = false;
}

Guardian_Defend::~Guardian_Defend() {
}

//Private Function(s)
bool Guardian_Defend::Think(double deltaTime) {
	//If we are low shield
	if (static_cast<float>(GetOwner().GetShield()) / static_cast<float>(GetOwner().GetMaxShield()) <= 0.1f) {
		GetOwner().SetNextState("Follow");
		return false;
	}

	//If there's nobody left to defend.
	if (helpRequests.empty() && attackerInfos.empty() && supportInfos.empty()) {
		GetOwner().SetNextState("Run");
		return false;
	}

	//Check if anybody is even needs help.
	if (helpRequests.empty()) {
		bool needHelp = false;
		for (unsigned int i = 0; i < attackerInfos.size() && !needHelp; ++i) {
			if (attackerInfos[i].currentState == "Attack" || static_cast<float>(attackerInfos[i].health) / static_cast<float>(attackerInfos[i].maxHealth) <= 0.5f) {
				needHelp = true;
				needsGuarding.push_back(attackerInfos[i]);
			}
		}
		for (unsigned int i = 0; i < supportInfos.size() && !needHelp; ++i) {
			if (static_cast<float>(supportInfos[i].health) / static_cast<float>(supportInfos[i].maxHealth) <= 0.5f) {
				needHelp = true;
				needsGuarding.push_back(supportInfos[i]);
			}
		}
		if (!needHelp) {
			GetOwner().SetNextState("Follow");
			return false;
		}
	}

	return true;
}

void Guardian_Defend::Act(double deltaTime) {	
	//Let's defend those who need help the most first.
	Vector3 direction;
	if (helpRequests.empty() == false) {
		//Let's help the one closest to us.
		float closestDistance = (helpRequests[0].position - GetOwner().GetPosition()).LengthSquared();
		Vector3 closestPosition = helpRequests[0].position;
		for (unsigned int i = 1; i < helpRequests.size(); ++i) {
			float distance = (helpRequests[i].position - GetOwner().GetPosition()).LengthSquared();
			if (distance < closestDistance) {
				closestDistance = distance;
				closestPosition = helpRequests[i].position;
			}
		}

		direction = closestPosition - GetOwner().GetPosition();
	} else {
		//Let's priorities those that are low health and within a reasonable range first.
		//If nobody is within the priorityRange, then go to the one with the lowest health.
		float priorityRange = GetOwner().GetPriorityRange();
		float lowestHealthPercentage = static_cast<float>(needsGuarding[0].health) / static_cast<float>(needsGuarding[0].maxHealth);
		Vector3 lowestHealthPosition = needsGuarding[0].position;

		bool hasPriority = false; //Is there anyone within our priority range?
		float priorityLowestHealthPercentage = 0.4f;
		Vector3 priorityPosition;
		
		for (unsigned int i = 0; i < needsGuarding.size(); ++i) {
			//Find the distance.
			float distance = (needsGuarding[i].position - GetOwner().GetPosition()).LengthSquared();

			//Find the health Percentage
			float healthPercentage = static_cast<float>(needsGuarding[i].health) / static_cast<float>(needsGuarding[i].maxHealth);
			if(healthPercentage < lowestHealthPercentage) {
				lowestHealthPercentage = healthPercentage;
				priorityPosition = needsGuarding[i].position;
			}

			//Check if the distance is within our priority range.
			if (distance < priorityRange * priorityRange) {				
				if (healthPercentage < priorityLowestHealthPercentage) {
					priorityLowestHealthPercentage = healthPercentage;
					priorityPosition = needsGuarding[i].position;
					hasPriority = true;
				}
			}
		}

		if (hasPriority) {
			direction = priorityPosition - GetOwner().GetPosition();			
		} else {
			direction = lowestHealthPosition - GetOwner().GetPosition();
		}		
	}

	if(move == false && direction.LengthSquared() > 1.0f * 1.0f) {
		move = true;
	} else if(move == true && direction.LengthSquared() < 0.5f * 0.5f) {
		move = false;
	}
	if(move && direction.LengthSquared() > Math::EPSILON) {
		direction.Normalize();
		GetOwner().Translate(direction * GetOwner().GetMovementSpeed() * deltaTime);
		//GetOwner().SetRotation(Math::RadianToDegree(atan2(direction.y, direction.x)));
	}
}

//Interface Function(s)
void Guardian_Defend::Update(double deltaTime) {
	if (Think(deltaTime)) {
		Act(deltaTime);
	}
	attackerInfos.clear();
	supportInfos.clear();
	helpRequests.clear();
	needsGuarding.clear();
}

void Guardian_Defend::ReceiveMessage(const Message* messagePtr) {
	//Does anyone need help?
	const RequestHelp* helpMessage = dynamic_cast<const RequestHelp*>(messagePtr);
	if (helpMessage != nullptr) {
		if (helpMessage->inParty == GetOwner().InParty() && helpMessage->name != "Guardian") {
			helpRequests.push_back(*helpMessage);
		}
	}

	//Get information about the characters
	const CharacterInfo* charInfo = dynamic_cast<const CharacterInfo*>(messagePtr);
	if (charInfo != nullptr && charInfo->isAlive) {
		if (charInfo->inParty == GetOwner().InParty()) {
			if (charInfo->name == "Attacker") {
				attackerInfos.push_back(*charInfo);
			} else if (charInfo->name == "Support") {
				supportInfos.push_back(*charInfo);
			}
		}
	}
}