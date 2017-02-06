#ifndef MESSAGE_H
#define MESSAGE_H

struct Message {

protected:
	//Constructor(s)
	Message(const int id) : senderID(id) {}

public:	
	const int senderID;

	//Destructor
	virtual ~Message() {}
	virtual Message* Copy() const = 0;

};

#endif