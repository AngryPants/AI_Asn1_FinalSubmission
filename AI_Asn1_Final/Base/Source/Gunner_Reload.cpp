#include "Gunner_Reload.h"
#include "Gunner.h"

Gunner_Reload::Gunner_Reload(Gunner& owner) :State("Gunner_Reload")
{
	this->owner = &owner;
	f_reloadTimer = 0.f;
}

Gunner_Reload::~Gunner_Reload()
{
}

void Gunner_Reload::ReceiveMessage(const Message* message)
{
}

void Gunner_Reload::Reset()
{
	f_reloadTimer = 0.f;
}

bool Gunner_Reload::Think()
{
	if (owner->GetAmmo() == owner->i_magSize)
	{
		owner->SetNextState("Gunner_Attack");
		Reset();
		return false;
	}
	return true;
}

void Gunner_Reload::Act(double dt)
{
	f_reloadTimer += dt;
	if (f_reloadTimer >= owner->cf_reloadTime)
	{
		owner->Reload();
	}
}

void Gunner_Reload::Update(double dt)
{
	if (Think() == true)
	{
		Act(dt);
	}
}