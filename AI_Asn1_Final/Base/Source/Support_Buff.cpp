#include "Support_Buff.h"
#include "Support.h"

#include "EntityManager.h"

//Include Messages
#include "RequestHelp.h"

Support_Buff::Support_Buff(Support& owner) :State("Buff")
{
	this->owner = &owner;
	b_changeToHeal = false;
	f_closestDistance = FLT_MAX;
	i_nxtTarget = -1;
}

Support_Buff::~Support_Buff()
{
}

void Support_Buff::Reset()
{
	b_changeToHeal = false;
	f_closestDistance = FLT_MAX;
	i_nxtTarget = -1;
	//owner->targetID = -1;
	owner->b_renderBeam = false;
}

bool Support_Buff::Think()
{
	if (b_changeToHeal == true && i_nxtTarget != -1)
	{
		//b_changeToHeal = false;
		owner->targetID = i_nxtTarget;
		//i_nxtTarget = -1;
		owner->SetNextState("Heal");
		Reset();
		return false;
	}
	if (static_cast<Character*>(EntityManager::GetInstance()->GetEntityByID(owner->targetID))->IsAlive() == false)
	{
		owner->SetNextState("Follow");
		Reset();
		return false;
	}
	return true;
}

void Support_Buff::Act(double dt)
{
	Character* target = static_cast<Character*>(EntityManager::GetInstance()->GetEntityByID(owner->targetID));

	if ((target->position - owner->position).LengthSquared() > owner->f_range * owner->f_range)
	{
		direction = (target->position - owner->position).Normalize();
		owner->position += direction*dt * owner->GetMovementSpeed();
	}
	else
	{
		owner->b_renderBeam = true;
		owner->Buff();
	}


}


void Support_Buff::Update(double dt)
{
	if (Think())
	{
		Act(dt);
	}
}

void Support_Buff::ReceiveMessage(const Message* messagePtr)
{
	const RequestHelp* charInfo = dynamic_cast<const RequestHelp*>(messagePtr);
	if (charInfo != nullptr)
	{
		if (charInfo->inParty == owner->InParty())
		{
			if ((owner->position - charInfo->position).LengthSquared() < f_closestDistance)
			{
				b_changeToHeal = true;
				i_nxtTarget = charInfo->senderID;
				f_closestDistance = (owner->position - charInfo->position).LengthSquared();
			}
		}
	}
}