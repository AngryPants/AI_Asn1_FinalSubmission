#include "Turret.h"
#include "MeshBuilder.h"
#include "GL\glew.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "EntityManager.h"
#include "GameManager.h"

#include "Bullet.h"

#include "Turret_Attack.h"
#include "Turret_Reload.h"
#include "Turret_Idle.h"

//Include Messsages
#include "CharacterInfo.h"
#include "ExistingCharacterCheck.h"
#include "ExistingCharacterReply.h"

Turret::Turret() : Character("Turret")
, fireRate(2.f)
, reloadTime(15.0f)
, detectionRange(15.f)
, attRange(20.f)
{
	//Initialise Mesh
	if (!MeshBuilder::GetInstance()->HasMesh("Turret")) {
		MeshBuilder::GetInstance()->GenerateQuad("Turret", Color(0, 0, 1), 1.0f);
		MeshBuilder::GetInstance()->GetMesh("Turret")->textureID = LoadTGA("Image//Turret.tga");
	}
	mesh = MeshBuilder::GetInstance()->GetMesh("Turret");

	if(!MeshBuilder::GetInstance()->HasMesh("HP Bar")) {
		MeshBuilder::GetInstance()->GenerateQuad("HP Bar",Color(1,0,0),1.0f);
	}
	healthMesh = MeshBuilder::GetInstance()->GetMesh("HP Bar");

	if(!MeshBuilder::GetInstance()->HasMesh("Ammo Bar")) {
		MeshBuilder::GetInstance()->GenerateQuad("Ammo Bar",Color(0,0,1),1.0f);
	}
	ammoMesh = MeshBuilder::GetInstance()->GetMesh("Ammo Bar");

	//Initialise Variables	
	//Character Stuff
	inParty = false;

	//Combat
	clipSize = 15;
	ammo = clipSize;
	dmg = 5;
	SetMaxHealth(50);
	SetHealth(GetMaxHealth());

	radius = 1.0f;

	//Initialise States
	currentState = new Turret_Attack(*this);
	states.insert(std::pair<string, State*>(currentState->name, currentState));

	currentState = new Turret_Reload(*this);
	states.insert(std::pair<string, State*>(currentState->name, currentState));

	currentState = new Turret_Idle(*this);
	states.insert(std::pair<string, State*>(currentState->name, currentState));
}

Turret::~Turret()
{

}

void Turret::SetNextState(const string &nextState)
{
	map<string, State*>::iterator mapIter = states.find(nextState);
	if (mapIter != states.end()) {
		//info.previousState = currentState->name;
		currentState = mapIter->second;
		//info.currentState = currentState->name;
	}
}

void Turret::Reset()
{
	ammo = clipSize;
}

void Turret::Update(double dt)
{
	if(IsAlive() == false) {
		return;
	}
	if (currentState == nullptr)
	{
		cout << "Turret has NO state" << endl;
		return;
	}

	HandleMessage();
	currentState->Update(dt);

	CharacterInfo info(GetID());
	info.name = "Turret";
	info.inParty = InParty();
	info.position = GetPosition();
	info.health = GetHealth();
	info.maxHealth = GetMaxHealth();
	info.isAlive = IsAlive();
	EntityManager::GetInstance()->BroadcastMessage(&info);
}

void Turret::Render()
{
	if(IsAlive() == false) {
		return;
	}
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(GetPosition().x,GetPosition().y,GetPosition().z);

	//Render the main body.
	modelStack.PushMatrix();
	modelStack.Rotate(GetRotation(),0,0,1);
	modelStack.Scale(radius * 2.0f,radius * 2.0f,1.0f);
	RenderHelper::RenderMesh(mesh);
	modelStack.PopMatrix();

	//Render the State
	modelStack.PushMatrix();
		string stateString;
		if(currentState->name == "Turret_Attack") {
			stateString = "Attack";
		} else if(currentState->name == "Turret_Idle") {
			stateString = "Idle";
		} else if(currentState->name == "Turret_Reload") {
			stateString = "Reload";
		}
		float textScale = 1.5f;
		modelStack.Translate(-textScale * (static_cast<float>(stateString.length()) * 0.5f),-GetRadius() - 1,-3);
		modelStack.Scale(textScale,textScale,1);		
		RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"),stateString,Color(1,1,1));
	modelStack.PopMatrix();

	//Render the Health
	modelStack.PushMatrix();
		modelStack.Translate(0,GetRadius() + 2.0f,0);
		float healthBarScale = Math::Max((static_cast<float>(health) / static_cast<float>(maxHealth)) * 5.0f,0.001f);
		modelStack.Scale(healthBarScale,0.5f,1);
		RenderHelper::RenderMesh(healthMesh);
	modelStack.PopMatrix();

	//Render the Ammo		
	float ammoBarSize = 1.0f / 3.0f;
	for(unsigned int i = 0; i < ammo; ++i) {
		modelStack.PushMatrix();
		modelStack.Translate(-static_cast<float>(clipSize - 1) * ammoBarSize * 0.5f + (i * ammoBarSize),GetRadius() + 3.0f,0);
		modelStack.Scale(ammoBarSize * 0.8f,0.5f,1);
		RenderHelper::RenderMesh(ammoMesh);
		modelStack.PopMatrix();
	}


	modelStack.PopMatrix();
}

void Turret::RenderUI()
{
}

int Turret::GetAmmo()
{
	return this->ammo;
}

void Turret::Reload()
{
	ammo = clipSize;
}

void Turret::Shoot(Vector3 &direction)
{
	Bullet* bullet = GameManager::GetInstance()->GetBullet();
	bullet->inParty = this->inParty;
	bullet->SetActive();
	bullet->damage = dmg;
	bullet->direction = direction;
	bullet->position = this->GetPosition();
	bullet->mesh = MeshBuilder::GetInstance()->GetMesh("Enemy_Bullet");
	--this->ammo;
}

void Turret::InitPos(Vector3 pos)
{
	this->SetPosition(pos);
	this->initialPos = pos;
}

void Turret::HandleMessage() {
	while(!messageQueue.empty()) {
		bool messageResolved = false;
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

		currentState->ReceiveMessage(messageQueue.front());
		delete messageQueue.front();
		messageQueue.pop();
	}
}