#include "Brawler_Run.h"
#include "Brawler.h"
#include "Mtx44.h"

#include "ProximityMessage.h"

Brawler_Run::Brawler_Run(Brawler& owner) : State("Run")
{
	this->owner = &owner;
	closestEnemy = 0.f;
	reset = true;
	rangeToRun = 5.f;
	angleToRun = 0.f;
}

Brawler_Run::~Brawler_Run()
{
}

void Brawler_Run::ReceiveMessage(const Message* message)
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

void Brawler_Run::Reset()
{
	closestEnemy = 0.f;
	reset = true;
}

bool Brawler_Run::Think()
{
	if (closestEnemy >= owner->detectionRange)
	{
		//owner->SetNextState("Run");
		Reset();
		return false;
	}
	return true;
}

void Brawler_Run::Act(double dt)
{
	if (reset == true)
	{
		angleToRun = Math::RandFloatMinMax(-20.f, 20.f);
		reset = false;

		direction = (owner->GetPosition() - enemyPos).Normalize();
		Mtx44 rotation;
		rotation.SetToRotation(angleToRun, 0, 0, 1);
		direction = rotation * direction;
	}
	if (direction.LengthSquared() < Math::EPSILON)
	{
		direction = Vector3(1,0,0);
	}

	float rotate = Math::RadianToDegree(atan2(direction.y, direction.x));
	owner->SetRotation(rotate);
	owner->SetPosition(owner->GetPosition() + (direction * dt * owner->moveSpeed));
	closestEnemy = FLT_MAX;
}

void Brawler_Run::Update(double dt)
{
	if (Think() == true)
	{
		Act(dt);
	}
	closestEnemy = FLT_MAX;
}