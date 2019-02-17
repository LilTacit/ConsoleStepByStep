#pragma once
#ifndef H_MAP
#define H_MAP

#include "MapCell.h"
#include <random>
#include <ctime>
#include <cstdlib>

/*
EXAMPLE:
[
. i . .
. i . .
i i i .
. . i i
]

. - empty cell
i - path cell

	DESCRIPTOIN AND OBJECTIVE:
Map work as independent class, who don`t work with Player and GameMode in directly symbiosis.
It containts methods what work only with self.


	METHODS:
- Map(MapPosition startPosition, MapPosition endPosition, int xSize, int ySize) constrcutor:
	resize map vector to xSize+ySize (vector<vector<MapCell>>) and fill it empty MapCell`s, save start & end positions and save x & y size.
- showMap():
	out map matrix int console.
- buildPath(MapPosition pos1, MapPosition pos2, bool isVertical):
	unlock map cells from pos1 to pos2, build path vertical or horizontal by isVertical bool.
- checkValidPosition(MapPosition pos):
	check out of bounds x & y has pos(MapPosition)
*/

class Map
{
private:
	int xSize, ySize;
	vector< vector<MapCell> > map;
	MapPosition startPosition, endPosition;

public:
	Map(MapPosition startPosition, MapPosition endPosition, int ySize = 1, int xSize = 1);

	//Random map generate constructor
	Map(int ySize, int xSize);

	~Map();

	//getters
	MapPosition getStartPosition() { return startPosition; }
	MapPosition getEndPosition() { return endPosition; }

	//show map in console:
	void showMap();

	//check valid position:
	bool checkValidPosition(MapPosition pos);
	bool checkValidPosition(MapDirectionPath pathDirection);

	//build roads:
	void buildPath(MapPosition pos1, MapPosition pos2, bool isVertical = true);
	void buildPath(MapDirectionPath pathDirection);
	void buildPath2(MapPosition posFrom, MapPosition posTo);
	void buildMap(std::vector<MapDirectionPath> &roads);

	//with cell actions:
	MapCell& getLinkCellByPostion(MapPosition position);
	void placeCharacterOnPath(BasicCharacter &character, MapPosition position);
	void replaceCharacterFromPath(MapPosition pos);
	void placeTreasureOnPath(Treasure& treasure, MapPosition position);
	void setUpTurns(vector<MapPosition> turns);

	//turn directions:
	vector<TurnDirection> getValidTurnDirections(MapPosition position);
	vector<TurnDirectionWithType> getValidTurnDirectionsWithType(MapPosition position);

	//turn descriptions
	void setUpTurnDescriptions(vector<TurnDescription> descriptions);

	//calculate positionTo by turnDirection
	MapPosition getPositionToByTurnDirection(MapPosition positionFrom, TurnDirection direction);
	MapPosition getNextPositionByTurnDirection(MapPosition positionFrom, TurnDirection direction);

	//algorithms
	void buildMapViaDots(vector<MapPosition> dots);
	void buildMapViaDots(vector<MapPositionWithDirections> dots);
	void movePosition(MapPosition& position, bool isVertical, int directionNum);
	void movePositionByDirection(MapPosition& position, Direction direction);
	bool checkPathByPosition(MapPosition position);
	void placeEmptyPath(MapPosition position);
	MapPosition getRandomTurn();

	//For random generation
	bool checkEqualPosition(MapPosition pos1, MapPosition pos2);
	vector<MapPosition> getTurnsList();
	bool isNextPositionByDirectionValid(MapPosition pos, Direction direction);
	void buildPathByLength(MapPosition posFrom, Direction direction, int length);

	//randomise
	int getRandomNumberInRange(int fromI, int toI);
};

#endif // !H_MAP