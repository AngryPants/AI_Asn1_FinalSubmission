#include "Character.h"
#include "GameManager.h"

//Include Messages
#include "CharacterCreated.h"

//Constructor(s) & Destructor
Character::Character(const string& name) : name(name) {
	currentState = nullptr;
	maxHealth = 100;
	health = maxHealth;
	inParty = false;
	mesh = nullptr;
	rotation = 0.0f;
	radius = 0.5f;
	GameManager::GetInstance()->AddCharacter(this);
}

Character::~Character() {
	//Delete our states.
	currentState = nullptr;
	for (map<string, State*>::iterator mapIter = states.begin(); mapIter != states.end(); ++mapIter) {
		State* statePtr = mapIter->second;
		if (statePtr != nullptr) {
			delete statePtr;
		}
	}
	states.clear();
	GameManager::GetInstance()->RemoveCharacter(this);
}

//Interface Function(s)
int Character::GetHealth() const {
	return health;
}

int Character::GetMaxHealth() const {
	return maxHealth;
}

const Vector3& Character::GetPosition() const {
	return position;
}

float Character::GetRotation() const {
	return rotation;
}

bool Character::InParty() const {
	return inParty;
}

void Character::SetHealth(const int health) {
	this->health = health;
	if (this->health < 0) {
		this->health = 0;
	}
}

void Character::Heal(const int health) {
	if (health <= 0) {
		return;
	}
	this->health += health;
	if (this->health> this->maxHealth)
	{
		this->health = this->maxHealth;
	}
}

void Character::TakeDamage(const int damage) {
	if (damage <= 0) {
		return;
	}
	this->health -= damage;
}

bool Character::IsAlive() const {
	return (health > 0);
}

void Character::Translate(float x, float y, float z) {
	this->position += Vector3(x, y, z);
}

void Character::Translate(const Vector3& translation) {
	this->position += translation;
}

void Character::SetPosition(const float x, const float y, const float z) {
	this->position.Set(x, y, z);
}

void Character::SetPosition(const Vector3& position) {
	this->position = position;
}

void Character::Rotate(float rotation) {
	this->rotation = rotation;
}

void Character::SetRotation(float rotation) {
	this->rotation = rotation;
}

void Character::HandleMessage() {
	while (!messageQueue.empty()) {
		if (currentState != nullptr) {
			currentState->ReceiveMessage(messageQueue.front());
		}		
		delete messageQueue.front();
		messageQueue.pop();
	}
}