#include "Gunner_Attack.h"
#include "Gunner.h"

//Messages includes
#include "EntityManager.h"

Gunner_Attack::Gunner_Attack(Gunner& owner) :State("Gunner_Attack")
{
	this->owner = &owner;
	f_fireDebounceTimer = 0;
	f_distanceToTarget = FLT_MAX;
	//i_targetID = -1;

}

Gunner_Attack::~Gunner_Attack()
{
}

void Gunner_Attack::ReceiveMessage(const Message* message)
{
	//const CharacterInfo* ch = dynamic_cast<const CharacterInfo*>(message);
	//if (ch)
	//{
	//	if (ch->isAlive == false && ch->inParty != owner->InParty)
	//	{
	//		if (f_closestEnemyDistance * f_closestEnemyDistance > (ch->position - owner->position).LengthSquared)
	//		{
	//			i_targetID = ch->senderID;
	//			f_closestEnemyDistance = (ch->position - owner->position).Length();
	//		}
	//		
	//	}
	//}
	//return;
}

void Gunner_Attack::Reset()
{
	f_fireDebounceTimer = 0.f;
	f_distanceToTarget = FLT_MAX;
}

bool Gunner_Attack::Think()
{
	if (f_distanceToTarget >= owner->cf_AttackRange)
	{
		owner->SetNextState("Gunner_Chase");
		Reset();
		return false;
	}
	if (owner->GetAmmo() == 0)
	{
		owner->SetNextState("Gunner_Reload");
		Reset();
		return false;
	}
	if (owner->GetHealth() < (owner->GetMaxHealth() * 0.2f))
	{
		owner->SetNextState("Gunner_Run");
		Reset();
		return false;
	}

	return true;
}

void Gunner_Attack::Act(double dt)
{
	f_fireDebounceTimer += dt;
	if (f_fireDebounceTimer > 1.f / owner->cf_fireRate)
	{
		Vector3 direction = targetPos - owner->GetPosition();
		float rotation = Math::RadianToDegree(atan2(direction.y, direction.x));
		owner->SetRotation(rotation);
		owner->Shoot(direction.Normalize());
		f_fireDebounceTimer = 0.f; 
	}

}

void Gunner_Attack::Update(double dt)
{
	Character* ch = dynamic_cast<Character*>(EntityManager::GetInstance()->GetEntityByID(owner->i_targetID));
	if (ch != nullptr && ch->IsAlive())
	{
		f_distanceToTarget = (ch->position - owner->position).Length();
	}
	else
	{
		f_distanceToTarget = FLT_MAX;
	}
	if (Think() == true)
	{
		targetPos = ch->position;
		Act(dt);
	}
}