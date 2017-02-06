#ifndef BRAWLER_H
#define BRAWLER_H

#include "Character.h"
#include "CharacterInfo.h"

class Brawler : public Character 
{
public:
	Brawler();
	virtual ~Brawler();

	float moveSpeed;

	int damage;
	float attRange;
	float detectionRange;
	float attackRate;
	int targetID;

	Vector3 initialPos;

	void Attack(Vector3& direction);
	virtual float GetRadius() const;
	void InitPos(Vector3 pos);

	virtual void SetNextState(const string& nextState);
	virtual void Reset();
	virtual void Update(double dt);
	virtual void Render();
	virtual void RenderUI();
	virtual void HandleMessage();

private:
	CharacterInfo info;
	Mesh* healthMesh;
};

#endif