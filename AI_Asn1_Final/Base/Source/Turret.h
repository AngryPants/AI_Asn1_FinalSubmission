#ifndef TURRET_H
#define TURRET_H

#include "Character.h"

class Turret : public Character
{
public:
	Turret();
	virtual ~Turret();

	int	clipSize;
	int dmg;
	const float fireRate;
	const float reloadTime;
	const float attRange;
	const float detectionRange;

	Vector3 initialPos;

	int GetAmmo();
	void Reload();
	void Shoot(Vector3 &direction);
	void InitPos(Vector3 pos);
	virtual void SetNextState(const string &nextState);
	virtual void Reset();
	virtual void Update(double dt);
	virtual void Render();
	virtual void RenderUI();
	virtual void HandleMessage();

private:
	int ammo;
	Mesh* healthMesh;
	Mesh* ammoMesh;

};

#endif