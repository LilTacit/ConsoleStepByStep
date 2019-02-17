#pragma once

#ifndef H_BASIC_CHARACTER
#define H_BASIC_CHARACTER

#include "MapNavigation.h"
#include "Treasure.h"

using namespace std;

/*
	DESCRIPTION: 
basic character class using as skelet for Player & AI.


	CHARACTER:
  description:
- Name

  params:
- HP
- Armoure
- Speed
- Damage
- (Enum) Type : Player, Beast, Human, Monster
- (Enum) Character : Player, Neutral, Enemy, Friendly


	BODY PART:
  description:
character sub object, for battle mechanic

  vars:
- name			: string for out in console
- hitChance		: hit or miss float param, in 0.0 - 1.0 range
- strength		: strength bodyPart define is break or not this part
- isBreake		: break or not
- isParry		: set parry on this part or not
*/

//World parametrs
const enum Type
{
	Basic, Beast, Human, Monster, PlayerType, EnemyType
};

const enum Character
{
	Neutral, Evil, Friendly
};

struct BodyPart
{
	string name;
	int hitChance = 100;
	int hitChanceConst = 100;
	float strength;
	float strengthAmount;
	float breakFactor = 1.0;
	bool isBreake = false, isParry = false;

	BodyPart(string name, int hitChance, float strength, float breakFactor)
		: name (name), hitChance(hitChance), strength(strength), breakFactor(breakFactor)
	{
		strengthAmount = strength;
		hitChanceConst = hitChance;
	}
};

//Classes
class BasicCharacter
{
private:
	string name;
	float hp, armour, speed, damage;
	float fullHp;
	bool isDead;
	MapPosition currentPosition;

protected:
	enum Type type;
	enum Character character;
	vector<BodyPart> bodyParts;

public:
	BasicCharacter(MapPosition position, string name = "Unknown", float hp = 1.0, float armour = 0.0, float speed = 1.0, float damage = 0.0);

	//getters
	bool checkDead() { return isDead; }
	float getHp() { return hp; }
	float getAmour() { return armour; }
	float getSpeed() { return speed; }
	float getDamage() { return damage; }
	string getName() { return name; }
	Type getType() { return type; }
	Character getCharacter() { return character; }
	MapPosition getCurrentPosition() { return currentPosition; }
	int getBodyPartsAmount() { return bodyParts.size(); }
	int getBodyPartHitChance(int num) { return bodyParts[num].hitChance; }
	string getBodyPartName(int num) { return bodyParts[num].name; }
	//setters
	void setCurrentPosition(MapPosition position) { this->currentPosition = position; }
	void setIsDead(bool is) { isDead = is; }

	//basic variabels
	void reduceHp(float damage);

	//show params
	virtual void showCharacterParams();
	virtual void showBattleParams();
	virtual void showBodyParts();

	//battle actions
	virtual void applyDamageToBodyPart(int num, float damage);
	void setParryToBodyPart(int num);
	void clearParry();
	bool checkParryOnBodyPart(int num);
	void clearBoostBodyPartsHitChance();
	void boostBodyPartHitChance(int num);

	//for GameMode
	virtual void resetCharacter();
};

//Player
class Player : public BasicCharacter
{
private:
	Gold gold = Gold(0);

public:
	Player(MapPosition position = MapPosition{}, string name = "Unknown", float hp = 100.0f, float armour = 14.0f, float speed = 2.0f, float damage = 12.0f);

	//inherited methods
	void showCharacterParams() override;
	void showBattleParams() override;
	void showBodyParts() override;
	void applyDamageToBodyPart(int num, float damage) override;

	//Player methods
	int getGoldAmount();
	void showGold();
	void increaseGold(Gold gold);
	void collectTreasure(Treasure treasure);

	//for GameMode
	void resetCharacter() override;
};

struct PreferedBodyPartNum
{
	int num = 1, chance = 100;
};

//Enemies
class Enemy : public BasicCharacter
{
protected:
	Treasure treasure;

public:
	Enemy(Treasure treasure = Treasure{}, MapPosition position = MapPosition{}, string name = "Draugr", float hp = 125.0f, float armour = 18.0f, float speed = 2.0f, float damage = 14.0f);

	//getters & setters
	Treasure getTreasure() { return treasure; }

	//inherited methods
	void showCharacterParams() override;
	void showBattleParams() override;
	void showBodyParts() override;
	void applyDamageToBodyPart(int num, float damage) override;

	vector<PreferedBodyPartNum> preferedBodyPartNums;
};

class WeakEnemy : public Enemy
{
public:
	WeakEnemy(Treasure treasure = Treasure{}, MapPosition position = MapPosition{}, string name = "Weak Draugr", float hp = 90.0f, float armour = 14.0f, float speed = 2.0f, float damage = 12.0f);
};

class Minotaur : public Enemy
{
public:
	Minotaur(Treasure treasure = Treasure{}, MapPosition position = MapPosition{}, string name = "Minotaur", float hp = 180.0f, float armour = 10.0f, float speed = 1.0f, float damage = 20.0f);
};

class SmallMinotaur : public Minotaur
{
public:
	SmallMinotaur(Treasure treasure = Treasure{}, MapPosition position = MapPosition{}, string name = "Small Minotaur", float hp = 126.0f, float armour = 7.0f, float speed = 1.0f, float damage = 16.0f);
};

class StoneElemental : public Enemy
{
public:
	StoneElemental(Treasure treasure = Treasure{}, MapPosition position = MapPosition{}, string name = "Stone Elemental", float hp = 240.0f, float armour = 0.0f, float speed = 1.0f, float damage = 16.0f);
};

#endif // !H_BASIC_CHARACTER