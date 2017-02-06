#ifndef CHARACTER_H
#define CHARACTER_H

#include "Vector3.h"
#include "Message.h"
#include "State.h"
#include <set>
#include <string>
#include <map>
#include "EntityBase.h"
#include "Mesh.h"
#include <queue>
#include <type_traits>
#include <exception>
#include "Mtx44.h"

using namespace std;

class Character : public EntityBase {

protected:
	//Variable(s)
	int health;
	int maxHealth;
	bool inParty;
	Mesh* mesh;
	State* currentState;
	map<string, State*> states;
	float radius;

public:
	//Variable(s)
	const string name;	
	Vector3 position;
	float rotation;

	//Constructor(s) & Destructor
	Character(const string& name);
	virtual ~Character();

	//Interface Function(s)
	int GetHealth() const;
	int GetMaxHealth() const;
	float GetHealthPercentage()
	{
		return ((float)this->health / (float)this->maxHealth)*100.f;
	}
	void SetMaxHealth(int hp) {
		if (hp < 1) {
			hp = 1;
		}
		this->maxHealth = hp;
	}
	const Vector3& GetPosition() const;
	float GetRotation() const;
	Vector3 GetDirection() const {
		Vector3 direction(1, 0, 0);
		Mtx44 mat;
		mat.SetToRotation(rotation, 0, 0, 1);
		return mat * direction;
	}
	bool InParty() const;

	virtual void SetHealth(const int health);
	virtual void Heal(const int health);
	virtual void Buff() {}
	virtual void TakeDamage(const int damage);
	bool IsAlive() const;
	
	virtual float GetRadius() const {
		return radius;
	}

	void Translate(const float x = 0.0f, const float y = 0.0f, const float z = 0.0f);
	void Translate(const Vector3& translation);
	void SetPosition(const float x = 0.0f, const float y = 0.0f, const float z = 0.0f);
	void SetPosition(const Vector3& position = Vector3(0, 0, 0));
	void Rotate(float rotation = 0.0f);
	void SetRotation(float rotation = 0.0f);
	
	//Messaging and states
	virtual void HandleMessage();
	virtual void SetNextState(const string& nextState) = 0;	
	virtual void Reset() = 0;

	virtual void Update(double deltaTime) = 0;
	virtual void Render() = 0;
	virtual void RenderUI() {}

};

#endif