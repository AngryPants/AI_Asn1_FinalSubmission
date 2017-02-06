#include "EntityManager.h"
#include "EntityBase.h"
#include "Message.h"

void EntityManager::Update(double _dt)
{
	// Update all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Update(_dt);
	}

	// Clean up entities that are done
	it = entityList.begin();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = entityList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

void EntityManager::Render()
{
	// Render all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Render();
	}
}

void EntityManager::RenderUI()
{
	// Render all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->RenderUI();
	}
}

void EntityManager::AddEntity(EntityBase* _newEntity)
{
	entityList.push_back(_newEntity);
}

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
}

EntityBase* EntityManager::GetEntityByID(int id) {
	for (list<EntityBase*>::iterator listIter = entityList.begin(); listIter != entityList.end(); ++listIter) {
		EntityBase* entityPtr = *listIter;
		if (entityPtr->GetID() == id) {
			return entityPtr;
		}
	}
	return nullptr;
}

void EntityManager::BroadcastMessage(const Message* messagePtr) {
	for (list<EntityBase*>::iterator listIter = entityList.begin(); listIter != entityList.end(); ++listIter) {
		EntityBase* entityPtr = *listIter;
		if (entityPtr->GetID() != messagePtr->senderID) {
			entityPtr->ReceiveMessage(messagePtr);
		}		
	}
}