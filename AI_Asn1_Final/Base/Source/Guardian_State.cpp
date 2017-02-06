#include "Guardian_State.h"
#include "Guardian.h"
#include "Message.h"

//Constructor(s)
Guardian_State::Guardian_State(const string& name, Guardian& owner) : State(name) {
	this->owner = &owner;
}

Guardian_State::~Guardian_State() {
}

//Interface
Guardian& Guardian_State::GetOwner() {
	return *owner;
}

void Guardian_State::ReceiveMessage(const Message* messagePtr) {
}