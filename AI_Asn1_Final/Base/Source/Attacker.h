#ifndef ATTACKER_H
#define ATTACKER_H

#include "Character.h"
#include "CharacterInfo.h"

class Attacker : public Character {
	
private:
	//Variable(s)
	//Movement
	float movementSpeed;
	
	//Combat
	int	maxBullets;
	int numBullets;
	int damage;
	float minRange;
	float maxRange;
	float fireRate;
	float timeToFire;
	
	//Message
	State* previousState;
	CharacterInfo info;
	
	//Mesh
	Mesh* healthMesh;
	Mesh* ammoMesh;
	Mesh* redCircleMesh;
	Mesh* blueCircleMesh;

public:
	//Constructor(s) & Destructor
	Attacker();
	virtual ~Attacker();

	//Interface Function(s)
	virtual void SetNextState(const string& nextState);
	virtual void Reset();

	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void RenderUI();
		
	//Movement
	float GetMovementSpeed() const;
	
	//States
	string GetCurrentStateName() const {
		return currentState->name;
	}
	string GetPreviousStateName() const {
		if (previousState != nullptr) {
			return previousState->name;
		} else {
			string temp;
			return temp;
		}		
	}

	//Combat
	int GetNumBullets() const;
	int GetMaxBullets() const;
	void AddBullets(const int numBullets);
	void Shoot(float hitProbability);
	bool CanShoot() {
		return timeToFire <= 0.0f;
	}
	float GetMinCombatRange() const;
	float GetMaxCombatRange() const;
	virtual void Buff();

	//Messsaging and states
	virtual void HandleMessage();

};

#endif