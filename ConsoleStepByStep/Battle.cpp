#include "pch.h"
#include "Battle.h"

Battle::Battle(Player* player, Enemy* enemy)
{
	//definition
	this->player = player; this->enemy = enemy;
}

//Player actions
void Battle::playerDoAttack()
{
	playerDoSelectAttackPoints();
	playerDoSelectBodyPart();
}

void Battle::playerDoSelectBodyPart()
{
	cout << "\n";
	enemy->showBodyParts();

	cout << "\n";
	cout << "  select body part to attack: ";
	string playerInput;
	cin >> playerInput;
	cout << "\n";

	if (checkNumInput(playerInput))
	{
		int selectedBodyPart = stoi(playerInput);

		if (selectedBodyPart >= 0 && selectedBodyPart < enemy->getBodyPartsAmount())
		{
			playerSelectedBodyPartNum = selectedBodyPart;

			if (isPlayerDoExtraAttack)
				playerDoSelectExtraBodyPart();
		}
		else
		{
			//recurse
			cout << "  " << incorrectSelectedBodyPartNum << endl;
			playerDoSelectBodyPart();
		}
	}
	else
	{
		//recurse
		cout << "  " << incorrectSelectedBodyPartNum << endl;
		playerDoSelectBodyPart();
	}
}

void Battle::playerDoSelectExtraBodyPart()
{
	cout << "\n";
	enemy->showBodyParts();

	cout << "\n";
	cout << "  select extra body part to attack: ";
	string playerInput;
	cin >> playerInput;
	cout << "\n";

	if (checkNumInput(playerInput))
	{
		int selectedBodyPart = stoi(playerInput);

		if (selectedBodyPart >= 0 && selectedBodyPart < enemy->getBodyPartsAmount())
		{
			playerSelectedExtraBodyPartNum = selectedBodyPart;
		}
		else
		{
			//recurse
			cout << incorrectSelectedBodyPartNum << endl;
			playerDoSelectExtraBodyPart();
		}
	}
	else
	{
		//recurse
		cout << incorrectSelectedBodyPartNum << endl;
		playerDoSelectExtraBodyPart();
	}
}

void Battle::playerDoSelectAttackPoints()
{
	cout << "\n";
	showPlayerStamina();

	cout << "  select attack points: ";
	string playerInput;
	cin >> playerInput;
	cout << "\n";

	if (checkNumInput(playerInput))
	{
		int tempI = stoi(playerInput);

		if (tempI >= 0 && tempI <= (maxPlayerStamina + playerExtraStamina))
		{
			playerSelectedAttackStamina = tempI;

			if (playerSelectedAttackStamina > playerStamina)
			{
				//move extra stamina to common stamina
				float selectedExtraPoints = playerSelectedAttackStamina - playerStamina;
				playerExtraStamina -= selectedExtraPoints;
				playerStamina += selectedExtraPoints;
			}

			playerStamina -= playerSelectedAttackStamina;

			//extra attack
			if (checkPlayerExtraAttack())
			{
				isPlayerDoExtraAttack = true;

				playerSelectedAttackStamina = playerSelectedAttackStamina / 2;

				cout << "\n";
				cout << "  " << extraAttackAttention << "\n";
			}
			cout << "\n";
		}
		else
		{
			//recurse
			cout << "  " << incorrectSelectedStaminaPoints << endl;
			playerDoSelectAttackPoints();
		}
	}
	else
	{
		//recurse
		cout << "  " << incorrectSelectedStaminaPoints << endl;
		playerDoSelectAttackPoints();
	}
}

void Battle::playerConfirmAttack()
{
	//randomise
	int randChance = getRandomNumberInRange(0, 100);

	if (enemy->getBodyPartHitChance(playerSelectedBodyPartNum) >= randChance)
	{
		//hit
		playerDamage = player->getDamage() * (playerSelectedAttackStamina / playerAttackStage);		//basic damage by stamine
		playerDamage = playerDamage * (1.0 - (enemyDefence / 100.0));								//damage - armour

		enemy->reduceHp(playerDamage);
		cout << "  Player do " << playerDamage << " damage to " << enemy->getBodyPartName(playerSelectedBodyPartNum) << "\n";

		//reduce strength
		enemy->applyDamageToBodyPart(playerSelectedBodyPartNum, playerDamage);

		if (playerSelectedExtraBodyPartNum >= 0)
		{
			//move extraBodyPart to common bodyPart
			playerSelectedBodyPartNum = playerSelectedExtraBodyPartNum;
			playerSelectedExtraBodyPartNum = -1;

			playerConfirmAttack();
		}

		//boost part hit chance
		enemy->clearBoostBodyPartsHitChance();
		enemy->boostBodyPartHitChance(playerSelectedBodyPartNum);
	}
	else
	{
		//miss
		cout << "  " << player->getName() << " missed\n";
	}

	enemy->showBodyParts();
}

void Battle::playerDoDefence()
{
	if (playerStamina > 0 || playerExtraStamina > 0)
	{
		cout << "\n";
		showPlayerStamina();

		cout << "  select defence points: ";
		string playerInput;
		cin >> playerInput;
		cout << "\n";

		if (checkNumInput(playerInput))
		{
			int tempI = stoi(playerInput);
			
			if(tempI >= 0 && tempI <= (maxPlayerStamina + playerExtraStamina) && tempI <= (playerStamina + playerExtraStamina) )
			{
				playerSelectedDefenceStamina = tempI;

				//check stage 2: parry
				if (playerSelectedDefenceStamina >= (playerDeffenceStage * 2))
				{
					//show bodyPart
					//get player input for parry
					//valid player input
						//set parry to playerBodyPart

					while (true)
					{
						string playerInput;

						cout << "\n";
						player->showBodyParts();

						cout << "\n";
						cout << "  set parry to: ";
						cin >> playerInput;
						cout << "\n";

						if (checkNumInput(playerInput))
						{
							int tempI = stoi(playerInput);

							if (tempI >= 0 && tempI < player->getBodyPartsAmount())
							{
								player->setParryToBodyPart(tempI);
								break;
							}
							else
							{
								//incorrect
								cout << "  " << incorrectSelectedBodyPartNum << endl;
							}
						}
						else
						{
							//incorrect
							cout << "  " << incorrectSelectedBodyPartNum << endl;
						}

						cout << "\n";
					}
				}

				if (playerSelectedDefenceStamina > playerStamina)
				{
					float selectedExtraPoints = playerSelectedDefenceStamina - playerStamina;
					playerExtraStamina -= selectedExtraPoints;
					playerStamina += selectedExtraPoints;
				}

				playerStamina -= playerSelectedDefenceStamina;

				cout << "\n";
			}
			else
			{
				//recurse
				cout << "  " << incorrectSelectedStaminaPoints << endl;
				playerDoDefence();
			}
		}
		else
		{
			//recurse
			cout << "  " << incorrectSelectedStaminaPoints << endl;
			playerDoDefence();
		}
	}
}

//Enemy actions
void Battle::enemyDoAttack()
{
	if (!isEnemyStaned)
	{
		//randomise
		int randInt = -1;

		do
		{
			for (PreferedBodyPartNum preferedBodyPart : enemy->preferedBodyPartNums)
			{
				if (getRandomNumberInRange(0, 100) <= preferedBodyPart.chance)
				{
					randInt = preferedBodyPart.num;
					break;
				}
			}
		} while (randInt < 0);

		enemySelectedBodyPartNum = randInt;

		//damage
		enemyDamage = enemy->getDamage();
	}
}

void Battle::enemyConfirmAttack()
{
	//check hit or miss
	int randChance = getRandomNumberInRange(0, 100);

	if (player->getBodyPartHitChance(enemySelectedBodyPartNum) >= randChance)
	{
		if (player->checkParryOnBodyPart(enemySelectedBodyPartNum))
		{
			//parry
			isEnemyStaned = true;
			cout << "\n";
			cout << "  " << player->getName() << " stunned enemy\n";
		}
		else
		{
			//not parry
			//hit
			enemyDamage = enemyDamage * (1.0 - (player->getAmour() * ((playerSelectedDefenceStamina / playerDeffenceStage) / 100)));	//damage - armour

			player->reduceHp(enemyDamage);
			cout << "\n";
			cout << "  Enemy do " << enemyDamage << " damage to " << player->getBodyPartName(enemySelectedBodyPartNum) << "\n";

			//reduce strength
			player->applyDamageToBodyPart(enemySelectedBodyPartNum, enemyDamage);
		}
	}
	else
	{
		//miss
		cout << "  " << enemy->getName() << " missed\n";
	}

	player->showBodyParts();
}

void Battle::enemyDoDefence()
{
	if(!isEnemyStaned)
		enemyDefence = enemy->getAmour();
}

//Main
void Battle::battleCycle()
{
	battleBegin();

	//Main Cycle
	while (isBothCharactersAlive)
	{
		if (isPlayerStep)
		{
			playerDoAttack();
			enemyDoDefence();

			enemyDamage = 0;
		}
		else
		{
			playerDoDefence();
			enemyDoAttack();
		}

		confirmStep();

		if (!isPlayerStep)
		{
			//new round
			playerExtraStamina += playerStamina;
			playerStamina = maxPlayerStamina;

			player->clearParry();
			enemy->clearParry();

			playerSelectedExtraBodyPartNum = -1;
			isPlayerDoExtraAttack = false;

			cout << "\n\n";
			cout << "  ~~~ New round ~~~" << "\n";
		}

		isPlayerStep = !isPlayerStep; //switch step
	}

	battleEnd();
}

void Battle::battleBegin()
{
	string whoStepFirst = "Your speed more than enemy - you attack first.";

	if (player->getSpeed() < enemy->getSpeed())
	{
		isPlayerStep = false;
		whoStepFirst = "Your speed less than enemy - defence now.";
	}
	else if (player->getSpeed() == enemy->getSpeed())
	{
		whoStepFirst = "Your speed equal enemy \n  Rand...\n  ";
		isPlayerStep = (getRandomNumberInRange(1, 2) > 1);
		if (isPlayerStep)
			whoStepFirst += "You attack first.";
		else
			whoStepFirst += "Enemy attack first";
	}

	cout << "  ~~~~* Battle Start *~~~~" << "\n\n";

	enemy->showCharacterParams();
	player->showCharacterParams();
	cout << "\n\n";
	
	//battle mechanic description
	cout << "    < Battle mechanic >" << "\n";
	cout << "  * That to do 100% damage from player attack - you need select min. 6 stamina points" << "\n";
	cout << "  * That to do 100% defence from player armour - you need select min. 2 stamina points" << "\n";
	cout << "  * Attack stage 2: if you select min. 12 attack points - you might to do extra attack" << "\n";
	cout << "  * Defence stage 2: if you select min. 4 attack points - you might set parry on 1 self body part" << "\n";
	cout << "\n\n";

	cout << "  " << whoStepFirst << "\n\n";
}

void Battle::battleEnd()
{
	if (enemy->getHp() <= 0)
	{
		//win
		player->collectTreasure(enemy->getTreasure());

		cout << "\n\n" << winAttention << endl;

		enemy->setIsDead(true);
	}

	if (player->getHp() <= 0)
	{
		//defeat
		cout << "  " << defeatAttention << endl;

		player->setIsDead(true);
	}

	cout << "\n" << "~~~~* Battle End *~~~~" << "\n\n";
}

void Battle::confirmStep()
{
	//player attack enemy
	if (playerSelectedAttackStamina > 0)
		playerConfirmAttack();

	//enemy attack player
	if (!isEnemyStaned)
	{
		if (enemyDamage > 0)
			enemyConfirmAttack();
	}
	else
		isEnemyStaned = false;

	//player->showBodyParts();

	playerSelectedAttackStamina = 0;
	playerSelectedDefenceStamina = 0;

	cout << "\n";
	player->showBattleParams();
	enemy->showBattleParams();

	//check end battle
	if (player->getHp() <= 0 || enemy->getHp() <= 0)
	{
		isBothCharactersAlive = false;
	}
}

//Console attentions
void Battle::showPlayerStamina()
{
	cout << "  You have "
		<< (playerStamina - playerSelectedAttackStamina - playerSelectedDefenceStamina)
		<< " stamina.\n";

	if (playerExtraStamina > 0)
		cout << "  And " << playerExtraStamina << " extra stamina.\n";
}

//Check
bool Battle::checkNumInput(string str)
{
	bool is = true;

	for (char c : str)
	{
		if (!isdigit(c))
		{
			is = false;
			break;
		}
	}

	return is;
}

bool Battle::checkPlayerExtraAttack()
{
	return ( playerSelectedAttackStamina >= (playerAttackStage * 2) );
}

//randomise
int Battle::getRandomNumberInRange(int fromI, int toI)
{
	default_random_engine generator;
	uniform_int_distribution<int> distribution(fromI, toI);
	generator.seed(time(NULL));

	return distribution(generator);
}

void battleTest()
{
	//Player
	Player *player = new Player(MapPosition{}, "Player");

	//Common type
	Enemy *commonEnemy = new Enemy ();

	//Weak enemy
	WeakEnemy *weakEnemy = new WeakEnemy();

	//Minotaur
	Minotaur *minotaur = new Minotaur();

	//SmallMinotaur
	SmallMinotaur *smallMinotaur = new SmallMinotaur();

	//Stone Elemental
	StoneElemental *stoneElemental = new StoneElemental();

	Battle battle(player, stoneElemental);
	battle.battleCycle();
}

/*
	* Copyright (C) 2019 {Kirill Andreev}
	________
*/