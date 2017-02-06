#include "Turret_Reload.h"
#include "Turret.h"

Turret_Reload::Turret_Reload(Turret &owner) : State("Turret_Reload")
{
	this->owner = &owner;
	reloadTimer = 0.f;
}

Turret_Reload::~Turret_Reload()
{

}

void Turret_Reload::ReceiveMessage(const Message *message)
{
}

void Turret_Reload::Reset()
{
	reloadTimer = 0.f;
}

bool Turret_Reload::Think()
{
	if (owner->GetAmmo() == owner->clipSize)
	{
		owner->SetNextState("Turret_Attack");
		Reset();
		return false;
	}
	return true;
}

void Turret_Reload::Act(double dt)
{
	reloadTimer += dt;
	if (reloadTimer >= owner->reloadTime)
	{
		owner->Reload();
		cout << "Turret Reloading" << endl;
	}
}

void Turret_Reload::Update(double dt)
{
	if (Think() == true)
	{
		Act(dt);
	}
}