#include "Turret_Idle.h"
#include "Turret.h"

#include "CharacterInfo.h"

Turret_Idle::Turret_Idle(Turret &owner) : State("Turret_Idle")
{
	this->owner = &owner;
	this->closestEnemyDist = FLT_MAX;
	directionResetTimer = 0.0;
	turretCurrentHP = owner.GetHealth();
	turretPreviousHP = owner.GetHealth();	
}

Turret_Idle::~Turret_Idle()
{
}

void Turret_Idle::ReceiveMessage(const Message *message)
{
	const CharacterInfo* charInfoPtr = dynamic_cast<const CharacterInfo*>(message);
	if(charInfoPtr != nullptr)
	{
		if(charInfoPtr->isAlive && charInfoPtr->inParty != owner->InParty()) {
			float distanceToEnemySquared = (charInfoPtr->position - owner->position).LengthSquared();
			if (distanceToEnemySquared < closestEnemyDist) {
				closestEnemyDist = distanceToEnemySquared;
				targetPosition = charInfoPtr->position;
			}
		}
	}
}

void Turret_Idle::Reset()
{
	closestEnemyDist = FLT_MAX;
}

bool Turret_Idle::Think()
{
	if (closestEnemyDist <= owner->detectionRange * owner->detectionRange && (targetPosition - owner->position).Dot(direction) > 0.0f)
	{
		owner->SetNextState("Turret_Attack");
		return false;
	}

	if (turretPreviousHP > turretCurrentHP) {
		owner->SetNextState("Turret_Attack");
		return false;
	}
	return true;
}

void Turret_Idle::Act(double dt)
{
	/*Vector3 direction = targetPosition - owner->GetPosition();
	float rotation = Math::RadianToDegree(atan2(direction.y, direction.x));
	owner->SetRotation(rotation);*/
	float rotation = Math::RadianToDegree(atan2(direction.y,direction.x));
	owner->SetRotation(rotation);
}

void Turret_Idle::Update(double dt)
{
	directionResetTimer -= dt;
	if (directionResetTimer < 0.0) {
		ResetDirection();
		directionResetTimer = 3.0;
	}
	turretPreviousHP = turretCurrentHP;
	turretCurrentHP = owner->GetHealth();
	if (Think() == true)
	{
		Act(dt);
	}
	closestEnemyDist = FLT_MAX;	
}

void Turret_Idle::ResetDirection() {
	do {
		direction.Set(Math::RandFloatMinMax(-1.0f, 1.0f), Math::RandFloatMinMax(-1.0f, 1.0f), 0.0f);
	} while (direction.LengthSquared() < Math::EPSILON);
	direction.Normalize();
}