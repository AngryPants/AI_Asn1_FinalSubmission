#include "Turret_Attack.h"
#include "Turret.h"

#include "ProximityMessage.h"

Turret_Attack::Turret_Attack(Turret &owner) : State("Turret_Attack")
{
	this->owner = &owner;
	fireDebounce = 1 / owner.fireRate;
	closestEnemyDist = FLT_MAX;
}

Turret_Attack::~Turret_Attack()
{

}

void Turret_Attack::ReceiveMessage(const Message *message)
{
	const CharacterInfo* charInfoPtr = dynamic_cast<const CharacterInfo*>(message);
	if (charInfoPtr != nullptr)
	{
		if (charInfoPtr->isAlive && charInfoPtr->inParty != owner->InParty()) {
			enemyInfos.push_back(*charInfoPtr);
		}
	}
}

void Turret_Attack::Reset()
{
	fireDebounce = 0.f;
}

bool Turret_Attack::Think()
{
	for (unsigned int i = 0; i < enemyInfos.size(); ++i)
	{
		float distanceSquared= (enemyInfos[i].position - owner->GetPosition()).LengthSquared();
		if (distanceSquared < closestEnemyDist) {
			closestEnemyDist = distanceSquared;
			targetPos = enemyInfos[i].position;
		}
	}

	if (closestEnemyDist >= owner->attRange * owner->attRange)
	{
		owner->SetNextState("Turret_Idle");
		Reset();
		return false;
	}
	if (owner->GetAmmo() == 0)
	{
		owner->SetNextState("Turret_Reload");
		Reset();
		return false;
	}

	return true;
}

void Turret_Attack::Act(double dt)
{
	fireDebounce += dt;
	if (fireDebounce > 1.f / owner->fireRate)
	{
		Vector3 direction = targetPos - owner->GetPosition();
		float rotation = Math::RadianToDegree(atan2(direction.y, direction.x));
		owner->SetRotation(rotation);
		owner->Shoot(direction.Normalize());
		fireDebounce = 0.f;
	}
}

void Turret_Attack::Update(double dt)
{
	if (Think() == true)
	{
		Act(dt);
	}
	closestEnemyDist = FLT_MAX;
	enemyInfos.clear();
}