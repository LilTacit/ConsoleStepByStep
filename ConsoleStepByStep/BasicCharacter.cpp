#include "pch.h"
#include "BasicCharacter.h"

//Basic Character:
BasicCharacter::BasicCharacter(MapPosition position, string name, float hp, float armour, float speed, float damage)
{
	//definition
	this->name = name; this->hp = hp; this->fullHp = hp; this->armour = armour; this->speed = speed; this->damage = damage; this->isDead = false;
	this->type = Type{ Basic };
	this->character = Character{Neutral};
	
	if (position.xPosition >= 0 && position.yPosition >= 0) this->currentPosition = position;
	else this->currentPosition = MapPosition{ 0, 0 };
}

//basic variabels
void BasicCharacter::reduceHp(float damage)
{
	hp -= damage;
}

//Show params
void BasicCharacter::showCharacterParams()
{
	cout << "  Name: " << name << ", hp: " << hp << ", armour: " << armour << ", speed: " << speed << ", damage: " << damage << endl;
}

void BasicCharacter::showBattleParams()
{
	cout << "  " << name << ", " << hp << "hp.\n";
}

void BasicCharacter::showBodyParts()
{
	cout << "    " << name << " body:" << "\n";
	
	for (int i = 0; i < bodyParts.size(); i++)
	{
		cout << "  ";

		BodyPart &part = bodyParts[i];

		string boostChance = "";
		if (part.hitChance > part.hitChanceConst)
			boostChance = "<BOOSTED CHANCE> ";

		if (!part.isBreake)
			cout << i << ". " << boostChance << part.name << " [" << part.strength << "] " << part.hitChance << "% chance\n";
		else
			cout << i << ". ~" << boostChance << part.name << " [Broken] " << part.hitChance << "% chance\n";
	}
}

//Battle actions
void BasicCharacter::applyDamageToBodyPart(int num, float damage)
{
	BodyPart &part = bodyParts[num];
	part.strength -= damage;

	if (part.strength <= 0 && !part.isBreake)
	{
		part.isBreake = true;

		float hpDamage = part.strengthAmount * part.breakFactor;
		reduceHp(hpDamage);
		
		cout << "\n";
		cout << "  ~~~ " << name << " break body ~~~" << "\n";
		cout << "  " << name << " is broke " << part.name << "\n";
		cout << "  " << name << " has took " << hpDamage << " damage" << "\n";
		showBattleParams();
		cout << "\n";
	}
}

void BasicCharacter::setParryToBodyPart(int num)
{
	bodyParts[num].isParry = true;
}

void BasicCharacter::clearParry()
{
	for (BodyPart &part : bodyParts)
		part.isParry = false;
}

bool BasicCharacter::checkParryOnBodyPart(int num)
{
	return bodyParts[num].isParry;
}

void BasicCharacter::clearBoostBodyPartsHitChance()
{
	for (BodyPart &part : bodyParts)
		part.hitChance = part.hitChanceConst;
}

void BasicCharacter::boostBodyPartHitChance(int num)
{
	if (num >= 0 && num < bodyParts.size())
	{
		BodyPart &part = bodyParts[num];
		part.hitChance += part.hitChance;

		if (part.hitChance > 100)
			part.hitChance = 100;
	}
}

//For GameMode
void BasicCharacter::resetCharacter()
{
	isDead = false;
	hp = fullHp;

	for (BodyPart &part : bodyParts)
	{
		part.isBreake = false;
		part.isParry = false;
		part.strength = part.strengthAmount;
	}
}

//Player:
Player::Player(MapPosition position, string name, float hp, float armour, float speed, float damage)
	: BasicCharacter(position, name, hp, armour, speed, damage)
{
	//character
	this->type = Type{ PlayerType };
	this->character = Character{ Neutral };

	//body parts
	this->bodyParts.clear();

	this->bodyParts.push_back(BodyPart{ "Head", 30, 16, 3 });
	this->bodyParts.push_back(BodyPart{ "Chest", 80, 60, 0.75 });
	this->bodyParts.push_back(BodyPart{ "Left arm", 40, 40, 1 });
	this->bodyParts.push_back(BodyPart{ "Right arm", 40, 40, 1 });
	this->bodyParts.push_back(BodyPart{ "Left leg", 60, 50, 1 });
	this->bodyParts.push_back(BodyPart{ "Right leg", 60, 50, 1 });
}

void Player::showCharacterParams()
{
	BasicCharacter::showCharacterParams();	//Super
}

void Player::showBattleParams()
{
	BasicCharacter::showBattleParams();	//Super
}

void Player::showBodyParts()
{
	BasicCharacter::showBodyParts();	//Super
}

void Player::applyDamageToBodyPart(int num, float damage)
{
	BasicCharacter::applyDamageToBodyPart(num, damage);	//Super
}

int Player::getGoldAmount()
{
	return gold.getAmount();
}

void Player::showGold()
{
	cout << "You have: " << gold.getAmount() << "\n";
}

void Player::increaseGold(Gold gold)
{
	this->gold += gold;
}

void Player::collectTreasure(Treasure treasure)
{
	gold += treasure.getGold();
}

void Player::resetCharacter()
{
	BasicCharacter::resetCharacter();
	gold = Gold(0);
}

//Enemies
Enemy::Enemy(Treasure treasure, MapPosition position, string name, float hp, float armour, float speed, float damage)
	: BasicCharacter(position, name, hp, armour, speed, damage)
{
	//treasure
	this->treasure = treasure;

	//character
	this->type = Type{ EnemyType };
	this->character = Character{ Evil };

	//body parts
	this->bodyParts.clear();

	this->bodyParts.push_back(BodyPart{ "Head", 30, 16, 3 });
	this->bodyParts.push_back(BodyPart{ "Chest", 80, 60, 0.75 });
	this->bodyParts.push_back(BodyPart{ "Left arm", 40, 40, 1 });
	this->bodyParts.push_back(BodyPart{ "Right arm", 40, 40, 1 });
	this->bodyParts.push_back(BodyPart{ "Left leg", 60, 50, 1 });
	this->bodyParts.push_back(BodyPart{ "Right leg", 60, 50, 1 });

	this->preferedBodyPartNums.clear();
	this->preferedBodyPartNums = { PreferedBodyPartNum{ 1, 50 }, PreferedBodyPartNum{ 2, 50 }, PreferedBodyPartNum{ 3, 50 } };
}

WeakEnemy::WeakEnemy(Treasure treasure, MapPosition position, string name, float hp, float armour, float speed, float damage)
	: Enemy(treasure, position, name, hp, armour, speed, damage)
{
}

Minotaur::Minotaur(Treasure treasure, MapPosition position, string name, float hp, float armour, float speed, float damage)
	: Enemy(treasure, position, name, hp, armour, speed, damage)
{
	//body parts
	this->bodyParts.clear();

	this->bodyParts.push_back(BodyPart{ "Left horn", 40, 16, 3 });
	this->bodyParts.push_back(BodyPart{ "Right horn", 40, 16, 3 });
	this->bodyParts.push_back(BodyPart{ "Head", 40, 40, 1.5 });
	this->bodyParts.push_back(BodyPart{ "Chest", 90, 120, 0.5 });
	this->bodyParts.push_back(BodyPart{ "Left arm", 60, 70, 1 });
	this->bodyParts.push_back(BodyPart{ "Right arm", 60, 70, 1 });
	this->bodyParts.push_back(BodyPart{ "Left leg", 80, 90, 1 });
	this->bodyParts.push_back(BodyPart{ "Right leg", 80, 90, 1 });
	this->bodyParts.push_back(BodyPart{ "Tail", 30, 8, 4 });

	this->preferedBodyPartNums.clear();
	this->preferedBodyPartNums = { PreferedBodyPartNum{ 2, 50 }, PreferedBodyPartNum{ 3, 25 }, PreferedBodyPartNum{ 0, 50 },
		PreferedBodyPartNum{ 1, 80 }, PreferedBodyPartNum{ 4, 50 }, PreferedBodyPartNum{ 5, 25 } };
}

SmallMinotaur::SmallMinotaur(Treasure treasure, MapPosition position, string name, float hp, float armour, float speed, float damage)
	: Minotaur(treasure, position, name, hp, armour, speed, damage)
{
	//body parts
	this->bodyParts.clear();

	this->bodyParts.push_back(BodyPart{ "Left horn", 45, 12, 3 });
	this->bodyParts.push_back(BodyPart{ "Right horn", 45, 12, 3 });
	this->bodyParts.push_back(BodyPart{ "Head", 45, 21, 1.5 });
	this->bodyParts.push_back(BodyPart{ "Chest", 90, 85, 0.75 });
	this->bodyParts.push_back(BodyPart{ "Left arm", 65, 50, 1 });
	this->bodyParts.push_back(BodyPart{ "Right arm", 65, 50, 1 });
	this->bodyParts.push_back(BodyPart{ "Left leg", 85, 65, 1 });
	this->bodyParts.push_back(BodyPart{ "Right leg", 85, 65, 1 });
	this->bodyParts.push_back(BodyPart{ "Tail", 45, 8, 4 });
}

StoneElemental::StoneElemental(Treasure treasure, MapPosition position, string name, float hp, float armour, float speed, float damage)
	: Enemy(treasure, position, name, hp, armour, speed, damage)
{
	//body parts
	this->bodyParts.clear();

	this->bodyParts.push_back(BodyPart{ "Diamond", 10, 20, 8 });
	this->bodyParts.push_back(BodyPart{ "Head", 40, 36, 4 });
	this->bodyParts.push_back(BodyPart{ "Left rock 1", 35, 24, 4 });
	this->bodyParts.push_back(BodyPart{ "Left rock 2", 45, 24, 4 });
	this->bodyParts.push_back(BodyPart{ "Right rock 1", 45, 24, 4 });
	this->bodyParts.push_back(BodyPart{ "Right rock 2", 35, 24, 4 });
	this->bodyParts.push_back(BodyPart{ "Ruby", 35, 12, 6 });
	this->bodyParts.push_back(BodyPart{ "Stone chest", 95, 80, 2 });
	this->bodyParts.push_back(BodyPart{ "Stone belt", 60, 50, 2 });
	this->bodyParts.push_back(BodyPart{ "Amethyst", 20, 16, 8 });

	this->preferedBodyPartNums.clear();
	this->preferedBodyPartNums = { PreferedBodyPartNum{ 0, 50 }, PreferedBodyPartNum{ 1, 25 }, PreferedBodyPartNum{ 2, 50 },
		PreferedBodyPartNum{ 3, 25 }, PreferedBodyPartNum{ 4, 50 }, PreferedBodyPartNum{ 5, 25 } };
}

void Enemy::showCharacterParams()
{
	BasicCharacter::showCharacterParams();	//Super
}

void Enemy::showBattleParams()
{
	BasicCharacter::showBattleParams();	//Super
}

void Enemy::showBodyParts()
{
	BasicCharacter::showBodyParts();	//Super
}

void Enemy::applyDamageToBodyPart(int num, float damage)
{
	BasicCharacter::applyDamageToBodyPart(num, damage);	//Super
}

// *** Test ***
void characterTest()
{
	Player player(MapPosition{ 0, 1 }, "Player", 100.0, 10.0, 10.0, 10.0);
	player.showCharacterParams();
	cout << "\n\n";

	Enemy enemy(Treasure(MapPosition{0,0}, 100), MapPosition{ 0,0 }, "Enemy", 100.0, 10.0, 10.0, 10.0);
	enemy.showCharacterParams();
}

/*
	* Copyright (C) 2019 {Kirill Andreev}
	________
*/