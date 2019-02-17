#include "pch.h"
#include "GameMode.h"

GameMode::GameMode(Map* map, Player* player, vector<Enemy> enemies, vector<Treasure> treasures)
{
	//definition
	this->player = player;

	//set first level
	Level lvl1 = { map, enemies, treasures };
	levels.push_back(lvl1);
	setLevel(currentLevel);
	
	//StartUp functions:
	showBeginInformation();
	setUpGameMode();

	gameBegin();
	startMainGameCycle();
}

GameMode::GameMode(Player * player, vector<Level> levels)
{
	//definition
	this->player = player; this->levels = levels;

	//set first level
	setLevel(currentLevel);

	//StartUp functions:
	showBeginInformation();
	setUpGameMode();

	gameBegin();
	startMainGameCycle();
}

GameMode::~GameMode()
{
	delete map;
	delete player;
}

void GameMode::setUpGameMode()
{
	//select showing map in game process
	string showingMapParam;

	cout << "\n" << "  Show map in game process? (Not recommended, for atmosphere)" << "\n";
	cout << "  *Remark: it`s valid only for first level, couse i don`t have much time for write all turn descriptions, sorry for that." << "\n";

	while (true)
	{
		
		cout << "\n" << "  y / n (recommended): ";

		cin >> showingMapParam;

		if (showingMapParam == YES)
		{
			isShowingMap = true;
			break;
		}
		else if (showingMapParam == NO)
		{
			isShowingMap = false;
			break;
		}
		else
			cout << "  " << incorrectInput << "\n\n";
	}

	//~?* select difficult

	cout << "\n"<< "  ____________________" << "\n";
}

void GameMode::gameBegin()
{
	//place player on map
	map->placeCharacterOnPath(*player, map->getStartPosition());
	player->setCurrentPosition(map->getStartPosition());

	//place enemy on map
	for (Enemy& e : enemies)
		map->placeCharacterOnPath(e, e.getCurrentPosition());

	//place treasures on map
	for (Treasure& treasure : treasures)
		map->placeTreasureOnPath(treasure, treasure.getPosition());

	cout << "\n";
	player->showCharacterParams();
	cout << "\n\n";

	if(isShowingMap)
		map->showMap();

	showUnderlineInConsole();
}

void GameMode::startMainGameCycle()
{
	while (true)
	{
		//check: player is dead
		if (player->checkDead())
		{
			playerDeadAction();
			break;
		}

		//check: player has come to end map
		if (player->getCurrentPosition() == map->getEndPosition())
		{
			cout << "  Complete!" << "\n\n";

			//Final score
			showGameScore();

			gameEndAction = GAME_NEXT_LEVEL;

			break;
		}

		vector<TurnDirectionWithType> validTurnDirectionsWithType = map->getValidTurnDirectionsWithType(player->getCurrentPosition());
		showValidTurnDirectionsInConsole(validTurnDirectionsWithType, player->getCurrentPosition());

		string playerInput;
		showUnderlineInConsole();
		cout << "\n" << "  Write your action: ";
		cin >> playerInput;
		cout << endl;

		//Main logic tree
		if (playerInput == EXIT)
		{
			//exit
			showExitNotification();
		}
		else
		{
			//turnDirection
			bool isSelecValidDirection = false;
			TurnDirectionWithType selectedDirection;

			for (TurnDirectionWithType direction : validTurnDirectionsWithType)
			{
				int i = direction.turnDirection.direction;
				if (to_string(i) == playerInput)
				{
					isSelecValidDirection = true;
					selectedDirection = direction;
					break;
				}
			}

			if (isSelecValidDirection)
			{
				if (selectedDirection.directionType == Turn)
				{
					movePlayerTo(map->getPositionToByTurnDirection(player->getCurrentPosition(), selectedDirection.turnDirection));
					
					if (isShowingMap)
						map->showMap();
				}
				else if (selectedDirection.directionType == Fight)
				{
					Enemy &enemyToBattle = enemies[ getEnemyNumByPosition(map->getNextPositionByTurnDirection(player->getCurrentPosition(), selectedDirection.turnDirection)) ];

					startBattle(&enemyToBattle);

					if (enemyToBattle.checkDead()) //player win
					{
						map->replaceCharacterFromPath(enemyToBattle.getCurrentPosition());

						movePlayerTo( map->getNextPositionByTurnDirection(player->getCurrentPosition(), selectedDirection.turnDirection) );

						if (isShowingMap)
							map->showMap();
					}
				}
				else if (selectedDirection.directionType == COLLECT)
				{
					MapCell treasureCell = map->getLinkCellByPostion(map->getNextPositionByTurnDirection(player->getCurrentPosition(), selectedDirection.turnDirection));
					Treasure &treasureToCollect = treasureCell.getTreasure();
					MapPosition treasurePosition = treasureToCollect.getPosition();

					playerCollectTreasure(treasureToCollect.getPosition());
					movePlayerTo( map->getNextPositionByTurnDirection(player->getCurrentPosition(), selectedDirection.turnDirection) );

					if (isShowingMap)
						map->showMap();
				}

				startMainGameCycle();
			}
			else
			{
				showInvalidActionNofication();
				startMainGameCycle();
			}
		}

		break;
	}

	if (gameEndAction == GAME_EXIT)
	{
		cout << "\n\n";
		showExitNotification();
	}
	else if (gameEndAction == GAME_RESTART)
	{
		cout << "\n\n";
		cout << "  game will be restart..." << "\n";
		restartGameMode();
	}
	else if (gameEndAction == GAME_NEXT_LEVEL)
	{
		cout << "\n\n";
		showGameScore();

		//set new level
		currentLevel++;

		if (currentLevel < levels.size())
		{
			player->resetCharacter();

			setLevel(currentLevel);
			gameBegin();
			startMainGameCycle();
		}
		else
		{
			//*! you complete all levels
			cout << "\n\n\n\n\n\n\n\n\n\n";
			cout << "  You pass all levels." << "\n";
		}
		
	}
}

void GameMode::playerDeadAction()
{
	string playerInput;

	cout << "\n\n";

	cout << "  For restart game - write RESTART" << "\n";
	cout << "  For exit - write EXIT" << "\n";
	cout << "  Action: ";

	cin >> playerInput;

	cout << "\n";

	if (playerInput == EXIT)
	{
		gameEndAction = GAME_EXIT;
	}
	else if (playerInput == RESTART)
	{
		gameEndAction = GAME_RESTART;
	}
	else
	{
		//recurse
		cout << "\n";
		cout << "  " << incorrectInput << "\n";

		playerDeadAction();
	}
}

void GameMode::restartGameMode()
{
	map->getLinkCellByPostion(player->getCurrentPosition()).replaceCharacter();

	player->resetCharacter();

	for (Enemy &enemy : enemies)
		enemy.resetCharacter();

	cout << "\n\n\n\n\n\n\n\n\n\n";
	
	showBeginInformation();

	gameBegin();
	startMainGameCycle();
}

void GameMode::setLevel(int number)
{
	if (number < levels.size() && number >= 0)
	{
		Level &selectedLevel = levels[number];

		map = selectedLevel.map;
		enemies = selectedLevel.enemies;
		treasures = selectedLevel.treasures;
	}
	else
	{
		//*! Error
	}
}

//Actions with player
void GameMode::movePlayerTo(MapPosition positionTo)
{
	//clear current player cell
	//set player current position from directionPath.end
	//set cell.placedCharacter -> player
	MapCell &currentPlayerCell = map->getLinkCellByPostion(player->getCurrentPosition());
	Player *emptyPlayer = nullptr;
	currentPlayerCell.setPlacedCharacter(*emptyPlayer);

	player->setCurrentPosition(positionTo);

	MapCell &cellToMove = map->getLinkCellByPostion(positionTo);
	cellToMove.setPlacedCharacter(*player);
}

void GameMode::playerCollectTreasure(MapPosition treasurePosition)
{
	if (map->checkValidPosition(treasurePosition))
	{
		MapCell& cell = map->getLinkCellByPostion(treasurePosition);

		if (cell.checkPath())
		{
			Treasure *treasure = &cell.getTreasure();

			if (treasure)
			{
				//show collect
				cout << "  " << player->getName() << ", collect " << treasure->getGold().getAmount() << " gold." << "\n\n";

				//move treasure to player
				player->collectTreasure(*treasure);

				//clear cell
				cell.replaceTreasure();
			}
		}
	}
}

//Fight
void GameMode::startBattle(Enemy * enemy)
{
	Battle currentBattle(player, enemy);
	currentBattle.battleCycle();
}

//
int GameMode::getEnemyNumByPosition(MapPosition position)
{
	int numToReturn = 0;

	for (int i = 0; i < enemies.size(); i++)
		if (enemies[i].getCurrentPosition() == position) numToReturn = i;

	return numToReturn;
}

//Turn Directions
void GameMode::showValidTurnDirectionsInConsole(vector<TurnDirection> turnDirections)
{
	cout << "\n" << "  You may go to:" << "\n";

	for (TurnDirection turnDirection : turnDirections)
	{
		//cout << turnDirection.direction << ". " <<  << endl;
	}
}

void GameMode::showValidTurnDirectionsInConsole(vector<TurnDirectionWithType> turnDirections)
{
	bool isTurnExit(false), isFightExist(false), isTreasureExist(false);

	for (TurnDirectionWithType& direction : turnDirections)
	{
		if (direction.directionType == Turn) isTurnExit = true;
		else if (direction.directionType == Fight) isFightExist = true;
		else if (direction.directionType == COLLECT) isTreasureExist = true;
	}

	if (isTurnExit)
	{
		cout << "\n" << "  You may go to:" << "\n";

		for (TurnDirectionWithType turnDirection : turnDirections)
			if (turnDirection.directionType == Turn) cout << "  " << turnDirection.turnDirection.direction << endl;
	}

	if (isFightExist)
	{
		cout << "\n" << "  You see enemy, for fight - go to:" << "\n";

		for (TurnDirectionWithType turnDirection : turnDirections)
			if (turnDirection.directionType == Fight) cout << "  " << turnDirection.turnDirection.direction << endl;
	}

	if (isTreasureExist)
	{
		cout << "\n" << "  You find treasure! - go to:" << "\n";

		for (TurnDirectionWithType turnDirection : turnDirections)
			if (turnDirection.directionType == COLLECT) cout << "  " << turnDirection.turnDirection.direction << endl;
	}
}

void GameMode::showValidTurnDirectionsInConsole(vector<TurnDirectionWithType> turnDirections, MapPosition positionForDescription)
{
	bool isTurnExit(false), isFightExist(false), isTreasureExist(false);

	for (TurnDirectionWithType& direction : turnDirections)
	{
		if (direction.directionType == Turn) isTurnExit = true;
		else if (direction.directionType == Fight) isFightExist = true;
		else if (direction.directionType == COLLECT) isTreasureExist = true;
	}

	if (isTurnExit)
	{
		cout << "\n" << "  You may go to:" << "\n";

		for (TurnDirectionWithType turnDirection : turnDirections)
		{
			if (turnDirection.directionType == Turn)
			{
				
				MapCell &cell = map->getLinkCellByPostion(positionForDescription);
				cout << "  " << turnDirection.turnDirection.direction << ". " << cell.getTurnDescription(turnDirection.turnDirection.direction) << endl;
			}
		}
	}

	if (isFightExist)
	{
		cout << "\n" << "  You see enemy, for fight - go to:" << "\n";

		for (TurnDirectionWithType turnDirection : turnDirections)
			if (turnDirection.directionType == Fight) cout << "  " << turnDirection.turnDirection.direction << endl;
	}

	if (isTreasureExist)
	{
		cout << "\n" << "  You find treasure! - go to:" << "\n";

		for (TurnDirectionWithType turnDirection : turnDirections)
			if (turnDirection.directionType == COLLECT) cout << "  " << turnDirection.turnDirection.direction << endl;
	}
}

//Show game information in console:
void GameMode::showBeginInformation()
{
	cout << "  Game is start." << "\n\n" 
		<< "  Rules & control:" << "\n"
		<< "  * If you wish exit - write: EXIT" << "\n"
		<< "  * For select path direction - write number of direction: 0, 1, 2, 3" << "\n";
	showUnderlineInConsole();
}

void GameMode::showUnderlineInConsole()
{
	cout << "\n" << "  ____________________" << "\n";
}

void GameMode::showExitNotification()
{
	cout << "  Thank`s for testing. Bye ^__^" << "\n";
}

void GameMode::showInvalidActionNofication()
{
	cout << "  You write invalid action, please try again." << "\n";
}

void GameMode::showGameScore()
{
	int collectedGold = player->getGoldAmount();
	int destroyedEnemy = 0;

	for (Enemy& e : enemies)
		if (e.checkDead()) destroyedEnemy++;

	cout << "  Final Score:" << "\n";
	cout << "  * Gold: " << collectedGold << "\n";
	cout << "  * Enemy destroyed: " << destroyedEnemy << "\n";
}

//Overload operators:
bool operator==(MapPosition pos1, MapPosition pos2)
{
	return pos1.xPosition == pos2.xPosition && pos1.yPosition == pos2.yPosition;
}

ostream & operator<<(ostream & out, Direction direction)
{
	int i = direction;
	string directionStr = "Left";

	if (i == 1) directionStr = "Right";
	else if (i == 2) directionStr = "Up";
	else if (i == 3) directionStr = "Down";

	out << i << ". " << directionStr;
	return out;
}

// *** Test ***
void gameModeTest()
{
	Player *player = new Player(MapPosition{ 0, 0 }, "Player", 100.0, 10.0, 10.0, 10.0);
	vector<Level> levels;

	//level 1
		//build map
	Map *map1 = new Map(MapPosition{ 0, 1 }, MapPosition{ 6, 2 }, 7, 5);
	
	vector<MapDirectionPath> roads1;
	roads1.push_back(MapDirectionPath{ MapPosition{ 3, 3 }, MapPosition{ 5, 3 }, true });
	roads1.push_back(MapDirectionPath{ MapPosition{ 3, 0 }, MapPosition{ 3, 3 }, false });
	roads1.push_back(MapDirectionPath{ MapPosition{ 0, 1 }, MapPosition{ 3, 1 }, true });
	roads1.push_back(MapDirectionPath{ MapPosition{ 5, 2 }, MapPosition{ 5, 4 }, false });
	vector<MapDirectionPath> &roads1L = roads1;
			//build roads
	map1->buildMap(roads1L);

		//add treasure
	vector<Treasure> treasures1;
	Treasure treasure1_1(MapPosition{5, 3}, 250);
	Treasure treasure1_2(MapPosition{3, 0}, 100);
	treasures1.push_back(treasure1_1);
	treasures1.push_back(treasure1_2);

		//add enemy
	vector<Enemy> enemies1;
	Enemy enemy1_1(Treasure(MapPosition{}, 100), MapPosition{ 3, 3 }, "Draugr", 50.0, 5.0, 5.0, 5.0);
	enemies1.push_back(enemy1_1);

		//add level 1
	levels.push_back(Level{ map1, enemies1, treasures1 });

	//level 2
		//build map
	Map *map2 = new Map(MapPosition{ 0, 0 }, MapPosition{ 4, 0 }, 7, 7);

	vector<MapDirectionPath> roads2;
			//horizontal
	roads2.push_back(MapDirectionPath{ MapPosition{ 0, 0 }, MapPosition{ 0, 4 }, false });
	roads2.push_back(MapDirectionPath{ MapPosition{ 2, 2 }, MapPosition{ 2, 6 }, false });
	roads2.push_back(MapDirectionPath{ MapPosition{ 4, 0 }, MapPosition{ 4, 6 }, false });
			//vertical
	roads2.push_back(MapDirectionPath{ MapPosition{ 0, 2 }, MapPosition{ 4, 2 }, true });
	roads2.push_back(MapDirectionPath{ MapPosition{ 4, 2 }, MapPosition{ 6, 2 }, true });
	roads2.push_back(MapDirectionPath{ MapPosition{ 0, 4 }, MapPosition{ 2, 4 }, true });
	roads2.push_back(MapDirectionPath{ MapPosition{ 0, 6 }, MapPosition{ 4, 6 }, true });
			//build roads
	map2->buildMap(roads2);

		//add treasure
	vector<Treasure> treasures2;
	Treasure treasure2_1(MapPosition{ 0,6 }, 100);
	treasures2.push_back(treasure2_1);

		//add enemy
	vector<Enemy> enemies2;

	Enemy enemy2_1(Treasure(MapPosition{}, 100), MapPosition{ 4, 2 }, "Enemy", 10.0, 10.0, 10.0, 10.0);
	Enemy enemy2_2(Treasure(MapPosition{}, 100), MapPosition{ 1, 6 }, "Enemy", 10.0, 10.0, 10.0, 10.0);

	enemies2.push_back(enemy2_1);
	enemies2.push_back(enemy2_2);

		//add level 2
	levels.push_back(Level{ map2, enemies2, treasures2 });

	//level 3
	Map *map3 = new Map(MapPosition{ 11, 8 }, MapPosition{ 14, 0 }, 15, 9);

		//build map
	vector<MapDirectionPath> roads3;
			//horizontal
	roads3.push_back(MapDirectionPath{ MapPosition{ 11, 2 }, MapPosition{ 11, 8 }, false });
	roads3.push_back(MapDirectionPath{ MapPosition{ 13, 2 }, MapPosition{ 13, 4 }, false });
	roads3.push_back(MapDirectionPath{ MapPosition{ 9, 2 }, MapPosition{ 9, 8 }, false });
	roads3.push_back(MapDirectionPath{ MapPosition{ 4, 0 }, MapPosition{ 4, 8 }, false });
	roads3.push_back(MapDirectionPath{ MapPosition{ 2, 6 }, MapPosition{ 2, 8 }, false });
	roads3.push_back(MapDirectionPath{ MapPosition{ 0, 6 }, MapPosition{ 0, 8 }, false });
			//vertical
	roads3.push_back(MapDirectionPath{ MapPosition{ 4, 0 }, MapPosition{ 14, 0 }, true });
	roads3.push_back(MapDirectionPath{ MapPosition{ 4, 2 }, MapPosition{ 11, 2 }, true });
	roads3.push_back(MapDirectionPath{ MapPosition{ 4, 4 }, MapPosition{ 7, 4 }, true });
	roads3.push_back(MapDirectionPath{ MapPosition{ 9, 4 }, MapPosition{ 11, 4 }, true });
	roads3.push_back(MapDirectionPath{ MapPosition{ 11, 4 }, MapPosition{ 13, 4 }, true });
	roads3.push_back(MapDirectionPath{ MapPosition{ 2, 6 }, MapPosition{ 4, 6 }, true });
	roads3.push_back(MapDirectionPath{ MapPosition{ 4, 6 }, MapPosition{ 9, 6 }, true });
	roads3.push_back(MapDirectionPath{ MapPosition{ 0, 8 }, MapPosition{ 9, 8 }, true });
			//build roads
	map3->buildMap(roads3);

		//add Enemy
	vector<Enemy> enemies3;

	Enemy enemy3_1(Treasure(MapPosition{}, 100), MapPosition{ 6, 4 }, "Enemy1", 10.0, 10.0, 10.0, 10.0);
	Enemy enemy3_2(Treasure(MapPosition{}, 100), MapPosition{ 4, 8 }, "Enemy2", 10.0, 10.0, 10.0, 10.0);
	Enemy enemy3_3(Treasure(MapPosition{}, 100), MapPosition{ 12, 0 }, "Enemy3", 10.0, 10.0, 10.0, 10.0);

	enemies3.push_back(enemy3_1);
	enemies3.push_back(enemy3_2);
	enemies3.push_back(enemy3_3);

		//add Treasure
	vector<Treasure> treasures3;

	Treasure treasure3_1(MapPosition{ 7, 4 }, 100);
	Treasure treasure3_2(MapPosition{ 0, 6 }, 100);

	treasures3.push_back(treasure3_1);
	treasures3.push_back(treasure3_2);

		//add level 3
	levels.push_back(Level{ map3, enemies3, treasures3 });

	//Game Mode
	//GameMode gm(map, player, enemies, treasures);
	GameMode gm(player, levels);
}

void gameModeTest2()
{
	Player *player = new Player(MapPosition{ 0, 0 }, "Player", 100.0, 10.0, 10.0, 10.0);
	vector<Level> levels;

	//level 1
		//build map
	Map *map1 = new Map(MapPosition{ 0, 1 }, MapPosition{ 6, 2 }, 7, 5);

	vector<MapPositionWithDirections> dotsWithDirections;
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{0, 1}, false, false, false, true });
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{3, 1}, true, true, true, false });
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{3, 0}, false, true, false, false });
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{3, 3}, true, false, false, true });
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{5, 3}, true, true, true, false });
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{5, 4}, true, false, false, false });
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{5, 2}, false, true, false, true });
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{6, 2}, false, true, false, false });
	map1->buildMapViaDots(dotsWithDirections);

	map1->setUpTurnDescriptions(MAP_1_TURN_DESCRIPTIONS);

	//add treasure
	vector<Treasure> treasures1;
	Treasure treasure1_1(MapPosition{ 5, 3 }, 250);
	Treasure treasure1_2(MapPosition{ 3, 0 }, 100);
	treasures1.push_back(treasure1_1);
	treasures1.push_back(treasure1_2);

	//add enemy
	vector<Enemy> enemies1;
	Enemy enemy1_1(Treasure(MapPosition{}, 100), MapPosition{ 3, 3 }, "Draugr", 50.0, 5.0, 5.0, 5.0);
	enemies1.push_back(enemy1_1);

	//add level 1
	//levels.push_back(Level{ map1, enemies1, treasures1 });

	//level 2
	//build map
	Map *map2 = new Map(MapPosition{ 0, 0 }, MapPosition{ 4, 0 }, 7, 7);

	vector<MapPositionWithDirections> dotsWithDirections2;
	dotsWithDirections2.push_back(MapPositionWithDirections{ MapPosition{0, 0}, false, true, false, false });
	dotsWithDirections2.push_back(MapPositionWithDirections{ MapPosition{0, 4}, true, false, false, true });
	dotsWithDirections2.push_back(MapPositionWithDirections{ MapPosition{2, 2}, false, true, true, true });
	dotsWithDirections2.push_back(MapPositionWithDirections{ MapPosition{2, 6}, true, false, true, false });
	dotsWithDirections2.push_back(MapPositionWithDirections{ MapPosition{4, 0}, false, true, false, false });
	dotsWithDirections2.push_back(MapPositionWithDirections{ MapPosition{4, 6}, true, false, true, false });
	dotsWithDirections2.push_back(MapPositionWithDirections{ MapPosition{0, 2}, true, true, false, true });
	dotsWithDirections2.push_back(MapPositionWithDirections{ MapPosition{4, 2}, true, true, true, true });
	dotsWithDirections2.push_back(MapPositionWithDirections{ MapPosition{6, 2}, false, false, true, false });
	dotsWithDirections2.push_back(MapPositionWithDirections{ MapPosition{2, 4}, true, true, true, false });
	dotsWithDirections2.push_back(MapPositionWithDirections{ MapPosition{0, 6}, false, false, false, true });
	map2->buildMapViaDots(dotsWithDirections2);

	//add treasure
	vector<Treasure> treasures2;
	Treasure treasure2_1(MapPosition{ 0,6 }, 100);
	treasures2.push_back(treasure2_1);

	//add enemy
	vector<Enemy> enemies2;

	Enemy enemy2_1(Treasure(MapPosition{}, 100), MapPosition{ 4, 2 }, "Enemy", 10.0, 10.0, 10.0, 10.0);
	Enemy enemy2_2(Treasure(MapPosition{}, 100), MapPosition{ 1, 6 }, "Enemy", 10.0, 10.0, 10.0, 10.0);

	enemies2.push_back(enemy2_1);
	enemies2.push_back(enemy2_2);

	//add level 2
	//levels.push_back(Level{ map2, enemies2, treasures2 });

	//level 3
	Map *map3 = new Map(MapPosition{ 11, 8 }, MapPosition{ 14, 0 }, 15, 9);

	//build map
	vector<MapPositionWithDirections> dotsWithDirections3;
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{0, 6}, false, true, false, false });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{0, 8}, true, false, false, true });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{2, 8}, true, false, true, true });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{2, 6}, false, true, false, true });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{4, 8}, true, false, true, true });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{4, 6}, true, true, true, true });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{4, 4}, true, true, false, true });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{4, 2}, true, true, false, true });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{4, 0}, false, true, false, true });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{7, 4}, false, false, true, false });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{9, 8}, true, false, true, false });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{9, 6}, true, true, true, false });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{9, 4}, true, true, false, true });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{9, 2}, false, true, true, true });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{11, 8}, true, false, false, false });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{11, 4}, true, true, true, true });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{11, 2}, false, true, true, false });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{13, 4}, true, false, true, false });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{13, 2}, false, true, false, false });
	dotsWithDirections3.push_back(MapPositionWithDirections{ MapPosition{14, 0}, false, false, true, false });
	map3->buildMapViaDots(dotsWithDirections3);

	//add Enemy
	vector<Enemy> enemies3;

	Enemy enemy3_1(Treasure(MapPosition{}, 100), MapPosition{ 6, 4 }, "Enemy1", 10.0, 10.0, 10.0, 10.0);
	Enemy enemy3_2(Treasure(MapPosition{}, 100), MapPosition{ 4, 8 }, "Enemy2", 10.0, 10.0, 10.0, 10.0);
	Enemy enemy3_3(Treasure(MapPosition{}, 100), MapPosition{ 12, 0 }, "Enemy3", 10.0, 10.0, 10.0, 10.0);

	enemies3.push_back(enemy3_1);
	enemies3.push_back(enemy3_2);
	enemies3.push_back(enemy3_3);

	//add Treasure
	vector<Treasure> treasures3;

	Treasure treasure3_1(MapPosition{ 7, 4 }, 100);
	Treasure treasure3_2(MapPosition{ 0, 6 }, 100);

	treasures3.push_back(treasure3_1);
	treasures3.push_back(treasure3_2);

	//add level 3
	levels.push_back(Level{ map3, enemies3, treasures3 });

	//Game Mode
	GameMode gm(player, levels);
}

void gameModeTestRandomize()
{
	Player *player = new Player(MapPosition{ 0, 0 }, "Player", 100.0, 10.0, 10.0, 10.0);
	
	RandomLevelGeneratorV1 generator(20, 20);
	vector<Level> levels;
	levels.push_back(generator.getRandomLevel());

	GameMode gm(player, levels);
}

//release
void startGameMode()
{
	//select gm
	const string GM_RANDOM = "rand";
	const string GM_FIXED = "fixed";
	string userGameModePref;
	bool isRand = false;
	int ySize = 20;
	int xSize = 20;

	cout << "\n" << "  You may select random generated levels or fixed designed" << "\n";
	cout << "  __________" << "\n\n";
	cout << "  For select random generated levels, write: " << GM_RANDOM << "\n";
	cout << "  For fixed, write: " << GM_FIXED << "\n";

	while (userGameModePref != GM_RANDOM && userGameModePref != GM_FIXED)
	{
		cout << "  Your selection: ";
		cin >> userGameModePref;
		cout << "\n";

		if (userGameModePref == GM_RANDOM)
			isRand = true;
		else if (userGameModePref == GM_FIXED)
			isRand = false;
		else
			cout << "  You write incorrect string, try again" << "\n";
	}

	if (isRand)
	{
		cout << "\n" << "  Select map size, set min 20x20." << "\n";
	
		while (true)
		{
			string playerInput;
			bool isInputCorrect = true;

			int yPlayerSize;
			cout << "  Select Y size: ";
			cin >> playerInput;
			
			for (char c : playerInput)
			{
				if (!isdigit(c))
				{
					isInputCorrect = false;
					break;
				}
			}

			if (isInputCorrect)
			{
				yPlayerSize = stoi(playerInput);

				if (isInputCorrect && yPlayerSize >= 20)
				{
					ySize = yPlayerSize;

					break;
				}
				else
					cout << "  You write incorrect string, try again" << "\n";
			}
			else
				cout << "  You write incorrect string, try again" << "\n";
		}

		while (true)
		{
			string playerInput;
			bool isInputCorrect = true;

			int xPlayerSize;
			cout << "  Select X size: ";
			cin >> playerInput;

			for (char c : playerInput)
			{
				if (!isdigit(c))
				{
					isInputCorrect = false;
					break;
				}
			}

			if (isInputCorrect)
			{
				xPlayerSize = stoi(playerInput);

				if (isInputCorrect && xPlayerSize >= 20)
				{
					xSize = xPlayerSize;

					break;
				}
				else
					cout << "  You write incorrect string, try again" << "\n";
			}
			else
				cout << "  You write incorrect string, try again" << "\n";
		}
	}

	//Player
	Player *player = new Player(MapPosition{ 0, 0 }, "Player", 100.0, 10.0, 10.0, 10.0);
	
	//RANDOM LEVELS
	RandomLevelGeneratorV1 generator(ySize, xSize);
	vector<Level> randomLevels;
	randomLevels.push_back(generator.getRandomLevel());

	//STATIC LEVELS
	vector<Level> levels;

	//level 1
		//build map
	Map *map1 = new Map(MapPosition{ 0, 1 }, MapPosition{ 6, 2 }, 7, 5);

	vector<MapDirectionPath> roads1;
	roads1.push_back(MapDirectionPath{ MapPosition{ 3, 3 }, MapPosition{ 5, 3 }, true });
	roads1.push_back(MapDirectionPath{ MapPosition{ 3, 0 }, MapPosition{ 3, 3 }, false });
	roads1.push_back(MapDirectionPath{ MapPosition{ 0, 1 }, MapPosition{ 3, 1 }, true });
	roads1.push_back(MapDirectionPath{ MapPosition{ 5, 2 }, MapPosition{ 5, 4 }, false });
	vector<MapDirectionPath> &roads1L = roads1;
	//build roads
	map1->buildMap(roads1L);

	//add treasure
	vector<Treasure> treasures1;
	Treasure treasure1_1(MapPosition{ 5, 3 }, 250);
	Treasure treasure1_2(MapPosition{ 3, 0 }, 100);
	treasures1.push_back(treasure1_1);
	treasures1.push_back(treasure1_2);

	//add enemy
	vector<Enemy> enemies1;
	Enemy enemy1_1(Treasure(MapPosition{}, 100), MapPosition{ 3, 3 }, "Draugr", 50.0, 5.0, 5.0, 5.0);
	enemies1.push_back(enemy1_1);

	//add level 1
	levels.push_back(Level{ map1, enemies1, treasures1 });

	//level 2
		//build map
	Map *map2 = new Map(MapPosition{ 0, 0 }, MapPosition{ 4, 0 }, 7, 7);

	vector<MapDirectionPath> roads2;
	//horizontal
	roads2.push_back(MapDirectionPath{ MapPosition{ 0, 0 }, MapPosition{ 0, 4 }, false });
	roads2.push_back(MapDirectionPath{ MapPosition{ 2, 2 }, MapPosition{ 2, 6 }, false });
	roads2.push_back(MapDirectionPath{ MapPosition{ 4, 0 }, MapPosition{ 4, 6 }, false });
	//vertical
	roads2.push_back(MapDirectionPath{ MapPosition{ 0, 2 }, MapPosition{ 4, 2 }, true });
	roads2.push_back(MapDirectionPath{ MapPosition{ 4, 2 }, MapPosition{ 6, 2 }, true });
	roads2.push_back(MapDirectionPath{ MapPosition{ 0, 4 }, MapPosition{ 2, 4 }, true });
	roads2.push_back(MapDirectionPath{ MapPosition{ 0, 6 }, MapPosition{ 4, 6 }, true });
	//build roads
	map2->buildMap(roads2);

	//add treasure
	vector<Treasure> treasures2;
	Treasure treasure2_1(MapPosition{ 0,6 }, 100);
	treasures2.push_back(treasure2_1);

	//add enemy
	vector<Enemy> enemies2;

	Enemy enemy2_1(Treasure(MapPosition{}, 100), MapPosition{ 4, 2 }, "Enemy", 10.0, 10.0, 10.0, 10.0);
	Enemy enemy2_2(Treasure(MapPosition{}, 100), MapPosition{ 1, 6 }, "Enemy", 10.0, 10.0, 10.0, 10.0);

	enemies2.push_back(enemy2_1);
	enemies2.push_back(enemy2_2);

	//add level 2
	levels.push_back(Level{ map2, enemies2, treasures2 });

	//level 3
	Map *map3 = new Map(MapPosition{ 11, 8 }, MapPosition{ 14, 0 }, 15, 9);

	//build map
	vector<MapDirectionPath> roads3;
	//horizontal
	roads3.push_back(MapDirectionPath{ MapPosition{ 11, 2 }, MapPosition{ 11, 8 }, false });
	roads3.push_back(MapDirectionPath{ MapPosition{ 13, 2 }, MapPosition{ 13, 4 }, false });
	roads3.push_back(MapDirectionPath{ MapPosition{ 9, 2 }, MapPosition{ 9, 8 }, false });
	roads3.push_back(MapDirectionPath{ MapPosition{ 4, 0 }, MapPosition{ 4, 8 }, false });
	roads3.push_back(MapDirectionPath{ MapPosition{ 2, 6 }, MapPosition{ 2, 8 }, false });
	roads3.push_back(MapDirectionPath{ MapPosition{ 0, 6 }, MapPosition{ 0, 8 }, false });
	//vertical
	roads3.push_back(MapDirectionPath{ MapPosition{ 4, 0 }, MapPosition{ 14, 0 }, true });
	roads3.push_back(MapDirectionPath{ MapPosition{ 4, 2 }, MapPosition{ 11, 2 }, true });
	roads3.push_back(MapDirectionPath{ MapPosition{ 4, 4 }, MapPosition{ 7, 4 }, true });
	roads3.push_back(MapDirectionPath{ MapPosition{ 9, 4 }, MapPosition{ 11, 4 }, true });
	roads3.push_back(MapDirectionPath{ MapPosition{ 11, 4 }, MapPosition{ 13, 4 }, true });
	roads3.push_back(MapDirectionPath{ MapPosition{ 2, 6 }, MapPosition{ 4, 6 }, true });
	roads3.push_back(MapDirectionPath{ MapPosition{ 4, 6 }, MapPosition{ 9, 6 }, true });
	roads3.push_back(MapDirectionPath{ MapPosition{ 0, 8 }, MapPosition{ 9, 8 }, true });
	//build roads
	map3->buildMap(roads3);

	//add Enemy
	vector<Enemy> enemies3;

	Enemy enemy3_1(Treasure(MapPosition{}, 100), MapPosition{ 6, 4 }, "Enemy1", 10.0, 10.0, 10.0, 10.0);
	Enemy enemy3_2(Treasure(MapPosition{}, 100), MapPosition{ 4, 8 }, "Enemy2", 10.0, 10.0, 10.0, 10.0);
	Enemy enemy3_3(Treasure(MapPosition{}, 100), MapPosition{ 12, 0 }, "Enemy3", 10.0, 10.0, 10.0, 10.0);

	enemies3.push_back(enemy3_1);
	enemies3.push_back(enemy3_2);
	enemies3.push_back(enemy3_3);

	//add Treasure
	vector<Treasure> treasures3;

	Treasure treasure3_1(MapPosition{ 7, 4 }, 100);
	Treasure treasure3_2(MapPosition{ 0, 6 }, 100);

	treasures3.push_back(treasure3_1);
	treasures3.push_back(treasure3_2);

	//add level 3
	levels.push_back(Level{ map3, enemies3, treasures3 });

	//run gm
	if(isRand)
		GameMode gm(player, randomLevels);
	else
		GameMode gm(player, levels);
}

/*
	* Copyright (C) 2019 {Kirill Andreev}
	________
*/