#ifndef TEST_CHARACTER_H
#define TEST_CHARACTER_H

#include "Character.h"
#include "MeshBuilder.h"
#include "CharacterInfo.h"
#include "CharacterCreated.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include <iostream>
#include <string>

using namespace std;

class TestCharacter : public Character {

public:
	CharacterInfo info;

	TestCharacter(const string& name = "Test Character", bool inParty = false) : Character(name), info(GetID()) {
		this->inParty = inParty;

		if (MeshBuilder::GetInstance()->HasMesh("Test Character") == false) {
			MeshBuilder::GetInstance()->GenerateQuad("Test Character", Color(0, 1, 1), 1.0f);
		}
		mesh = MeshBuilder::GetInstance()->GetMesh("Test Character");

		currentState = nullptr;

		//Send creation message.
		CharacterCreated creationMessage(GetID());
		creationMessage.inParty = inParty;
		creationMessage.name = name;
		EntityManager::GetInstance()->BroadcastMessage(&creationMessage);
	}
	virtual ~TestCharacter() {}

	virtual void Reset() {}
	virtual void Update(double deltaTime) {
		if (IsAlive() == false) {
			return;
		}

		HandleMessage();
		
		info.name = name;
		info.position = GetPosition();
		info.health = GetHealth();
		info.maxHealth = GetMaxHealth();
		info.isAlive = IsAlive();
		if (currentState != NULL) {
			info.currentState = currentState->name;
		}		
		info.inParty = inParty;
		EntityManager::GetInstance()->BroadcastMessage(&info);
	}

	virtual void Render() {
		if (IsAlive() == false) {
			return;
		}
		//cout << "Test Character Position: " << "[" << GetPosition().x << "," << GetPosition().y << "," << GetPosition().z << "]" << endl;
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
			modelStack.Translate(GetPosition().x,GetPosition().y,GetPosition().z);
			modelStack.Scale(5, 5, 1);
			RenderHelper::RenderMesh(mesh);
		modelStack.PopMatrix();
	}
	virtual void RenderUI() {}
	virtual void SetNextState(const string& stateName) {}

};

#endif