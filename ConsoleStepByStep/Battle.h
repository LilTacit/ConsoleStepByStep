#pragma once

#ifndef H_BATTLE
#define H_BATTLE

#include "BasicCharacter.h"
#include <random>
#include <ctime>

/*
	MECHANIC:

  Step:
If player step - enemy attack = 0,
if enemy step - player attack = 0,
as a result - player or enemy step decide attack amount.

  Damage & defence calculate:
- Enemy:
	Damage & defence apply directly, from enemy damage & armour.
- Player:
	For do 100% damage or defence need select equal or greate stamin points than damage or defence step constant.
	For example: attack step = 6, character damage = 10, for do 10 damage - need select 6 stamina points.


	BATTLE CYCLE:

{
- if(player step)
	set player attack();
	set enemy defence();
	set enemy attack( to 0 );

- if(enemy step)
	set player defence();
	set enemy attack();
	set player attack(to 0);

- confirm()
	Apply all params (damage & defence) to player and enemy;
}


	FUNCTOINS:

  Main:
- battleBegin();
	Decide who step first by characters speed. And out begin battle attention.
- battleEnd();
	... And out end battle attention.
- battleCycle();
	Contain main cycle, which end when one of characters die.

  Player actions:
- playerDoAttack();
	...
- playerDoDefence();
	...

  Enemy actions:
- enemyDoAttack();
	...
- enemyDoDefence();
	...
*/

class Battle
{
private:
	//pointers
	Player *player;
	Enemy *enemy;
	//characters var
	float playerStamina = 10, playerExtraStamina = 0, playerSelectedAttackStamina = 0, playerSelectedDefenceStamina = 0;
	float playerDamage = 0, playerDefence = 1.0;
	float enemyDamage = 0, enemyDefence = 1.0;
	float const maxPlayerStamina = 10;
	float const playerAttackStage = 6;
	float const playerDeffenceStage = 2;
	int playerSelectedBodyPartNum = 0;
	int playerSelectedExtraBodyPartNum = -1;
	int enemySelectedBodyPartNum = 1;
	//strings
	const string incorrectSelectedStaminaPoints = "You write incorrect amount points, try again.";
	const string incorrectSelectedBodyPartNum = "You write incorrect body part number.";
	const string winAttention = "Enemy is dead. You win!";
	const string defeatAttention = "You died.";
	const string extraAttackAttention = "~ EXTRA! You might do extra attack! ~";
	//bool`s
	bool isBothCharactersAlive = true;
	bool isPlayerStep = true;
	bool isPlayerStaned = false, isEnemyStaned = false;
	bool isPlayerDoExtraAttack = false;

public:
	Battle(Player* player, Enemy* enemy);

	//main cycle
	void battleCycle();
	void battleBegin();
	void battleEnd();
	void confirmStep();
	
	//player actions
		//atack
	void playerDoAttack();
	void playerDoSelectBodyPart();
	void playerDoSelectExtraBodyPart();
	void playerDoSelectAttackPoints();
	void playerConfirmAttack();
		//defence
	void playerDoDefence();

	//enemy actions
		//attack
	void enemyDoAttack();
	void enemyConfirmAttack();
		//defence
	void enemyDoDefence();

	//console attentions
	void showPlayerStamina();

	//check
	bool checkNumInput(string str);
	bool checkPlayerExtraAttack();

	//randomise
	int getRandomNumberInRange(int fromI, int toI);
};

#endif // !H_BATTLE