#ifndef CHARACTER_CREATED_H
#define CHARACTER_CREATED_H

#include "Message.h"
#include <string>

using std::string;

struct CharacterCreated : public Message {

public:
	bool inParty;
	string name;
	
	CharacterCreated(const int senderID);
	CharacterCreated(const CharacterCreated& rhs);
	virtual ~CharacterCreated();
	
	virtual CharacterCreated* Copy() const;

};

#endif