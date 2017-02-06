#include "Brawler.h"
#include "MeshBuilder.h"
#include "GL\glew.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "EntityManager.h"
#include "GameManager.h"

//include states
#include "Brawler_Patrol.h"
#include "Brawler_Attack.h"
#include "Brawler_Run.h"

//Include Messages
#include "CharacterCreated.h"
#include "ExistingCharacterCheck.h"
#include "ExistingCharacterReply.h"

Brawler::Brawler() : Character("Brawler")
, attackRate(2.f)
, detectionRange(3.f)
, attRange(5.f)
, info(GetID())
{
	if (!MeshBuilder::GetInstance()->HasMesh("Brawler")) 
	{
		MeshBuilder::GetInstance()->GenerateQuad("Brawler", Color(1, 1, 1), 1.0f);
		MeshBuilder::GetInstance()->GetMesh("Brawler")->textureID = LoadTGA("Image//Brawler.tga");
	}
	mesh = MeshBuilder::GetInstance()->GetMesh("Brawler");

	if (!MeshBuilder::GetInstance()->HasMesh("HP Bar")) 
	{
		MeshBuilder::GetInstance()->GenerateQuad("HP Bar", Color(1, 0, 0), 1.0f);
	}
	healthMesh = MeshBuilder::GetInstance()->GetMesh("HP Bar");

	inParty = false;

	radius = 1.5f;
	moveSpeed = 6.0f;
	damage = 5;
	SetMaxHealth(40);
	SetHealth(GetMaxHealth());
	
	targetID = -1;

	currentState = new Brawler_Run(*this);
	states.insert(std::pair<string, State*>(currentState->name, currentState));

	currentState = new Brawler_Patrol(*this);
	states.insert(std::pair<string, State*>(currentState->name, currentState));

	currentState = new Brawler_Attack(*this);
	states.insert(std::pair<string, State*>(currentState->name, currentState));

	CharacterCreated creationMessage(GetID());
	creationMessage.inParty = inParty;
	creationMessage.name = name;
	EntityManager::GetInstance()->BroadcastMessage(&creationMessage);
}

Brawler::~Brawler()
{
}

void Brawler::SetNextState(const string& nextState)
{
	map<string, State*>::iterator mapIter = states.find(nextState);
	if (mapIter != states.end()) 
	{
		currentState = mapIter->second;
	}
}

void Brawler::Reset()
{

}

void Brawler::Update(double dt)
{
	if (IsAlive() == false)
	{
		return;
	}

	if (currentState == nullptr) 
	{
		cout << "Brawler's currentState is NULL!" << endl;
		return;
	}

	HandleMessage();
	currentState->Update(dt);

	info.name = "Brawler";
	info.inParty = InParty();
	info.position = GetPosition();
	info.health = GetHealth();
	info.maxHealth = GetMaxHealth();
	info.isAlive = IsAlive();
	SendMessageToObservers(&info);
}

void Brawler::Render()
{
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
	modelStack.Translate(-textScale * (static_cast<float>(currentState->name.length()) * 0.5f), -GetRadius() - 1, -3);
	modelStack.Scale(textScale, textScale, 1);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), currentState->name, Color(1, 1, 1));
	modelStack.PopMatrix();

	//Render the Health
	modelStack.PushMatrix();
	modelStack.Translate(0, GetRadius() + 2.0f, 0);
	float healthBarScale = Math::Max((static_cast<float>(health) / static_cast<float>(maxHealth)) * 5.0f, 0.001f);
	modelStack.Scale(healthBarScale, 0.5f, 1);
	RenderHelper::RenderMesh(healthMesh);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void Brawler::RenderUI()
{

}

void Brawler::Attack(Vector3& direction)
{
	Character* target = static_cast<Character*>(EntityManager::GetInstance()->GetEntityByID(targetID));
	target->TakeDamage(1);
}

float Brawler::GetRadius() const
{
	if (health > 0)
	{
		return radius;
	}
}

void Brawler::InitPos(Vector3 pos)
{
	this->SetPosition(pos);
	this->initialPos = pos;
}

void Brawler::HandleMessage()
{
	while (!messageQueue.empty()) {
		bool messageResolved = false;
		if (!messageResolved) {
			const ExistingCharacterCheck* checkMessage = dynamic_cast<ExistingCharacterCheck*>(messageQueue.front());
			if (checkMessage != nullptr) {
				EntityBase* entityPtr = EntityManager::GetInstance()->GetEntityByID(checkMessage->senderID);
				if (entityPtr != nullptr) {
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