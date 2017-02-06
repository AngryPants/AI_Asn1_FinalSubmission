#include "Brawler_Attack.h"
#include "Brawler.h"

#include "ProximityMessage.h"

Brawler_Attack::Brawler_Attack(Brawler& owner) : State("Attack")
{
	this->owner = &owner;
	attackTimer = 0;
	closestEnemy = FLT_MAX;
}

Brawler_Attack::~Brawler_Attack()
{
}

void Brawler_Attack::ReceiveMessage(const Message* message)
{
	const CharacterInfo* charInfoPtr = dynamic_cast<const CharacterInfo*>(message);
	if (charInfoPtr != nullptr)
	{
		if (charInfoPtr->isAlive && charInfoPtr->inParty != owner->InParty()) {
			float distanceToEnemySquared = (charInfoPtr->position - owner->position).LengthSquared();
			if (distanceToEnemySquared < closestEnemy) {
				closestEnemy = distanceToEnemySquared;
				owner->targetID = charInfoPtr->senderID;
				targetPos = charInfoPtr->position;
			}
		}
	}
}

void Brawler_Attack::Reset()
{
	attackTimer = 0.f;
	owner->targetID = -1;
}

bool Brawler_Attack::Think()
{
	if (closestEnemy >= owner->attRange * owner->attRange)
	{
		owner->SetNextState("Patrol");
		Reset();
		return false;
	}
	if (owner->GetHealth() < (owner->GetMaxHealth() * 0.5f))
	{
		owner->SetNextState("Run");
		Reset();
		return false;
	}

	return true;
}

void Brawler_Attack::Act(double dt)
{
	attackTimer += dt;
	if (attackTimer > 1.f / owner->attackRate)
	{
		Vector3 direction = targetPos - owner->GetPosition();
		float rotation = Math::RadianToDegree(atan2(direction.y, direction.x));
		owner->SetRotation(rotation);
		owner->Attack(direction.Normalize());
		attackTimer = 0.f;
	}
	closestEnemy = FLT_MAX;
}

void Brawler_Attack::Update(double dt)
{
	if (Think() == true)
	{
		Act(dt);
	}
	closestEnemy = FLT_MAX;
	enemyInfos.clear();
}