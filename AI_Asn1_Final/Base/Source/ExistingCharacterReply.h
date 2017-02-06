#ifndef EXISTING_CHARACTER_REPLY_H
#define EXISTING_CHARACTER_REPLY_H

#include "Message.h"

struct ExistingCharacterReply : public Message {

public:
	bool inParty;

	ExistingCharacterReply(int senderID) : Message(senderID) {
		inParty = false;
	}
	virtual ~ExistingCharacterReply() {}

	ExistingCharacterReply* Copy() const {
		ExistingCharacterReply* copyPtr = new ExistingCharacterReply(*this);
		return copyPtr;
	}

};

#endif