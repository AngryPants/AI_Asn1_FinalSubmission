#include "Gunner_Patrol.h"
#include "Gunner.h"

//Messages includes
#include "ProximityMessage.h"

Gunner_Patrol::Gunner_Patrol(Gunner& owner) :State("Gunner_Patrol")
, cf_derpTime(1.f)
{
	this->owner = &owner;
	this->f_closestEnemyDistance = FLT_MAX;
	this->f_patrolRange = 5.f;

	this->patrolPoint = owner.GetPosition();
	this->f_derpTimer = 0.f;
	i_nearestID = -1;
}

Gunner_Patrol::~Gunner_Patrol()
{
}

void Gunner_Patrol::ReceiveMessage(const Message* message)
{
	const CharacterInfo* ch = dynamic_cast<const CharacterInfo*>(message);
	if (ch)
	{
		if (ch->isAlive == true && ch->inParty != owner->InParty())
		{
			if (f_closestEnemyDistance * f_closestEnemyDistance > (ch->position - owner->position).LengthSquared())
			{
				f_closestEnemyDistance = (ch->position - owner->position).Length();
				i_nearestID = ch->senderID;
			}

		}
	}
	return;
}

void Gunner_Patrol::Reset()
{
	f_closestEnemyDistance = FLT_MAX;
}

bool Gunner_Patrol::Think()
{
	if (f_closestEnemyDistance <= owner->cf_detectionRange)
	{
		owner->SetNextState("Gunner_Chase");
		owner->i_targetID = i_nearestID;
		Reset();
		return false;
	}

	return true;
}

void Gunner_Patrol::Act(double dt)
{
	// check distance whether close to target pos
	if ((owner->GetPosition() - patrolPoint).LengthSquared() < (owner->f_movSpeed*dt) * (owner->f_movSpeed*dt))
	{
		patrolPoint.Set(owner->initialPos.x + Math::RandFloatMinMax(-f_patrolRange, f_patrolRange),
						owner->initialPos.y + Math::RandFloatMinMax(-f_patrolRange, f_patrolRange),
						0.f);
		f_derpTimer = cf_derpTime;
	}
	if (f_derpTimer > 0.f)
	{
		f_derpTimer -= dt;
	}
	else
	{
	
		Vector3 direction = patrolPoint - owner->GetPosition();
		float rotation = Math::RadianToDegree(atan2(direction.y, direction.x));
		owner->SetRotation(rotation);
		owner->SetPosition(owner->GetPosition() + direction.Normalized() * dt * owner->f_movSpeed);
	}
}

void Gunner_Patrol::Update(double dt)
{
	if (Think() == true)
	{
		Act(dt);
	}
}

void Gunner_Patrol::SetPatrolPoint(Vector3& pos)
{
	this->patrolPoint.Set(pos.x, pos.y, pos.z);
}