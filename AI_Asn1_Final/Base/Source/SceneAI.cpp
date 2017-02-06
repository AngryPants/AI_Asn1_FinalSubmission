#include "SceneAI.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "EntityManager.h"
#include "GameManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "TestCharacter.h"
#include "Attacker.h"
#include "Gunner.h"
#include "Turret.h"
#include "Guardian.h"
#include "Support.h"
#include "Brawler.h"


SceneAI::SceneAI() {
}

SceneAI::~SceneAI() {
}

void SceneAI::Init() {
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	
	// Tell the shader program to store these uniform locations
	currProg->AddUniform("MVP");
	currProg->AddUniform("MV");
	currProg->AddUniform("MV_inverse_transpose");
	currProg->AddUniform("material.kAmbient");
	currProg->AddUniform("material.kDiffuse");
	currProg->AddUniform("material.kSpecular");
	currProg->AddUniform("material.kShininess");
	currProg->AddUniform("lightEnabled");
	currProg->AddUniform("numLights");
	currProg->AddUniform("lights[0].type");
	currProg->AddUniform("lights[0].position_cameraspace");
	currProg->AddUniform("lights[0].color");
	currProg->AddUniform("lights[0].power");
	currProg->AddUniform("lights[0].kC");
	currProg->AddUniform("lights[0].kL");
	currProg->AddUniform("lights[0].kQ");
	currProg->AddUniform("lights[0].spotDirection");
	currProg->AddUniform("lights[0].cosCutoff");
	currProg->AddUniform("lights[0].cosInner");
	currProg->AddUniform("lights[0].exponent");
	currProg->AddUniform("lights[1].type");
	currProg->AddUniform("lights[1].position_cameraspace");
	currProg->AddUniform("lights[1].color");
	currProg->AddUniform("lights[1].power");
	currProg->AddUniform("lights[1].kC");
	currProg->AddUniform("lights[1].kL");
	currProg->AddUniform("lights[1].kQ");
	currProg->AddUniform("lights[1].spotDirection");
	currProg->AddUniform("lights[1].cosCutoff");
	currProg->AddUniform("lights[1].cosInner");
	currProg->AddUniform("lights[1].exponent");
	currProg->AddUniform("colorTextureEnabled");
	currProg->AddUniform("colorTexture");
	currProg->AddUniform("textEnabled");
	currProg->AddUniform("textColor");
	
	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");
	
	currProg->UpdateInt("numLights", 0);
	currProg->UpdateInt("textEnabled", 0);
	currProg->UpdateInt("lightEnabled", 0);

	// Create and attach the camera to the scene
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	//World Width & Height
	worldWidth = 100;
	worldHeight = 100;

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");

	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);

	MeshBuilder::GetInstance()->GenerateSphere("Enemy_Bullet",Color(0,1,0),36,36,0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("Party_Bullet",Color(0,0,1),36,36,0.5f);
	MeshBuilder::GetInstance()->GenerateQuad("Background", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Background")->textureID = LoadTGA("Image//Background.tga");
	
	camera.position.Set(0, 0, 50);
	camera.width = 48;
	camera.height = 36;

	spawnButtonDebounce = false;
	currentSpawnType = SPAWN_ATTACKER;

	for (unsigned int i = 0; i < 2; ++i) {
		Turret* turret = new Turret();
		turret->InitPos(Vector3(0, 10,0));
	}

	for(unsigned int i = 0; i < 2; ++i) {
		Turret* turret = new Turret();
		turret->InitPos(Vector3(0,-10,0));
	}

	for(unsigned int i = 0; i < 2; ++i) {
		Turret* turret = new Turret();
		turret->InitPos(Vector3(-10,0,0));
	}

	for(unsigned int i = 0; i < 2; ++i) {
		Turret* turret = new Turret();
		turret->InitPos(Vector3(10,0,0));
	}

	Brawler* brawler = new Brawler();
	brawler->InitPos(Vector3(10,0,0));

	Gunner * gunner = new Gunner();
	gunner->InitPos(Vector3(-16,16,0));
	
	Attacker* attacker = new Attacker();

	Guardian* guardian = new Guardian();

	Support* sp = new Support();
	sp->position.Set(0,-4,0);

	spawnTimer = 0.0f;
}

void SceneAI::Update(double deltaTime) {
	
	// Update our entities
	EntityManager::GetInstance()->Update(deltaTime);
	GameManager::GetInstance()->Update(deltaTime);

	float camSpeed = 50.0f;
	if (KeyboardController::GetInstance()->IsKeyDown('A')) {
		camera.position.x -= camSpeed * deltaTime;
	}
	if (KeyboardController::GetInstance()->IsKeyDown('D')) {
		camera.position.x += camSpeed * deltaTime;
	}
	if (KeyboardController::GetInstance()->IsKeyDown('W')) {
		camera.position.y += camSpeed * deltaTime;
	}
	if (KeyboardController::GetInstance()->IsKeyDown('S')) {
		camera.position.y -= camSpeed * deltaTime;
	}

	camera.position.x = Math::Clamp(camera.position.x, -(worldWidth / 2 - camera.width / 2), (worldWidth / 2 - camera.width / 2));
	camera.position.y = Math::Clamp(camera.position.y, -(worldHeight / 2 - camera.height / 2), (worldHeight / 2 - camera.height / 2));

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	/*if(KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/

	/*spawnTimer -= deltaTime;
	if (spawnTimer < 0.0) {
		spawnTimer = 5.0;
		int randInt = Math::RandIntMinMax(0, 2);
		switch (randInt) {
			case 0: {
				Turret* turret = new Turret();
				turret->InitPos(Vector3(Math::RandFloatMinMax(-50.0f, 50.0f), Math::RandFloatMinMax(-50.0f, 50.0f), 0.0f));
				break;
			}
			case 1: {
				Gunner* gunner = new Gunner();
				gunner->InitPos(Vector3(Math::RandFloatMinMax(-50.0f, 50.0f), Math::RandFloatMinMax(-50.0f, 50.0f), 0.0f));
				break;
			}
			case 2:{
				Support* support = new Support();
				support->position = Vector3(Math::RandFloatMinMax(-50.0f, 50.0f), Math::RandFloatMinMax(-50.0f, 50.0f), 0.0f);
				break;
			}
		}
	}*/

	if(KeyboardController::GetInstance()->IsKeyDown('1')) {
		currentSpawnType = SPAWN_ATTACKER;
	} else if(KeyboardController::GetInstance()->IsKeyDown('2')) {
		currentSpawnType = SPAWN_GUARDIAN;
	} else if(KeyboardController::GetInstance()->IsKeyDown('3')) {
		currentSpawnType = SPAWN_SUPPORT;
	} else if(KeyboardController::GetInstance()->IsKeyDown('4')) {
		currentSpawnType = SPAWN_GUNNER;
	} else if(KeyboardController::GetInstance()->IsKeyDown('5')) {
		currentSpawnType = SPAWN_TURRET;
	} else if(KeyboardController::GetInstance()->IsKeyDown('6')) {
		currentSpawnType = SPAWN_BRAWLER;
	}

	
	if (KeyboardController::GetInstance()->IsKeyDown('F')) {
		if (spawnButtonDebounce == true) {
		spawnButtonDebounce = false;
			switch(currentSpawnType) {
				case SceneAI::SPAWN_ATTACKER: {
					Attacker* characterPtr = new Attacker();
					characterPtr->SetPosition(Vector3(Math::RandFloatMinMax(-static_cast<float>(worldWidth) * 0.8f, static_cast<float>(worldWidth) * 0.8f), Math::RandFloatMinMax(-static_cast<float>(worldHeight) * 0.8f, static_cast<float>(worldHeight) * 0.8f), 0.0f));
					break;
				}
				case SceneAI::SPAWN_GUARDIAN: {
					Guardian* characterPtr = new Guardian();
					characterPtr->SetPosition(Vector3(Math::RandFloatMinMax(-static_cast<float>(worldWidth)* 0.8f,static_cast<float>(worldWidth)* 0.8f),Math::RandFloatMinMax(-static_cast<float>(worldHeight)* 0.8f,static_cast<float>(worldHeight)* 0.8f),0.0f));
					break;
				}
				case SceneAI::SPAWN_SUPPORT: {			
					Support* support = new Support();
					support->position = Vector3(Math::RandFloatMinMax(-50.0f, 50.0f), Math::RandFloatMinMax(-50.0f, 50.0f), 0.0f);
					break;
				}			
				case SceneAI::SPAWN_GUNNER: {
					Gunner* characterPtr = new Gunner();
					characterPtr->SetPosition(Vector3(Math::RandFloatMinMax(-static_cast<float>(worldWidth)* 0.8f,static_cast<float>(worldWidth)* 0.8f),Math::RandFloatMinMax(-static_cast<float>(worldHeight)* 0.8f,static_cast<float>(worldHeight)* 0.8f),0.0f));
					break;
				}
				case SceneAI::SPAWN_TURRET: {
					Turret* characterPtr = new Turret();
					characterPtr->InitPos(Vector3(Math::RandFloatMinMax(-static_cast<float>(worldWidth)* 0.8f,static_cast<float>(worldWidth)* 0.8f),Math::RandFloatMinMax(-static_cast<float>(worldHeight)* 0.8f,static_cast<float>(worldHeight)* 0.8f),0.0f));
					break;
				}
				case SceneAI::SPAWN_BRAWLER: {
					Brawler* characterPtr = new Brawler();
					characterPtr->InitPos(Vector3(Math::RandFloatMinMax(-static_cast<float>(worldWidth)* 0.8f, static_cast<float>(worldWidth)* 0.8f), Math::RandFloatMinMax(-static_cast<float>(worldHeight)* 0.8f, static_cast<float>(worldHeight)* 0.8f), 0.0f));
					break;
				}
				default:
					break;
			}
		}
	} else {
		spawnButtonDebounce =  true;
	}

}

void SceneAI::Render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	//GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	GraphicsManager::GetInstance()->SetOrthographicProjection(-camera.width * 0.5f, camera.width * 0.5f, -camera.height * 0.5f, camera.height * 0.5f, -100, 100);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();	
	modelStack.PushMatrix();
		modelStack.Translate(0, 0, -10);
		modelStack.Scale(100, 100, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Background"));
	modelStack.PopMatrix();
	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(-10, 10, -10, 10, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();
	modelStack.PushMatrix();
		modelStack.Translate(-3,-8,0);
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), "Press F to Spawn:", Color(1, 1, 1));
		modelStack.Translate(0,-1,0);
		string typeString;
		switch(currentSpawnType) {
			case SceneAI::SPAWN_ATTACKER: {
				typeString = "Attacker";
				break;
			}
			case SceneAI::SPAWN_GUARDIAN: {
				typeString = "Guardian";
				break;
			}
			case SceneAI::SPAWN_SUPPORT: {
				typeString = "Support";
				break;
			}
			case SceneAI::SPAWN_GUNNER: {
				typeString = "Gunner";
				break;
			}
			case SceneAI::SPAWN_TURRET: {
				typeString = "Turret";
				break;
			}
			case SceneAI::SPAWN_BRAWLER: {
				typeString = "Brawler";
				break;
			}
			default:
				break;
		}
		RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), typeString, Color(1, 1, 1));
	modelStack.PopMatrix();

}

void SceneAI::Exit() {
}