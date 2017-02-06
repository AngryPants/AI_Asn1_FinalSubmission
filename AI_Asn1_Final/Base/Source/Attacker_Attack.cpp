#include "Attacker_Attack.h"
#include "Attacker.h"

#include "CharacterInfo.h"

//Constructor(s) & Destructor
Attacker_Attack::Attacker_Attack(Attacker& owner) : Attacker_State("Attack", owner) {
	hasSupport = true;
	currentMode = ATTACK_CLOSEST;
	currentTargetID = -1;
	
	for (unsigned int i = 0; i < NUM_ATTACK_MODE; ++i) {
		modeChance[i] = 100.0f / static_cast<float>(NUM_ATTACK_MODE);
	}
}

Attacker_Attack::~Attacker_Attack() {
}

//Private Function(s)
bool Attacker_Attack::Think(double deltaTime) {
	if (hasSupport && resetRandom && static_cast<float>(GetOwner().GetHealth()) / static_cast<float>(GetOwner().GetMaxHealth()) <= 0.3f) {
		int randomInt = Math::RandInt() % 10;
		resetRandom = false;
		//if (randomInt > 3) {
			GetOwner().SetNextState("Run");
			return false;
		//}
	}
	if (enemyInfos.empty()) {
		GetOwner().SetNextState("Move");
		resetRandom = true;
		return false;
	}		
	if (GetOwner().GetNumBullets() == 0) {
		GetOwner().SetNextState("Reload");
		resetRandom = true;
		return false;
	}

	return true;
}

void Attacker_Attack::Act(double deltaTime) {
	if (TargetCurrent() == false) {
		float chance = Math::RandFloatMinMax(0.1f, 100.0f);
		for (unsigned int i = 0; i < NUM_ATTACK_MODE; ++i) {
			float totalModeChance = 0.0f;
			for (unsigned int k = 0; k <= i; ++k) {
				totalModeChance += modeChance[k];
			}
			if (chance < totalModeChance) {
				currentMode = static_cast<ATTACK_MODE>(i);
				float chanceGain = (modeChance[i] * 0.5f) / NUM_ATTACK_MODE;
				modeChance[i] *= 0.5f;
				for (unsigned int j = 0; j < NUM_ATTACK_MODE; ++j) {
					if (j != i) {
						modeChance[j] += chanceGain;
					}
					/*
					string modeString;
					switch (static_cast<ATTACK_MODE>(j)) {
						case ATTACK_CLOSEST:
							modeString = "Attack Closest";
							break;
						case ATTACK_FURTHEST:
							modeString = "Attack Furthest";
							break;
						case ATTACK_HIGHEST_HP:
							modeString = "Attack Highest HP";
							break;
						case ATTACK_LOWEST_HP:
							modeString = "Attack Lowest HP";
							break;
					}
					cout << modeString << " Mode Chance: " << modeChance[j] << endl;
					*/
				}
				break;
			} else {
				chance += modeChance[i];
			}
		}

		if (GetOwner().CanShoot()) {
			switch (currentMode) {
				case ATTACK_CLOSEST: {
					TargetClosest();
					break;
				}
				case ATTACK_FURTHEST: {
					TargetFurthest();
					break;
				}
				case ATTACK_LOWEST_HP: {
					TargetLowestHP();
					break;
				}
				case ATTACK_HIGHEST_HP: {
					TargetHighestHP();
					break;
				}
			}
		}
	}

	Vector3 direction = targetPosition - GetOwner().GetPosition();
	float distance = direction.Length();
	if (distance > Math::EPSILON) {
		direction.Normalize();
		GetOwner().SetRotation(Math::RadianToDegree(atan2(direction.y, direction.x)));
	}
	float optimalRange = 8.0f;
	if (distance > optimalRange) {
		GetOwner().Translate(direction * deltaTime * GetOwner().GetMovementSpeed() * 0.2f);
	}
	GetOwner().Shoot((distance / optimalRange) * 8.0f);
}

bool Attacker_Attack::TargetCurrent() {
	if(enemyInfos.empty()) {
		return false;
	}

	for (unsigned int i = 0; i < enemyInfos.size(); ++i) {
		if (enemyInfos[i].senderID == currentTargetID) {
			targetPosition = enemyInfos[i].position;
			return true;
		}
	}

	return false;
}

void Attacker_Attack::TargetClosest() {
	if (enemyInfos.empty()) {
		return;
	}

	targetPosition = enemyInfos[0].position;
	float closestDistanceSquared = (targetPosition - GetOwner().GetPosition()).LengthSquared();
	currentTargetID = enemyInfos[0].senderID;
	for (unsigned int i = 1; i < enemyInfos.size(); ++i) {
		float distanceSquared = (enemyInfos[i].position - GetOwner().GetPosition()).LengthSquared();
		if (distanceSquared < closestDistanceSquared) {
			closestDistanceSquared = distanceSquared;
			targetPosition = enemyInfos[i].position;
			currentTargetID = enemyInfos[i].senderID;
		}
	}
}

void Attacker_Attack::TargetFurthest() {
	if(enemyInfos.empty()) {
		return;
	}

	targetPosition = enemyInfos[0].position;
	float closestDistanceSquared = (targetPosition - GetOwner().GetPosition()).LengthSquared();
	currentTargetID = enemyInfos[0].senderID;
	for(unsigned int i = 1; i < enemyInfos.size(); ++i) {
		float distanceSquared = (enemyInfos[i].position - GetOwner().GetPosition()).LengthSquared();
		if(distanceSquared > closestDistanceSquared) {
			closestDistanceSquared = distanceSquared;
			targetPosition = enemyInfos[i].position;
			currentTargetID = enemyInfos[i].senderID;
		}
	}
}

//Work on this one.
void Attacker_Attack::TargetLowestHP() {
	if(enemyInfos.empty()) {
		return;
	}	

	targetPosition = enemyInfos[0].position;
	float lowestHP = static_cast<float>(enemyInfos[0].health) / static_cast<float>(enemyInfos[0].maxHealth);
	currentTargetID = enemyInfos[0].senderID;
	for (unsigned int i = 1; i < enemyInfos.size(); ++i) {
		float hp = static_cast<float>(enemyInfos[i].health) / static_cast<float>(enemyInfos[i].maxHealth);
		if (hp < lowestHP) {
			lowestHP = hp;
			this->targetPosition = enemyInfos[i].position;
			currentTargetID = enemyInfos[i].senderID;
		}
	}
}

//And this
void Attacker_Attack::TargetHighestHP() {
	if(enemyInfos.empty()) {
		return;
	}

	targetPosition = enemyInfos[0].position;
	float highestHP = static_cast<float>(enemyInfos[0].health) / static_cast<float>(enemyInfos[0].maxHealth);
	currentTargetID = enemyInfos[0].senderID;
	for(unsigned int i = 1; i < enemyInfos.size(); ++i) {
		float hp = static_cast<float>(enemyInfos[i].health) / static_cast<float>(enemyInfos[i].maxHealth);
		if(hp > highestHP) {
			highestHP = hp;
			this->targetPosition = enemyInfos[i].position;
			currentTargetID = enemyInfos[i].senderID;
		}
	}
}

//Interface Function(s)
void Attacker_Attack::Update(double deltaTime) {
	if (Think(deltaTime)) {
		Act(deltaTime);
	}
	hasSupport = false;
	enemyInfos.clear();
}

void Attacker_Attack::ReceiveMessage(const Message* messagePtr) {
	const CharacterInfo* charInfo = dynamic_cast<const CharacterInfo*>(messagePtr);
	if (charInfo != nullptr && charInfo->isAlive) {
		if (charInfo->inParty == GetOwner().InParty()) {
			if (charInfo->name == "Support") {
				hasSupport = charInfo->isAlive;
			}
		} else {
			float distanceSquared = (charInfo->position - GetOwner().GetPosition()).LengthSquared();
			if (distanceSquared < GetOwner().GetMaxCombatRange() * GetOwner().GetMaxCombatRange()) {
				enemyInfos.push_back(*charInfo);
			}
		}
	}
}

Attacker_Attack::ATTACK_MODE Attacker_Attack::GetAttackMode() const {

	return currentMode;

}