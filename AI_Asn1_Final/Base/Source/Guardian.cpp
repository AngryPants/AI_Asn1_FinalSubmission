#include "Guardian.h"
#include "EntityManager.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "GL\glew.h"
#include "LoadTGA.h"

//Include States
#include "Guardian_Run.h"
#include "Guardian_Follow.h"
#include "Guardian_Defend.h"

//Include Messsages
#include "CharacterCreated.h"
#include "ExistingCharacterCheck.h"
#include "ExistingCharacterReply.h"

//Constructor(s) & Destructor
Guardian::Guardian() : Character("Guardian"), info(GetID()) {
	inParty = true;

	//Initialise Mesh
	if (!MeshBuilder::GetInstance()->HasMesh("Guardian")) {
		MeshBuilder::GetInstance()->GenerateQuad("Guardian", Color(1,0,0),1.0f);
		MeshBuilder::GetInstance()->GetMesh("Guardian")->textureID = LoadTGA("Image//Guardian.tga");
	}
	mesh = MeshBuilder::GetInstance()->GetMesh("Guardian");
	
	if (!MeshBuilder::GetInstance()->HasMesh("Shield Mesh")) {
		MeshBuilder::GetInstance()->GenerateQuad("Shield Mesh", Color(1, 1, 1));
		MeshBuilder::GetInstance()->GetMesh("Shield Mesh")->textureID = LoadTGA("Image//Shield.tga");
	}
	shieldMesh = MeshBuilder::GetInstance()->GetMesh("Shield Mesh");

	if(!MeshBuilder::GetInstance()->HasMesh("HP Bar")) {
		MeshBuilder::GetInstance()->GenerateQuad("HP Bar",Color(1,0,0),1.0f);
	}
	healthMesh = MeshBuilder::GetInstance()->GetMesh("HP Bar");

	if(!MeshBuilder::GetInstance()->HasMesh("Red Circle")) {
		MeshBuilder::GetInstance()->GenerateRing("Red Circle",Color(1,0,0),32,1.0f,0.999f);
	}
	redCircleMesh = MeshBuilder::GetInstance()->GetMesh("Red Circle");

	if(!MeshBuilder::GetInstance()->HasMesh("Blue Circle")) {
		MeshBuilder::GetInstance()->GenerateRing("Blue Circle",Color(0,0,1),32,1.0f,0.999f);
	}
	blueCircleMesh = MeshBuilder::GetInstance()->GetMesh("Blue Circle");

	//Shield & Health
	maxShield = 30;
	shield = maxShield;
	radius = 1.5f;
	shieldRadius = static_cast<float>(shield) / static_cast<float>(maxShield) + radius;
	shieldRechargeTimer = 0.0;
	buffTimer = 0.0;
	priorityRange = 12.0f;

	//Movement
	movementSpeed = 7.0f;

	//Initialise States
	currentState = new Guardian_Defend(*this);
	states.insert(std::pair<string,State*>(currentState->name,currentState));
	currentState = new Guardian_Run(*this);
	states.insert(std::pair<string,State*>(currentState->name,currentState));
	currentState = new Guardian_Follow(*this);
	states.insert(std::pair<string,State*>(currentState->name,currentState));

	//Send creation message.
	CharacterCreated creationMessage(GetID());
	creationMessage.inParty = inParty;
	creationMessage.name = name;
	EntityManager::GetInstance()->BroadcastMessage(&creationMessage);
}

Guardian::~Guardian() {
}

//Health & Shield
void Guardian::SetShield(const int shield) {
	if(shield < 0) {
		this->shield = 0;
	} else {
		this->shield = shield;
	}
}

int Guardian::GetShield() const {
	return shield;
}

int Guardian::GetMaxShield() const {
	return maxShield;
}

void Guardian::Buff() {
	if (buffTimer <= 0.0) {
		shield += 5;
		buffTimer = 1.0;
	}
}

void Guardian::Heal(const int health) {
	this->health = Math::Min(maxHealth, this->health + health);
}

void Guardian::TakeDamage(const int damage) {
	if (this->shield >= damage) {
		this->shield -= damage;
	} else {
		this->health -= (damage - shield);
		shield = 0;
	}
}

float Guardian::GetRadius() const {
	if (shield > 0) {
		return shieldRadius;
	} else {
		return radius;
	}
}

//Movement
float Guardian::GetMovementSpeed() const {
	return movementSpeed;
}

//Others
void Guardian::Update(double deltaTime) {
	if (IsAlive() == false) {
		return;
	}

	if (currentState == nullptr) {
		cout << "Guardian's currentState is NULL!" << endl;
		return;
	}

	buffTimer -= deltaTime;
	shieldRadius = static_cast<float>(shield) / static_cast<float>(maxShield) + radius;
	shieldRechargeTimer -= deltaTime;
	if (shieldRechargeTimer <= 0.0) {
		shield = Math::Min(maxShield, shield + 2);
		shieldRechargeTimer = 0.3;
	}

	HandleMessage();
	currentState->Update(deltaTime);

	info.name = name;
	info.position = GetPosition();
	info.health = GetHealth();
	info.maxHealth = GetMaxHealth();
	info.isAlive = IsAlive();
	info.inParty = inParty;
	info.currentState = currentState->name;
	EntityManager::GetInstance()->BroadcastMessage(&info);
}

void Guardian::Render() {
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(GetPosition().x,GetPosition().y,GetPosition().z);

	//Render the main body.
	modelStack.PushMatrix();
		modelStack.Translate(0, 0, 5);
		modelStack.Rotate(GetRotation(),0,0,1);
		modelStack.Scale(radius * 2.0f,radius * 2.0f,1.0f);
		RenderHelper::RenderMesh(mesh);
	modelStack.PopMatrix();

	//Render the shield
	modelStack.PushMatrix();
		modelStack.Translate(0, 0, 6);
		modelStack.Scale(shieldRadius * 2.0f,shieldRadius * 2.0f,1);
		RenderHelper::RenderMesh(shieldMesh);
	modelStack.PopMatrix();

	//Render the State
	modelStack.PushMatrix();
		float textScale = 1.5f;
		modelStack.Translate(-textScale * (static_cast<float>(currentState->name.length()) * 0.5f),-GetRadius() - 1,-3);
		modelStack.Scale(textScale,textScale,1);
		RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"),currentState->name,Color(1,1,1));
	modelStack.PopMatrix();

	//Render the Health
	modelStack.PushMatrix();
		modelStack.Translate(0,GetRadius() + 2.0f,0);
		float healthBarScale = Math::Max((static_cast<float>(health) / static_cast<float>(maxHealth)) * 5.0f,0.001f);
		modelStack.Scale(healthBarScale,0.5f,1);
		RenderHelper::RenderMesh(healthMesh);
	modelStack.PopMatrix();
		
	//Render the Priority Range
	modelStack.PushMatrix();
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(priorityRange, 1,priorityRange);
		RenderHelper::RenderMesh(redCircleMesh);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void Guardian::RenderUI() {
}

void Guardian::SetNextState(const string& nextState) {
	map<string,State*>::iterator mapIter = states.find(nextState);
	if(mapIter != states.end()) {
		currentState = mapIter->second;
	}
}

void Guardian::Reset() {
}

void Guardian::HandleMessage() {
	while(!messageQueue.empty()) {
		bool messageResolved = false;

		if (!messageResolved) {
			const CharacterCreated* creationMessage = dynamic_cast<CharacterCreated*>(messageQueue.front());
			if (creationMessage != nullptr) {
				if (creationMessage->inParty == inParty && creationMessage->name == "Support") {
					AddSubject(*EntityManager::GetInstance()->GetEntityByID(creationMessage->senderID));
				}
			}
		}

		if(!messageResolved) {
			const ExistingCharacterCheck* checkMessage = dynamic_cast<ExistingCharacterCheck*>(messageQueue.front());
			if(checkMessage != nullptr) {
				EntityBase* entityPtr = EntityManager::GetInstance()->GetEntityByID(checkMessage->senderID);
				if(entityPtr != nullptr) {
					ExistingCharacterReply reply(GetID());
					reply.inParty = InParty();
					entityPtr->ReceiveMessage(&reply);
				}
				messageResolved = true;
			}
		}

		if(!messageResolved) {
			const ExistingCharacterReply* replyMessage = dynamic_cast<ExistingCharacterReply*>(messageQueue.front());
			if(replyMessage != nullptr) {
				EntityBase* entityPtr = EntityManager::GetInstance()->GetEntityByID(replyMessage->senderID);
				if(entityPtr != nullptr) {
					Character* characterPtr = dynamic_cast<Character*>(entityPtr);
					if (characterPtr != nullptr && characterPtr->name == "Support" && replyMessage->inParty == inParty) {
						AddSubject(*entityPtr);
					}					
				}
				messageResolved = true;
			}
		}

		currentState->ReceiveMessage(messageQueue.front());
		delete messageQueue.front();
		messageQueue.pop();
	}
}