#include "Support_Heal.h"
#include "Support.h"

#include "EntityManager.h"

//Include Messages
#include "CharacterInfo.h"

Support_Heal::Support_Heal(Support& owner) :State("Heal")
{
	this->owner = &owner;
	b_canSwitchTarget = false;
	f_closestDistance = FLT_MAX;
	i_nxtTarget = -1;
	f_healTimer = 0.f;
}

Support_Heal::~Support_Heal()
{
}

void Support_Heal::Reset()
{
	b_canSwitchTarget = false;
	f_closestDistance = FLT_MAX;
	i_nxtTarget = -1;
	f_healTimer = 0.f;
	owner->targetID = -1;
	owner->b_renderBeam = false;
}

bool Support_Heal::Think()
{
	Character* ch = static_cast<Character*>(EntityManager::GetInstance()->GetEntityByID(owner->targetID));
	if (ch->GetHealthPercentage() == 100.f || ch->IsAlive() == false)
	{
		owner->SetNextState("Follow");
		Reset();
		return false;
	}
	
	return true;
}

void Support_Heal::Act(double dt)
{
	f_healTimer += dt;
	Character* target = static_cast<Character*>(EntityManager::GetInstance()->GetEntityByID(owner->targetID));
	if (b_canSwitchTarget == true && i_nxtTarget != -1)
	{
		b_canSwitchTarget = false;
		owner->targetID = i_nxtTarget;
		i_nxtTarget = -1;
	}
	if ((target->position - owner->position).LengthSquared() > owner->f_range * owner->f_range)
	{
		direction = (target->position - owner->position).Normalize();
		owner->position += direction*dt * owner->GetMovementSpeed();
	}
	else
	{
		if (f_healTimer >= 1.f)
		{
			f_healTimer = 0.f;
			owner->HealAlly();
			owner->b_renderBeam = true;
		}
		
	}
	if (target->GetHealthPercentage() > 85.f)
	{
		b_canSwitchTarget = true;
		f_closestDistance = FLT_MAX;
		i_nxtTarget = -1;
	}

}


void Support_Heal::Update(double dt)
{
	if (Think())
	{
		Act(dt);
	}
}

void Support_Heal::ReceiveMessage(const Message* messagePtr)
{
	if (b_canSwitchTarget == false)
	{
		return;
	}
	const CharacterInfo* charInfo = dynamic_cast<const CharacterInfo*>(messagePtr);
	if (charInfo)
	{
		if (charInfo->isAlive == true && charInfo->inParty == owner->InParty())
		{
			if ((owner->position - charInfo->position).LengthSquared() < f_closestDistance )
			{
				i_nxtTarget = charInfo->senderID;
				f_closestDistance = (owner->position - charInfo->position).LengthSquared();
			}
		}
	}
}