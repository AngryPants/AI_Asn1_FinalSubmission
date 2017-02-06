#ifndef PROXIMITY_MESSAGE_H
#define PROXIMITY_MESSAGE_H

#include "Message.h"
#include "Vector3.h"
#include "Character.h"

struct ProximityMessage : public Message {

public:
	float distance;
	Vector3 position;
	
	ProximityMessage(const int senderID) : Message(senderID) {}
	ProximityMessage(const ProximityMessage& rhs) : Message(rhs.senderID) {
		this->distance = rhs.distance;
		this->position = rhs.position;
	}
	virtual ~ProximityMessage() {}

	virtual ProximityMessage* Copy() const {
		return new ProximityMessage(*this);
	}

};

#endif