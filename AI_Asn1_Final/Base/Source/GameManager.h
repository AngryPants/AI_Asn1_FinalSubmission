#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "SingletonTemplate.h"
#include <list>
#include "ProximityMessage.h"

class Character;
class Bullet;
struct Message;

class GameManager : public Singleton<GameManager> {

	friend Singleton<GameManager>;

public:
	void Update(double _dt);
	void Render();
	void RenderUI();

	void AddCharacter(Character* character);
	void RemoveCharacter(Character* character);
	void AddBullet(Bullet* bullet);
	void RemoveBullet(Bullet* bullet);
	Bullet* GetBullet();
	//ProximityMessage GetClosestEnemy(Character* characterPtr);

private:
	GameManager();
	virtual ~GameManager();

	std::set<Character*> characterList;
	set<Bullet*> bulletList;

};

#endif // ENTITY_MANAGER_H