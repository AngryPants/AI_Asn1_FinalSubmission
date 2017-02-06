#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "SingletonTemplate.h"
#include <list>
#include <type_traits>
#include <iostream>

using namespace std;

class EntityBase;
struct Message;

class EntityManager : public Singleton<EntityManager>
{
	friend Singleton<EntityManager>;
public:
	void Update(double _dt);
	void Render();
	void RenderUI();

	void AddEntity(EntityBase* _newEntity);
	void BroadcastMessage(const Message* messagePtr);
	EntityBase* GetEntityByID(int id);

private:
	EntityManager();
	virtual ~EntityManager();

	std::list<EntityBase*> entityList;
};

#endif // ENTITY_MANAGER_H