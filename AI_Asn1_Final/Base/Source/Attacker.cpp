#include "Attacker.h"
#include "MeshBuilder.h"
#include "GL\glew.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "EntityManager.h"
#include "GameManager.h"
#include "Bullet.h"

//Include States
#include "Attacker_Attack.h"
#include "Attacker_Reload.h"
#include "Attacker_Run.h"
#include "Attacker_Move.h"

//Include Messages
#include "CharacterCreated.h"
#include "ExistingCharacterCheck.h"
#include "ExistingCharacterReply.h"

//Constructor(s) & Destructor
Attacker::Attacker() : Character("Attacker"), info(GetID()) {
	//Initialise Mesh
	if (!MeshBuilder::GetInstance()->HasMesh("Attacker")) {
		MeshBuilder::GetInstance()->GenerateQuad("Attacker", Color(1, 0, 0), 1.0f);
		MeshBuilder::GetInstance()->GetMesh("Attacker")->textureID = LoadTGA("Image//Attacker.tga");
	}
	mesh = MeshBuilder::GetInstance()->GetMesh("Attacker");	
	
	if (!MeshBuilder::GetInstance()->HasMesh("HP Bar")) {
		MeshBuilder::GetInstance()->GenerateQuad("HP Bar", Color(1, 0, 0), 1.0f);
	}
	healthMesh = MeshBuilder::GetInstance()->GetMesh("HP Bar");
 
	if(!MeshBuilder::GetInstance()->HasMesh("Ammo Bar")) {
		MeshBuilder::GetInstance()->GenerateQuad("Ammo Bar",Color(0,0,1),1.0f);
	}
	ammoMesh = MeshBuilder::GetInstance()->GetMesh("Ammo Bar");

	if (!MeshBuilder::GetInstance()->HasMesh("Red Circle")) {
		MeshBuilder::GetInstance()->GenerateRing("Red Circle", Color(1, 0, 0), 32, 1.0f, 0.999f);
	}
	redCircleMesh = MeshBuilder::GetInstance()->GetMesh("Red Circle");

	if(!MeshBuilder::GetInstance()->HasMesh("Blue Circle")) {
		MeshBuilder::GetInstance()->GenerateRing("Blue Circle",Color(0,0,1),32,1.0f,0.999f);
	}
	blueCircleMesh = MeshBuilder::GetInstance()->GetMesh("Blue Circle");

	//Initialise Variables	
	//Character Stuff
	inParty = true;

	//Movement
	movementSpeed = 5.0f;
		
	//Combat
	maxBullets = 10;
	numBullets = maxBullets;	
	damage = 10;
	minRange = 12.0f;
	maxRange = minRange * 1.5f;
	fireRate = 2.0;
	timeToFire = 0.0;
	radius = 1.5f;

	SetMaxHealth(500);
	SetHealth(GetMaxHealth());

	//Initialise States
	currentState = new Attacker_Attack(*this);
	states.insert(std::pair<string, State*>(currentState->name, currentState));
	currentState = new Attacker_Run(*this);
	states.insert(std::pair<string, State*>(currentState->name, currentState));
	currentState = new Attacker_Reload(*this);
	states.insert(std::pair<string, State*>(currentState->name, currentState));
	currentState = new Attacker_Move(*this);
	states.insert(std::pair<string, State*>(currentState->name, currentState));

	//Send creation message.
	CharacterCreated creationMessage(GetID());
	creationMessage.inParty = inParty;
	creationMessage.name = name;
	EntityManager::GetInstance()->BroadcastMessage(&creationMessage);

	ExistingCharacterCheck checkMessage(GetID());
	EntityManager::GetInstance()->BroadcastMessage(&checkMessage);
}

Attacker::~Attacker() {
}

//Interface Function(s)
void Attacker::SetNextState(const string& nextState) {
	map<string, State*>::iterator mapIter = states.find(nextState);
	if (mapIter != states.end()) {
		previousState = currentState;
		currentState = mapIter->second;
	}
}

void Attacker::Reset() {
	numBullets = maxBullets;
	timeToFire = 0.0f;
}

void Attacker::Update(double deltaTime) {
	if (timeToFire > 0.0) {
		timeToFire = Math::Max(0.0, timeToFire - deltaTime);
	}
	if (currentState == nullptr) {
		cout << "Attacker's currentState is NULL!" << endl;
		return;
	}

	HandleMessage();
	currentState->Update(deltaTime);

	info.name = name;
	info.position = GetPosition();
	info.health = GetHealth();
	info.maxHealth = GetMaxHealth();
	info.isAlive = IsAlive();
	info.currentState = currentState->name;
	info.inParty = inParty;
	EntityManager::GetInstance()->BroadcastMessage(&info);
	fireRate = 2.0f;
}

void Attacker::Render() {
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
		modelStack.Translate(GetPosition().x, GetPosition().y, GetPosition().z);

		//Render the main body.
		modelStack.PushMatrix();
			modelStack.Rotate(GetRotation(), 0, 0, 1);
			modelStack.Scale(radius * 2.0f, radius * 2.0f, 1.0f);
			RenderHelper::RenderMesh(mesh);
		modelStack.PopMatrix();

		//Render the State
		modelStack.PushMatrix();
			float textScale = 1.5f;
			modelStack.Translate(-textScale * (static_cast<float>(currentState->name.length()) * 0.5f), -GetRadius() - 1.0f, -3);
			modelStack.Scale(textScale, textScale, 1);
			RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), currentState->name, Color(1, 1, 1));
			//Current Attack State
			if(currentState->name == "Attack") {
				modelStack.Translate(0, -1.0f,0);
				Attacker_Attack* attackStatePtr = static_cast<Attacker_Attack*>(currentState);
				string attackMode;
				switch (attackStatePtr->GetAttackMode()) {
					case Attacker_Attack::ATTACK_CLOSEST:{
						attackMode = "Closest";
						break;
					}
					case Attacker_Attack::ATTACK_FURTHEST:{
						attackMode = "Furthest";
						break;
					}
					case Attacker_Attack::ATTACK_LOWEST_HP:{
						attackMode = "Lowest HP";
						break;
					}
					case Attacker_Attack::ATTACK_HIGHEST_HP:{
						attackMode = "Highest HP";
						break;
					}
				}
				RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), attackMode, Color(1, 1, 1));
			}
		modelStack.PopMatrix();		

		//Render the Health
		modelStack.PushMatrix();
			modelStack.Translate(0, GetRadius() + 2.0f, 0);
			float healthBarScale = Math::Max((static_cast<float>(health) / static_cast<float>(maxHealth)) * 5.0f, 0.001f);
			modelStack.Scale(healthBarScale, 0.5f, 1);
			RenderHelper::RenderMesh(healthMesh);
		modelStack.PopMatrix();

		//Render the Ammo		
		float ammoBarSize = 0.5f;
		for (unsigned int i = 0; i < numBullets; ++i) {
			modelStack.PushMatrix();
				modelStack.Translate(-static_cast<float>(maxBullets - 1) * ammoBarSize * 0.5f + (i * ammoBarSize), GetRadius() + 3.0f, 0);
				modelStack.Scale(ammoBarSize * 0.8f, 0.5f, 1);
				RenderHelper::RenderMesh(ammoMesh);
			modelStack.PopMatrix();
		}
		
		//Render the detection radius
		modelStack.PushMatrix();
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(minRange, 1, minRange);
			RenderHelper::RenderMesh(redCircleMesh);
		modelStack.PopMatrix();

		//Render the combat radius
		modelStack.PushMatrix();
			modelStack.Rotate(90,1,0,0);
			modelStack.Scale(maxRange,1,maxRange);
			RenderHelper::RenderMesh(blueCircleMesh);
		modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void Attacker::RenderUI() {
}

//Movement
float Attacker::GetMovementSpeed() const {
	return movementSpeed;
}

//Combat
int Attacker::GetNumBullets() const {
	return numBullets;
}

int Attacker::GetMaxBullets() const {
	return maxBullets;
}

void Attacker::AddBullets(const int numBullets) {
	this->numBullets += numBullets;
	if (this->numBullets > this->maxBullets) {
		this->numBullets = this->maxBullets;
	} else if (this->numBullets < 0) {
		this->numBullets = 0;
	}
}

void Attacker::Shoot(float bulletSpread) {

	if (bulletSpread > 20.0f) {
		bulletSpread = 20.0f;
	} else if (bulletSpread < -20.0f) {
		bulletSpread = -20.0f;
	}

	if (timeToFire <= 0.0 && numBullets > 0) {
		timeToFire = 1.0 / fireRate;
		--numBullets;
		Bullet* bullet = GameManager::GetInstance()->GetBullet();
		bullet->SetActive();
		bullet->damage = damage;
		bullet->inParty = this->inParty;
		bullet->position = this->GetPosition();
		bullet->mesh = MeshBuilder::GetInstance()->GetMesh("Party_Bullet");

		Vector3 direction(1, 0, 0);
		Mtx44 mat;
		mat.SetToRotation(GetRotation() + Math::RandFloatMinMax(-bulletSpread, bulletSpread), 0, 0, 1);
		bullet->direction = mat * direction;
	}
}

float Attacker::GetMinCombatRange() const {
	return minRange;
}

float Attacker::GetMaxCombatRange() const {
	return maxRange;
}

void Attacker::Buff() {
	fireRate = 10.0f;
}

//Messaging & States
void Attacker::HandleMessage() {
	while(!messageQueue.empty()) {
		bool messageResolved = false;
		if (!messageResolved) {
			const CharacterCreated* creationMesssage = dynamic_cast<const CharacterCreated*>(messageQueue.front());
			if (creationMesssage != nullptr && creationMesssage->inParty != this->inParty) {
				EntityBase* entityPtr = EntityManager::GetInstance()->GetEntityByID(creationMesssage->senderID);
				if (entityPtr != nullptr) {
					AddSubject(*entityPtr);
				}
				messageResolved = true;
			}
		}

		if (!messageResolved) {
			const ExistingCharacterCheck* checkMessage = dynamic_cast<ExistingCharacterCheck*>(messageQueue.front());
			if (checkMessage != nullptr) {
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
			const ExistingCharacterReply* checkMessage = dynamic_cast<ExistingCharacterReply*>(messageQueue.front());
			if(checkMessage != nullptr && checkMessage->inParty != InParty()) {
				EntityBase* entityPtr = EntityManager::GetInstance()->GetEntityByID(checkMessage->senderID);
				if(entityPtr != nullptr) {
					AddSubject(*entityPtr);
				}
				messageResolved = true;
			}
		}

		currentState->ReceiveMessage(messageQueue.front());
		delete messageQueue.front();
		messageQueue.pop();
	}
}