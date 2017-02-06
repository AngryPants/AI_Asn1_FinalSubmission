#include "GameManager.h"
#include "Character.h"
#include "Message.h"
#include "Bullet.h"

void GameManager::Update(double _dt)
{

	float boundaryX = 50;
	float boundaryY = 50;

	for (set<Character*>::iterator characterIter = characterList.begin(); characterIter != characterList.end(); ++characterIter) {
		Character* characterPtr = *characterIter;
		if (characterPtr->IsAlive()) {
			if (characterPtr->position.x > boundaryX) {
				characterPtr->position.x = -boundaryX;
			} else if (characterPtr->position.x < -boundaryX) {
				characterPtr->position.x = boundaryX;
			}
			if(characterPtr->position.y > boundaryY) {
				characterPtr->position.y = -boundaryY;
			} else if(characterPtr->position.y < -boundaryY) {
				characterPtr->position.y = boundaryY;
			}
		}
	}

	for (set<Bullet*>::iterator bulletIter = bulletList.begin(); bulletIter != bulletList.end(); ++bulletIter)
	{
		Bullet* bulletPtr = *bulletIter;
		if (bulletPtr->IsActive() == false) {
			continue;
		}
		for (set<Character*>::iterator characterIter = characterList.begin(); characterIter != characterList.end(); ++characterIter) {
			Character* characterPtr = *characterIter;
			if (characterPtr->InParty() == bulletPtr->inParty) {
				continue;
			}
			float distanceSquared = (bulletPtr->position - characterPtr->GetPosition()).LengthSquared();
			if (distanceSquared < (bulletPtr->radius + characterPtr->GetRadius()) * (bulletPtr->radius + characterPtr->GetRadius())) {
				characterPtr->TakeDamage(bulletPtr->damage);
				if (characterPtr->IsAlive() == false) {
					characterPtr->SetIsDone();
				}
				bulletPtr->SetInactive();
				break;
			}
		}
	}

}

void GameManager::Render()
{
}

void GameManager::RenderUI()
{
	// Render all entities
	std::set<Character*>::iterator it, end;
	end = characterList.end();
	for (it = characterList.begin(); it != end; ++it)
	{
		(*it)->RenderUI();
	}
}

void GameManager::AddCharacter(Character* character)
{
	characterList.insert(character);
}

void GameManager::RemoveCharacter(Character* character)
{
	characterList.erase(characterList.find(character));
}

void GameManager::AddBullet(Bullet* bullet)
{
	bulletList.insert(bullet);
}

void GameManager::RemoveBullet(Bullet* bullet)
{
	bulletList.erase(bulletList.find(bullet));
}

Bullet* GameManager::GetBullet() {
	for (set<Bullet*>::iterator bulletIter = bulletList.begin(); bulletIter != bulletList.end(); ++bulletIter)
	{
		if((*bulletIter)->IsActive() == false)
		{
			return (*bulletIter);
		}
	}
	Bullet* ptr;
	for(int i=0;i<10;++i)
	{
		ptr = new Bullet();
	}
	return ptr;
}

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}