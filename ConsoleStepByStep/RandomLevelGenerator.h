#pragma once

#ifndef RANDOM_MAP_GENERATOR
#define RANDOM_MAP_GENERATOR

#include "Level.h"

enum Sections
{
	ZERO, ONE, TWO, THREE
};

class RandomLevelGeneratorV1
{
private:
	int ySize, xSize, yHalf, xHalf;
	MapPosition enterPos, exitPos;

	//Numbers
	bool checkOdd(int i);

	//Sections
	Sections determineSection(int y, int x);
	Sections getOppositeSection(Sections section);

	//MapPositions
	MapPosition getRandomDotOnSectionBorder(Sections section);
	MapPosition getNextRandomPositionByDirection(Map &map, MapPosition positionFrom, Direction direction);
	//bool checkEqualPosition(MapPosition pos1, MapPosition pos2);
	void cleanPathDotList(vector<MapPosition> & pathDots);

	//Directions
	bool checkValidDirection(vector<Direction> validDirections, Direction direction);
	bool checkValidDirection(Map &map, MapPosition position, Direction direction);

	//Random numbers
	int rng(int fromI, int toI);

	//Map
	Map * getRandomMapPtr();

	//Build random path
	void generateMainPath(Map &map);
	void generateRandomTurns(Map &map);

public:
	RandomLevelGeneratorV1(int ySize = 10, int xSize = 14);
	
	Level getRandomLevel();
};

#endif // !RANDOM_MAP_GENERATOR