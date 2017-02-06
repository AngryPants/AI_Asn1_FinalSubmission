#include "Attacker_Move.h"
#include "Message.h"
#include "Attacker.h"

//Include Messages
#include "ProximityMessage.h"
#include "CharacterInfo.h"

//Constructor(s) & Destructor
Attacker_Move::Attacker_Move(Attacker& owner) : Attacker_State("Move", owner) {
	enemyNearby = false;

	/*waypoints.push_back(Vector3(30, 30, 0));
	waypoints.push_back(Vector3(30, -30, 0));
	waypoints.push_back(Vector3(-30, -30, 0));
	waypoints.push_back(Vector3(-30, 30, 0));
	waypointIndex = 0;*/
}

Attacker_Move::~Attacker_Move() {
}

//Private Function(s)
bool Attacker_Move::Think(double deltaTime) {
	if (enemyNearby) {
		GetOwner().SetNextState("Attack");
		return false;
	}
	if (GetOwner().GetNumBullets() != GetOwner().GetMaxBullets()) {
		GetOwner().SetNextState("Reload");
		return false;
	}

	return true;
}

void Attacker_Move::Act(double deltaTime) {
	/*Vector3 direction = waypoints[waypointIndex] - GetOwner().GetPosition();
	float minDistanceToWaypoint = 3.0f;
	if (direction.LengthSquared() > minDistanceToWaypoint * minDistanceToWaypoint) {
		GetOwner().Translate(direction.Normalize() * GetOwner().GetMovementSpeed() * deltaTime);
		GetOwner().SetRotation(Math::RadianToDegree(atan2(direction.y, direction.x)));
	} else {
		waypointIndex = (waypointIndex + 1) % waypoints.size();
	}*/
	Vector3 direction = destination - GetOwner().GetPosition();
	float minDistanceToWaypoint = 3.0f;
	if (direction.LengthSquared() > minDistanceToWaypoint * minDistanceToWaypoint) {
		GetOwner().Translate(direction.Normalize() * GetOwner().GetMovementSpeed() * deltaTime);
		GetOwner().SetRotation(Math::RadianToDegree(atan2(direction.y, direction.x)));
	} else {
		destination.Set(Math::RandFloatMinMax(-50.0f, 50.0f), Math::RandFloatMinMax(-50.0f, 50.0f), 0.0f);
	}
}

//Interface Function(s)
void Attacker_Move::Update(double deltaTime) {
	//Process Info
	if (Think(deltaTime)) {
		//Carry out the action.
		Act(deltaTime);
	}
	enemyNearby = false;
}

void Attacker_Move::ReceiveMessage(const Message* messagePtr) {
	//Check for nearby enemies.
	const CharacterInfo* infoPtr = dynamic_cast<const CharacterInfo*>(messagePtr);
	if (infoPtr != nullptr) {
		if (infoPtr->isAlive && infoPtr->inParty != GetOwner().InParty()) {
			if (enemyNearby == false) {
				float distanceSquared = (infoPtr->position - GetOwner().GetPosition()).LengthSquared();
				if (distanceSquared < GetOwner().GetMinCombatRange() * GetOwner().GetMinCombatRange()) {
					enemyNearby = true;
				}
			}
		}
	}
}