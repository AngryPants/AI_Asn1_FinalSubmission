#include "Support_Run.h"
#include "Support.h"

#include "EntityManager.h"

//Include Messages
#include "RequestHelp.h"

Support_Run::Support_Run(Support& owner) :State("Run")
{
	this->owner = &owner;
	b_haveDirection = false;
	f_healTimer = 0.f;
}

Support_Run::~Support_Run()
{
}

void Support_Run::Reset()
{
	b_haveDirection = false;
	owner->targetID = -1;
	f_healTimer = 0.f;
}

bool Support_Run::Think()
{
	if (owner->GetHealthPercentage() == 100.f)
	{
		owner->SetNextState("Follow");
		Reset();
		return false;

	}
	return true;
}

void Support_Run::Act(double dt)
{
	f_healTimer++;
	if (f_healTimer > 1.f)
	{
		owner->SelfHeal();
		f_healTimer = 0.f;
	}
	

	if (b_haveDirection == false)
	{
		float angle = Math::RandFloatMinMax(0.f, 360.f);
		Mtx44 rotation;
		rotation.SetToRotation(angle, 0, 0, 1);
		direction = rotation * Vector3(1,0,0);
	}
	owner->position += direction*dt * owner->GetMovementSpeed();
	RequestHelp requestHelp(owner->GetID());
	requestHelp.name = owner->name;
	requestHelp.position = owner->GetPosition();
	requestHelp.inParty = owner->InParty();
	owner->SendMessageToObservers(&requestHelp);

}


void Support_Run::Update(double dt)
{
	if (Think())
	{
		Act(dt);
	}
}

void Support_Run::ReceiveMessage(const Message* messagePtr)
{

}