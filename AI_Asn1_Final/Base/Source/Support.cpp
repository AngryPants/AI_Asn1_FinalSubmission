#include "Support.h"
#include "MeshBuilder.h"
#include "GL\glew.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "EntityManager.h"
#include "GameManager.h"
#include "Bullet.h"

//include states
#include "Support_Follow.h"
#include "Support_Buff.h"
#include "Support_Heal.h"
#include "Support_Run.h"

//Include Messages
#include "CharacterCreated.h"
#include "ExistingCharacterCheck.h"
#include "ExistingCharacterReply.h"

//Constructor(s) & Destructor
Support::Support() : Character("Support"), info(GetID()) {
	//Initialise Mesh
	if (!MeshBuilder::GetInstance()->HasMesh("Support")) {
		MeshBuilder::GetInstance()->GenerateQuad("Support", Color(1, 0, 0), 1.0f);
		MeshBuilder::GetInstance()->GetMesh("Support")->textureID = LoadTGA("Image//Support.tga");
	}
	mesh = MeshBuilder::GetInstance()->GetMesh("Support");

	if (!MeshBuilder::GetInstance()->HasMesh("HP Bar")) {
		MeshBuilder::GetInstance()->GenerateQuad("HP Bar", Color(1, 0, 0), 1.0f);
	}
	healthMesh = MeshBuilder::GetInstance()->GetMesh("HP Bar");

	if (!MeshBuilder::GetInstance()->HasMesh("Heal Beam"))
	{
		MeshBuilder::GetInstance()->GenerateBeam("Heal Beam", Color(1, 1, 0), 1.f);
	}
	healBeam = MeshBuilder::GetInstance()->GetMesh("Heal Beam");

	if (!MeshBuilder::GetInstance()->HasMesh("Buff Beam"))
	{
		MeshBuilder::GetInstance()->GenerateBeam("Buff Beam", Color(0, 0, 1), 1.f);
	}
	buffBeam = MeshBuilder::GetInstance()->GetMesh("Buff Beam");

	//Initialise Variables	
	//Character Stuff
	inParty = true;
	targetID = -1;
	//Movement
	movementSpeed = 5.0f;

	//Combat
	healPerSec = 12;
	radius = 1.5f;
	f_range = 10.f;
	b_renderBeam = false;

	SetMaxHealth(500);
	SetHealth(GetMaxHealth());

	//Initialise States
	currentState = new Support_Buff(*this);
	states.insert(std::pair<string, State*>(currentState->name, currentState));
	currentState = new Support_Run(*this);
	states.insert(std::pair<string, State*>(currentState->name, currentState));
	currentState = new Support_Heal(*this);
	states.insert(std::pair<string, State*>(currentState->name, currentState));
	currentState = new Support_Follow(*this);
	states.insert(std::pair<string, State*>(currentState->name, currentState));

	//Send creation message.
	CharacterCreated creationMessage(GetID());
	creationMessage.inParty = inParty;
	creationMessage.name = name;
	EntityManager::GetInstance()->BroadcastMessage(&creationMessage);

	ExistingCharacterCheck checkMessage(GetID());
	EntityManager::GetInstance()->BroadcastMessage(&checkMessage);
}

Support::~Support() {
}

//Interface Function(s)
void Support::SetNextState(const string& nextState) {
	map<string, State*>::iterator mapIter = states.find(nextState);
	if (mapIter != states.end()) {
		previousState = currentState;
		currentState = mapIter->second;
	}
}

void Support::Reset() {

}

void Support::Update(double deltaTime) {
	
	if (currentState == nullptr) {
		cout << "Support's currentState is NULL!" << endl;
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
	if (IsAlive() == false)
	{
		this->SetIsDone();
	}
}

void Support::Render() {
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
	string state = currentState->name;
	modelStack.PushMatrix();
	float textScale = 1.5f;
	modelStack.Translate(-textScale * (static_cast<float>(state.length())* 0.5f), -GetRadius() - 1, -3);
	modelStack.Scale(textScale, textScale, 1);
	//strcpy(&state, currentState->name.c_str()[7]);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), state, Color(1, 1, 1));
	modelStack.PopMatrix();

	//Render the Health
	modelStack.PushMatrix();
	modelStack.Translate(0, GetRadius() + 2.0f, 0);
	float healthBarScale = Math::Max((static_cast<float>(health) / static_cast<float>(maxHealth)) * 5.0f, 0.001f);
	modelStack.Scale(healthBarScale, 0.5f, 1);
	RenderHelper::RenderMesh(healthMesh);
	modelStack.PopMatrix();

	Character* target = static_cast<Character*>(EntityManager::GetInstance()->GetEntityByID(targetID));

	if (state == "Heal" && b_renderBeam == true)
	{
		modelStack.PushMatrix();
		//modelStack.Translate(position.x,position.y,position.z);
		Vector3 direction = (target->position - position).Normalize();
		float rotation = Math::RadianToDegree(atan2(direction.y, direction.x));
		modelStack.Rotate(rotation, 0, 0, 1);

		float beamScale = (this->position - target->position).Length();
		modelStack.Scale(beamScale, 0.2f, 1);
		RenderHelper::RenderMesh(healBeam);
		modelStack.PopMatrix();
	}

	if (state == "Buff" && b_renderBeam == true)
	{
		modelStack.PushMatrix();
		//modelStack.Translate(position.x, position.y, position.z);
		Vector3 direction = (target->position - position).Normalize();
		float rotation = Math::RadianToDegree(atan2(direction.y, direction.x));
		modelStack.Rotate(rotation, 0, 0, 1);

		float beamScale = (this->position - target->position).Length();
		modelStack.Scale(beamScale, 0.2f, 1);
		RenderHelper::RenderMesh(buffBeam);
		modelStack.PopMatrix();
	}

	modelStack.PopMatrix();
}

void Support::RenderUI() {
}

//Movement
float Support::GetMovementSpeed() const {
	return movementSpeed;
}

//Messaging & States
void Support::HandleMessage() {
	while (!messageQueue.empty()) {
		bool messageResolved = false;
		if (!messageResolved) {
			const CharacterCreated* creationMesssage = dynamic_cast<const CharacterCreated*>(messageQueue.front());
			if (creationMesssage != nullptr) {
				EntityBase* entityPtr = EntityManager::GetInstance()->GetEntityByID(creationMesssage->senderID);
				if (creationMesssage->inParty != this->inParty) {
					if (entityPtr != nullptr) {
						AddSubject(*entityPtr);
					}
					messageResolved = true;
				}
				else {
					Character* characterPtr = dynamic_cast<Character*>(entityPtr);
					if (characterPtr != nullptr && characterPtr->name == "Attacker" || characterPtr->name == "Guardian") {
						AddSubject(*entityPtr);
					}
				}
			}
		}

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

		if (!messageResolved) {
			const ExistingCharacterReply* checkMessage = dynamic_cast<ExistingCharacterReply*>(messageQueue.front());
			if (checkMessage != nullptr && checkMessage->inParty != InParty()) {
				EntityBase* entityPtr = EntityManager::GetInstance()->GetEntityByID(checkMessage->senderID);
				if (entityPtr != nullptr) {
					AddSubject(*entityPtr);
				}
				messageResolved = true;
			}
		}

		if (!messageResolved) {
			const ExistingCharacterReply* checkMessage = dynamic_cast<ExistingCharacterReply*>(messageQueue.front());
			if (checkMessage != nullptr && checkMessage->inParty == InParty()) {
				EntityBase* entityPtr = EntityManager::GetInstance()->GetEntityByID(checkMessage->senderID);
				if (entityPtr != nullptr) {
					Character* characterPtr = dynamic_cast<Character*>(entityPtr);
					if (characterPtr != nullptr && characterPtr->name == "Attacker" || characterPtr->name == "Guardian") {
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

//Combat
void Support::HealAlly()
{
	if (this->targetID == -1)
	{
		return;
	}
	Character* target = static_cast<Character*>(EntityManager::GetInstance()->GetEntityByID(targetID));
	if (target->InParty() != this->inParty)
	{
		return;
	}
	else
	{
		target->Heal(healPerSec);
	}
}

void Support::Buff()
{
	if (this->targetID == -1)
	{
		return;
	}
	Character* target = static_cast<Character*>(EntityManager::GetInstance()->GetEntityByID(targetID));
	if (target->InParty() != this->inParty)
	{
		return;
	}
	else
	{
		target->Buff();
	}
}

void Support::SelfHeal()
{
	this->Heal(this->healPerSec);
}