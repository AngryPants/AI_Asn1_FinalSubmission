#ifndef SCENE_AI_H
#define SCENE_AI_H

#include "Scene.h"
#include "Camera2D.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "ShaderProgram.h"

class Attacker;
class TestChar;
class Gunner;
class Guardian;

class SceneAI : public Scene {

private:
	Camera2D camera;
	ShaderProgram* currProg;
	int worldWidth, worldHeight;
	double spawnTimer;
	bool spawnButtonDebounce;
	
	enum SPAWN_TYPE {
		SPAWN_ATTACKER=0,
		SPAWN_GUARDIAN,
		SPAWN_SUPPORT,
		SPAWN_GUNNER,
		SPAWN_TURRET,
		SPAWN_BRAWLER,
		SPAWN_MAX
	};

	SPAWN_TYPE currentSpawnType;

public:
	SceneAI();
	virtual ~SceneAI();

	virtual void Init();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void Exit();

};

#endif