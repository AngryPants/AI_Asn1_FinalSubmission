#include "CharacterCreated.h"

CharacterCreated::CharacterCreated(const int senderID) : Message(senderID) {
	inParty = false;
}

CharacterCreated::CharacterCreated(const CharacterCreated& rhs) : Message(rhs.senderID) {
	this->inParty = rhs.inParty;
	this->name = rhs.name;
}

CharacterCreated::~CharacterCreated() {
}

CharacterCreated* CharacterCreated::Copy() const {
	return new CharacterCreated(*this);
}