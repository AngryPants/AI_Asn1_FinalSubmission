#ifndef CHARACTER_INFO_H
#define CHARACTER_INFO_H

#include "Message.h"
#include "Vector3.h"
#include <string>

using std::string;

struct CharacterInfo : public Message {

public:
	string name;
	string currentState;
	Vector3 position;
	int health;
	int maxHealth;
	bool isAlive;
	bool inParty;

	CharacterInfo(const int senderID) : Message(senderID) {
		health = -1;
		maxHealth = -1;
		inParty = false;
		isAlive = false;
	}
	virtual ~CharacterInfo() {}

	virtual CharacterInfo* Copy() const {
		CharacterInfo* copyPtr = new CharacterInfo(*this);
		return copyPtr;
	}

};

#endif