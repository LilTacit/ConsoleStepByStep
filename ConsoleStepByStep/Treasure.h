#pragma once
#ifndef H_TREASURE
#define H_TREASURE

#include "MapNavigation.h"

/*
Treasure 
	- main container class for all treasures in game.

Gold
	- part of treasure class and main score object for game.
*/

using namespace std;

class Gold
{
private:
	int amount = 0;

public:
	Gold(int amount = 0);

	//getters
	int getAmount() { return amount; }

	//overload operators
	friend Gold operator+(Gold& gold1, Gold& gold2);
	Gold& operator+=(const Gold& gold2);
};

class Treasure
{
private:
	Gold gold = Gold();
	MapPosition position = MapPosition{};

public:
	Treasure(MapPosition position = {0,0}, int goldAmount = 0);

	//getters
	MapPosition getPosition() { return position; }
	Gold getGold() { return gold; }

	//console attentions
	void showTreasure();
};

#endif // !H_TREASURE
