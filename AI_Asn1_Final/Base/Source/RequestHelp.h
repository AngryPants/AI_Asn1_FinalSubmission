#ifndef REQUEST_HELP_H
#define REQUEST_HELP_H

#include "Message.h"
#include <string>
#include "Vector3.h"

using std::string;

struct RequestHelp : public Message {

public:
	bool inParty;
	Vector3 position;
	string name;

	RequestHelp(const int senderID) : Message(senderID) {
		inParty = false;
	}
	RequestHelp(const RequestHelp& rhs) : Message(rhs.senderID) {
		this->inParty = rhs.inParty;
		this->position = rhs.position;
		this->name = rhs.name;
	}
	virtual ~RequestHelp() {}

	virtual RequestHelp* Copy() const {
		return new RequestHelp(*this);
	}

};

#endif