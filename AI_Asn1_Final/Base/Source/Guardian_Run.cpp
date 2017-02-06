#include "Guardian_Run.h"
#include "Guardian.h"

//Messages
#include "CharacterInfo.h"
#include "RequestHelp.h"

//Constructor(s) & Destructor
Guardian_Run::Guardian_Run(Guardian& owner) : Guardian_State("Run", owner) {
	randomDirectionTimer = 0.0f;
	canFollow = false;
	move = false;
}

Guardian_Run::~Guardian_Run() {
}

//Private Function(s)
bool Guardian_Run::Think(double deltaTime) {
	if (canFollow) {
		//If we are high enough HP, and there's actually people to follow, go back to following.
		if (static_cast<float>(GetOwner().GetHealth()) / static_cast<float>(GetOwner().GetMaxHealth()) >= 0.7f) {
			GetOwner().SetNextState("Follow");
			return false;
		}

		//If we are high enough Shield, and there's actually people to follow, go back to following.
		if (static_cast<float>(GetOwner().GetShield()) / static_cast<float>(GetOwner().GetMaxShield()) >= 0.6f) {
			GetOwner().SetNextState("Follow");
			return false;
		}
	}

	return true;
}

void Guardian_Run::Act(double deltaTime) {
	//Ask for help!
	RequestHelp requestHelp(GetOwner().GetID());
	requestHelp.name = GetOwner().name;
	requestHelp.position = GetOwner().GetPosition();
	requestHelp.inParty = GetOwner().InParty();
	GetOwner().SendMessageToObservers(&requestHelp);

	//Look for the closest support and run like crazy to it.
	//We need to know if there's a support to run to.
	if (destinations.empty()) { //No there isn't.
		//Let's run in a random direction.
		randomDirectionTimer -= deltaTime;
		if (randomDirectionTimer < 0.0) {
			GenerateRandomDirection();
			randomDirectionTimer = Math::RandFloatMinMax(3.0f, 5.0f);
		}
		GetOwner().Translate(randomDirection * GetOwner().GetMovementSpeed() * deltaTime);
		//GetOwner().Rotate(Math::RadianToDegree(atan2(randomDirection.y, randomDirection.x)));
		//cout << "Running Randomly" << endl;
		//cout << "Guardian_Run's Random Direction: " << randomDirection << endl;
	} else { //Yes there is.
		//Find the closest one.
		Vector3 closestPosition = destinations[0];
		float closestDistance = (closestPosition - GetOwner().GetPosition()).LengthSquared();
		for (unsigned int i = 1; i < destinations.size(); ++i) {
			float distance = (destinations[i] - GetOwner().GetPosition()).LengthSquared();
			if(distance < closestDistance) {
				closestDistance = distance;
				closestPosition = destinations[i];
			}
		}

		//Move towards it.
		Vector3 direction = closestPosition - GetOwner().GetPosition();
		if (move == false && direction.LengthSquared() > 5.0f * 5.0f) {
			move = true;
		} else if (move == true && direction.LengthSquared() < 3.0f * 3.0f) {
			move = false;
		}
		if (move && direction.LengthSquared() > Math::EPSILON) {
			cout << "Running" << endl;
			direction.Normalize();
			GetOwner().Translate(direction * GetOwner().GetMovementSpeed() * deltaTime);
			//GetOwner().Rotate(Math::RadianToDegree(atan2(randomDirection.y,randomDirection.x)));
		}
	}	
}

void Guardian_Run::GenerateRandomDirection() {
	do {
		randomDirection.Set(Math::RandFloatMinMax(-5.0f,5.0f),Math::RandFloatMinMax(-5.0f,5.0f),0);
	} while (randomDirection.LengthSquared() < Math::EPSILON);
	randomDirection.Normalize();
}

//Interface Function(s)
void Guardian_Run::Update(double deltaTime) {
	if (Think(deltaTime)) {
		Act(deltaTime);
	}
	canFollow = false;
	destinations.clear();
}

void Guardian_Run::ReceiveMessage(const Message* messagePtr) {
	//cout << "Guardian_Run has received a message." << endl;
	const CharacterInfo* charInfo = dynamic_cast<const CharacterInfo*>(messagePtr);
	if (charInfo != nullptr) {
		if (charInfo->inParty == GetOwner().InParty() && charInfo->isAlive) {
			if (charInfo->name == "Support") {
				destinations.push_back(charInfo->position);
				canFollow = true;
			} else if (charInfo->name == "Attacker") {
				canFollow = true;
			}
		}
	}
}