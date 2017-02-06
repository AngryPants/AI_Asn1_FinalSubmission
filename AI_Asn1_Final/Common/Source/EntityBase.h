#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include <set>
#include <queue>
#include "Message.h"

using std::set;
using std::queue;

struct Message;

class EntityBase
{
private:
	set<EntityBase*> subjects; //Things that we subscribed to.
	set<EntityBase*> observers; //Things that subscribe to this.
	set<EntityBase*> addQueue; //Add Queue for observers.
	set<EntityBase*> removeQueue; //Remove Queue for observers.	
	bool isMessagingObservers;
	
	int id;
	static set<int> usedGameObjectIDs;
	static set<int> freeGameObjectIDs;

	int GetGameObjectID();
	void ReturnGameObjectID(const int id);

	void AddObserver(EntityBase& observer); //Another Character subscribes to us.
	void RemoveObserver(EntityBase& observer); //Another Character subscribes to us.

protected:
	queue<Message*> messageQueue;

public:
	EntityBase();
	virtual ~EntityBase();

	virtual void Update(double _dt);
	virtual void Render();
	virtual void RenderUI();

	void AddSubject(EntityBase& subject); //This Character observes the subject.
	void RemoveSubject(EntityBase& subject); //This Character stops observing the subject.
	void SendMessageToObservers(const Message* messagePtr);
	void ReceiveMessage(const Message* messagePtr);
	virtual void HandleMessage();
	int GetID() const;

	bool IsDone();
	void SetIsDone();

private:
	bool isDone;
};

#endif // ENTITY_BASE_H