#include "EntityBase.h"
#include <iostream>
#include "Message.h"
#include "EntityManager.h"
#include <string>

using std::cout;
using std::endl;
using std::string;

set<int> EntityBase::usedGameObjectIDs;
set<int> EntityBase::freeGameObjectIDs;

EntityBase::EntityBase() :
isDone(false)
{
	id = GetGameObjectID();
	EntityManager::GetInstance()->AddEntity(this);
	isMessagingObservers = false;	
}

EntityBase::~EntityBase()
{
	//Add our new observers.
	for (set<EntityBase*>::iterator setIter = addQueue.begin(); setIter != addQueue.end(); ++setIter) {
		observers.insert(*setIter);
	}
	addQueue.clear();

	//Remove the observers.
	for(set<EntityBase*>::iterator setIter = removeQueue.begin(); setIter != removeQueue.end(); ++setIter) {
		set<EntityBase*>::iterator removeIter = observers.find(*setIter);
		if (removeIter != observers.end()) {
			observers.erase(removeIter);
		}
	}
	
	//Tell our observers to stop observing us.
	/*for (set<EntityBase*>::iterator setIter = observers.begin(); setIter != observers.end(); ++setIter) {
		EntityBase* entityPtr = *setIter;
		entityPtr->RemoveSubject(*this);
	}
	observers.clear();*/
	while (observers.empty() == false) {
		(*observers.begin())->RemoveSubject(*this);
	}

	//Stop observing our subjects.
	for (set<EntityBase*>::iterator setIter = subjects.begin(); setIter != subjects.end(); ++setIter) {
		EntityBase* entityPtr = *setIter;
		entityPtr->RemoveObserver(*this);
	}
	subjects.clear();	
	ReturnGameObjectID(id);
}

int EntityBase::GetGameObjectID() {
	int id = usedGameObjectIDs.size();
	if(freeGameObjectIDs.empty() == false) {
		id = *freeGameObjectIDs.begin();
		freeGameObjectIDs.erase(freeGameObjectIDs.begin());
	}
	usedGameObjectIDs.insert(id);
	return id;
}

void EntityBase::ReturnGameObjectID(const int id) {
	//Find the ID.
	set<int>::iterator setIter = usedGameObjectIDs.find(id);
	//Check if the ID is valid.
	if(setIter == usedGameObjectIDs.end()) {
		string errorMessage = "Error! ReturnGameObjectID() failed as ID is not valid.";
		cout << errorMessage << endl;
		throw std::exception(errorMessage.c_str());
	}
	//Do the necessary actions.
	usedGameObjectIDs.erase(setIter);
	freeGameObjectIDs.insert(id);
	//If there are no more usedGameObjectIDs, it means that there are no more gameObjects.
	if(usedGameObjectIDs.empty()) {
		freeGameObjectIDs.clear();
	}
	//Remove the ids we don't need.
	while(!freeGameObjectIDs.empty() && !usedGameObjectIDs.empty() && *freeGameObjectIDs.rbegin() > *usedGameObjectIDs.rbegin()) {
		freeGameObjectIDs.erase(--freeGameObjectIDs.end());
	}
}

int EntityBase::GetID() const {
	return id;
}

void EntityBase::Update(double _dt)
{
}

void EntityBase::Render()
{
}

void EntityBase::RenderUI()
{
}

bool EntityBase::IsDone()
{
	return isDone;
}

void EntityBase::SetIsDone()
{
	isDone = true;
}

void EntityBase::AddObserver(EntityBase& observer)
{
	//Check if it is ourself.
	if (&observer == this) {
		cout << "Cannot AddObserver(Character& observer) self." << endl;
		return;
	}
	//If we are not iterating through our observers, then just add it into the list.
	if (!isMessagingObservers) {
		observers.insert(&observer);
	} else { //Add it to the addQueue.
		addQueue.insert(&observer);
	}
}

void EntityBase::RemoveObserver(EntityBase& observer)
{
	if(&observer == this) {
		cout << "Cannot RemoveObserver(Character& observer) self." << endl;
		return;
	}
	if (!isMessagingObservers) {
		set<EntityBase*>::iterator setIter = observers.find(&observer);
		if (setIter != observers.end()) {
			observers.erase(setIter);
		}
	} else {		
		removeQueue.insert(&observer);
	}
}

void EntityBase::AddSubject(EntityBase& subject)
{
	if (&subject == this) {
		cout << "Cannot AddSubject(Character& subject) self." << endl;
		return;
	}
	subject.AddObserver(*this);
	subjects.insert(&subject);
}

void EntityBase::RemoveSubject(EntityBase& subject) {
	if (&subject == this) {
		cout << "Cannot RemoveSubject(Character& subject) self." << endl;
		return;
	}
	set<EntityBase*>::iterator setIter = subjects.find(&subject);
	if (setIter != subjects.end()) {
		subject.RemoveObserver(*this);
		subjects.erase(setIter);
	}
}

void EntityBase::SendMessageToObservers(const Message* message) {
	isMessagingObservers = true;

	//Add our new observers.
	for(set<EntityBase*>::iterator setIter = addQueue.begin(); setIter != addQueue.end(); ++setIter) {
		observers.insert(*setIter);
	}
	addQueue.clear();

	//Remove the observers.
	for(set<EntityBase*>::iterator setIter = removeQueue.begin(); setIter != removeQueue.end(); ++setIter) {
		set<EntityBase*>::iterator removeIter = observers.find(*setIter);
		if(removeIter != observers.end()) {
			observers.erase(removeIter);
		}
	}

	//Messages all our observers.
	for (set<EntityBase*>::iterator setIter = observers.begin(); setIter != observers.end(); ++setIter) {
		(*setIter)->ReceiveMessage(message);
	}
	
	removeQueue.clear();
	
	isMessagingObservers = false;
}

void EntityBase::ReceiveMessage(const Message* messagePtr) {
	messageQueue.push(messagePtr->Copy());
}

void EntityBase::HandleMessage() {
	while(messageQueue.size() != 0) {
		delete messageQueue.front();
		messageQueue.pop();
	}
}