#ifndef SUPPORT_H
#define SUPPORT_H

#include "Character.h"
#include "CharacterInfo.h"

class Support : public Character {

private:
	//Variable(s)
	//Movement
	float movementSpeed;

	//Message
	State* previousState;
	CharacterInfo info;

	//Mesh
	Mesh* healthMesh;
	Mesh* healBeam;
	Mesh* buffBeam;
	

	int healPerSec;


public:
	int targetID;
	float f_range;
	bool b_renderBeam;
	//Constructor(s) & Destructor
	Support();
	virtual ~Support();

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
		}
		else {
			string temp;
			return temp;
		}
	}

	//Combat
	void HealAlly();
	void Buff();
	void SelfHeal();


	//Messsaging and states
	virtual void HandleMessage();

};

#endif