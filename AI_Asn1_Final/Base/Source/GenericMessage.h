#ifndef GENERIC_MESSAGE_H
#define GENERIC_MESSAGE_H

#include "Message.h"
#include <string>

using std::string;

struct GenericMessage : public Message {

public:
	//Variable(s)
	string data;

	//Constructor(s) & Destructor
	GenericMessage(const int senderID) : Message(senderID) {}
	GenericMessage(const GenericMessage& rhs) : Message(rhs.senderID) {
		this->data = rhs.data;
	}
	virtual ~GenericMessage() {}

	virtual GenericMessage* Copy() const {
		GenericMessage* copyPtr = new GenericMessage(*this);
		return copyPtr;
	}

};

#endif