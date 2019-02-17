#pragma once

#ifndef H_GAMEMODE
#define H_GAMEMODE

#include "RandomLevelGenerator.h"
#include "MapNavigationUI.h"
#include "Battle.h"

const enum GameEndAction
{
	GAME_EXIT,
	GAME_RESTART,
	GAME_NEXT_LEVEL
};

class GameMode
{
private:
	//Level
	vector<Level> levels;
	int currentLevel = 0;

	//Game variables
	Map *map;
	Player *player;
	vector<Enemy> enemies;
	vector<Treasure> treasures;
	GameEndAction gameEndAction = GAME_EXIT;

	//Parametrs
	bool isShowingMap = false;

	//Consts
	const string EXIT = "EXIT";
	const string RESTART = "RESTART";
	const string incorrectInput = "You write incorrect string, try again.";
	const string YES = "y";
	const string NO = "n";

public:
	GameMode(Map* map, Player* player, vector<Enemy> enemies, vector<Treasure> treasures);
	GameMode(Player* player, vector<Level> levels);
	~GameMode();

	//game methods & cycles
	void setUpGameMode();
	void gameBegin();
	void startMainGameCycle();
	void playerDeadAction();
	void restartGameMode();

	//level actions
	void setLevel(int number);
	
	//actions with player
	void movePlayerTo(MapPosition positionTo);
	void playerCollectTreasure(MapPosition treasurePosition);

	//fight
	void startBattle(Enemy* enemy);

	//
	int getEnemyNumByPosition(MapPosition position);

	//turn directions
	void showValidTurnDirectionsInConsole(vector<TurnDirection> turnDirections);
	void showValidTurnDirectionsInConsole(vector<TurnDirectionWithType> turnDirections);
	void showValidTurnDirectionsInConsole(vector<TurnDirectionWithType> turnDirections, MapPosition positionForDescription);

	//show game information in console
	void showBeginInformation();
	void showUnderlineInConsole();
	void showExitNotification();
	void showInvalidActionNofication();
	void showGameScore();
};

//Overload operators:
bool operator==(MapPosition pos1, MapPosition pos2);

ostream& operator<<(ostream &out, Direction direction);

#endif // !H_GAMEMODE
