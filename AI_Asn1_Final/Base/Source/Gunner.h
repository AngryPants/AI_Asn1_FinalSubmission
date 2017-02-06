#ifndef GUNNER_H
#define GUNNER_H

#include "Character.h"
#include "CharacterInfo.h"

class Gunner : public Character {

public:
	Gunner();
	virtual ~Gunner();
	float f_movSpeed;

	int i_magSize;
	int i_damage;
	const float cf_fireRate;
	const float cf_reloadTime;
	const float cf_AttackRange;
	const float cf_detectionRange;
	int i_targetID;

	Vector3 initialPos;

	int GetAmmo();
	void Reload();
	void Shoot(Vector3& direction);
	void InitPos(Vector3 pos);
	virtual void SetNextState(const string& nextState);
	virtual void Reset();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void RenderUI();
	virtual void HandleMessage();

private:	
	int i_ammo;
	CharacterInfo info;
	Mesh* healthMesh;
	Mesh* ammoMesh;

protected:


};

#endif