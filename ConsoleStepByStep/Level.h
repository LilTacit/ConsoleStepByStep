#pragma once

#include "BasicCharacter.h"
#include "Map.h"

using namespace std;

struct Level
{
	Map *map;
	vector<Enemy> enemies;
	vector<Treasure> treasures;
};