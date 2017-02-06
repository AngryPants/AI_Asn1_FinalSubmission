#include "Gunner_Run.h"
#include "Gunner.h"
#include "Mtx44.h"

#include "EntityManager.h"

Gunner_Run::Gunner_Run(Gunner& owner) :State("Gunner_Run")
{
	this->owner = &owner;
	f_distanceToTarget = 0.f;
	b_reset = false;
	f_rangeToRun = 2.f;
}

Gunner_Run::~Gunner_Run()
{
}

void Gunner_Run::ReceiveMessage(const Message* message)
{
	//const CharacterInfo* ch = dynamic_cast<const CharacterInfo*>(message);
	//if (ch)
	//{
	//	if (ch->isAlive == false && ch->inParty != owner->InParty)
	//	{
	//		if (f_closestEnemyDistance * f_closestEnemyDistance > (ch->position - owner->position).LengthSquared)
	//		{
	//			f_closestEnemyDistance = (ch->position - owner->position).Length();

	//		}

	//	}
	//}
	//return;
}

void Gunner_Run::Reset()
{
	f_distanceToTarget = FLT_MAX;
	b_reset = true;
}

bool Gunner_Run::Think()
{
	if (f_distanceToTarget >= owner->cf_detectionRange + f_rangeToRun)
	{
		owner->SetNextState("Gunner_Patrol");
		Reset();
		return false;
	}
	return true;
}

void Gunner_Run::Act(double dt)
{
	
	if(b_reset == true)
	{
		f_angleToRun = Math::RandFloatMinMax(-45.f, 45.f);
		b_reset = false;

		direction = owner->GetPosition() - enemyPos;
		Mtx44 rotation;
		rotation.SetToRotation(f_angleToRun,0,0,1);
		direction = rotation * direction;
	}
	if(direction.LengthSquared() < Math::EPSILON)
	{
		direction = owner->GetPosition() - enemyPos;
	}
	float rotate = Math::RadianToDegree(atan2(direction.y, direction.x));
	owner->SetRotation(rotate);
	owner->SetPosition(owner->GetPosition() + direction.Normalize() * dt * owner->f_movSpeed);
}

void Gunner_Run::Update(double dt)
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
		enemyPos = ch->position;
		Act(dt);
	}
}