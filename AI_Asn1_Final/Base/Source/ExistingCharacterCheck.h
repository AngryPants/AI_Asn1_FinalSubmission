#ifndef EXISTING_CHARACTER_CHECK_H
#define EXISTING_CHARACTER_CHECK_H

#include "Message.h"

struct ExistingCharacterCheck : public Message {

public:
	//Constructor(s) & Destructor
	ExistingCharacterCheck(int senderID) : Message(senderID) {}
	virtual ~ExistingCharacterCheck() {}

	virtual ExistingCharacterCheck* Copy() const {
		ExistingCharacterCheck* copyPtr = new ExistingCharacterCheck(*this);
		return copyPtr;
	}

};

#endif