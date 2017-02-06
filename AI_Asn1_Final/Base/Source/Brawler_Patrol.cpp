#include "Brawler_Patrol.h"
#include "Brawler.h"

#include "ProximityMessage.h"

Brawler_Patrol::Brawler_Patrol(Brawler& owner) : State("Patrol")
, idleTimer(1.f)
{
	this->owner = &owner;
	this->closestEnemy = FLT_MAX;

	this->patrolRange = 5.f;
	this->patrolPoint = owner.GetPosition();
	this->idleTimer = 0.f;
}

Brawler_Patrol::~Brawler_Patrol()
{
}

void Brawler_Patrol::ReceiveMessage(const Message* message)
{
	const CharacterInfo* charInfoPtr = dynamic_cast<const CharacterInfo*>(message);
	if (charInfoPtr != nullptr)
	{
		if (charInfoPtr->isAlive && charInfoPtr->inParty != owner->InParty()) {
			float distanceToEnemySquared = (charInfoPtr->position - owner->position).LengthSquared();
			if (distanceToEnemySquared < closestEnemy) {
				closestEnemy = distanceToEnemySquared;
				targetPosition = charInfoPtr->position;
			}
		}
	}
}

void Brawler_Patrol::Reset()
{
	closestEnemy = FLT_MAX;
}

bool Brawler_Patrol::Think()
{
	if (closestEnemy <= owner->detectionRange * owner->detectionRange)
	{
		owner->SetNextState("Attack");
		Reset();
		return false;
	}

	return true;
}

void Brawler_Patrol::Act(double dt)
{
	if ((owner->GetPosition() - patrolPoint).LengthSquared() < (owner->moveSpeed*dt) * (owner->moveSpeed*dt))
	{
		patrolPoint.Set(owner->initialPos.x + Math::RandFloatMinMax(-patrolRange, patrolRange),
			owner->initialPos.y + Math::RandFloatMinMax(-patrolRange, patrolRange),
			0.f);
		idleTimer = idleTimer;
	}
	if (idleTimer > 0.f)
	{
		idleTimer -= dt;
	}
	else
	{
		Vector3 direction = patrolPoint - owner->GetPosition();
		float rotation = Math::RadianToDegree(atan2(direction.y, direction.x));
		owner->SetRotation(rotation);
		owner->SetPosition(owner->GetPosition() + direction.Normalized() * dt * owner->moveSpeed);
	}
}

void Brawler_Patrol::Update(double dt)
{
	if (Think() == true)
	{
		Act(dt);
	}
	closestEnemy = FLT_MAX;
}

void Brawler_Patrol::SetPatrolPoint(Vector3 pos)
{
	this->patrolPoint.Set(pos.x, pos.y, pos.z);
}