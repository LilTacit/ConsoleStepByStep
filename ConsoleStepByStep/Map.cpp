#include "pch.h"
#include "Map.h"
#include "MapCell.h"

//Constructors & destructors
Map::Map(MapPosition startPosition, MapPosition endPosition, int ySize, int xSize)
{
	//definition
	this->xSize = xSize; this->ySize = ySize;
	this->startPosition = startPosition; this->endPosition = endPosition;

	if (ySize > 0 && xSize > 0)
	{
		//build map matrix
		map.resize(ySize);

		for (int i = 0; i < ySize; i++)
			map[i].resize(xSize);

		for (int i = 0; i < ySize; i++)
		{
			for (int j = 0; j < xSize; j++)
				map[i][j] = MapCell(false);
		}

		//set map start & end positions
		if(checkValidPosition(startPosition) && checkValidPosition(endPosition))
		{
			//create
			map[startPosition.yPosition][startPosition.xPosition] = MapCell(true);
			map[endPosition.yPosition][endPosition.xPosition] = MapCell(true);

			//set turn
			map[startPosition.yPosition][startPosition.xPosition].setIsTurn(true);
			map[endPosition.yPosition][endPosition.xPosition].setIsTurn(true);
		}
		else
		{
			//*throw exception...
			cout << "\n" << "  ERROR: Map positions is incorrect!" << endl;
		}
	}
	else
	{
		//*throw exception...
		cout << "\n" << "  ERROR: Map size is incorrect!" << endl;
	}
}

Map::Map(int ySize, int xSize)
{
	//Random map generate
	//definition
	this->xSize = xSize; this->ySize = ySize;
	
	if (ySize > 0 && xSize > 0)
	{
		//build map matrix
		map.resize(ySize);

		for (int i = 0; i < ySize; i++)
			map[i].resize(xSize);

		for (int i = 0; i < ySize; i++)
		{
			for (int j = 0; j < xSize; j++)
				map[i][j] = MapCell(false);
		}
	}
	else
	{
		//*throw exception...
		cout << "\n" << "  ERROR: Map size is incorrect!" << endl;
	}
}

Map::~Map()
{
	for (int i = 0; i < ySize; i++)
		map[i].clear();

	map.clear();
}

//For random generation
bool Map::checkEqualPosition(MapPosition pos1, MapPosition pos2)
{
	bool isVertical = false;
	bool isHorizontal = false;

	if (pos1.yPosition == pos2.yPosition)
		isVertical = true;

	if (pos1.xPosition == pos2.xPosition)
		isHorizontal = true;

	return isVertical * isHorizontal;
}

vector<MapPosition> Map::getTurnsList()
{
	vector<MapPosition> list;

	for (int i = 0; i < ySize; i++)
	{
		for (int j = 0; j < xSize; j++)
		{
			if (map[i][j].checkTurn())
			{
				MapPosition pos{ i,j };

				if(!checkEqualPosition(startPosition, pos) && !checkEqualPosition(endPosition, pos))
					list.push_back(pos);
			}
		}
	}

	return list;
}

bool Map::isNextPositionByDirectionValid(MapPosition pos, Direction direction)
{
	bool is = false;

	MapPosition posForCheck = pos;
	
	if (direction == LEFT)
		pos.xPosition += -1;
	else if (direction == RIGHT)
		pos.xPosition += 1;
	else if (direction == UP)
		pos.yPosition += -1;
	else if (direction == DOWN)
		pos.yPosition += 1;

	return checkValidPosition(pos);
}

void Map::buildPathByLength(MapPosition posFrom, Direction direction, int length)
{
	if (checkValidPosition(posFrom))
	{
		bool isVertical = true;
		int directionI = 1;

		if (direction == LEFT || direction == RIGHT)
			isVertical = false;

		if (direction == LEFT || direction == UP)
			directionI = -1;

		int from;
		int to;

		if (isVertical)
		{
			int xPos = posFrom.xPosition;
			from = posFrom.yPosition;
			to = posFrom.yPosition + (directionI * length);

			if (directionI > 0)
			{
				for (int i = from; i <= to; i++)
				{
					if (checkValidPosition(MapPosition{ i, xPos }))
						map[i][xPos] = MapCell(true);
					else
						break;
				}
			}
			else
			{
				for (int i = from; i >= to; i--)
				{
					if (checkValidPosition(MapPosition{ i, xPos }))
						map[i][xPos] = MapCell(true);
					else
						break;
				}
			}
		}
		else
		{
			int yPos = posFrom.yPosition;
			from = posFrom.xPosition;
			to = posFrom.xPosition + (directionI * length);

			if (directionI > 0)
			{
				for (int i = from; i <= to; i++)
				{
					if (checkValidPosition(MapPosition{ yPos, i }))
						map[yPos][i] = MapCell(true);
					else
						break;
				}
			}
			else
			{
				for (int i = from; i >= to; i--)
				{
					if (checkValidPosition(MapPosition{ yPos, i }))
						map[yPos][i] = MapCell(true);
					else
						break;
				}
			}
		}
	}
}

//show map in console:
void Map::showMap()
{
	for (int i = 0; i < ySize; i++)
	{
		cout << "    ";

		for (MapCell& cell : map[i])
		{
			if (cell.checkPath())
			{
				if (&cell.getPlacedCharacter())
				{
					if (dynamic_cast<Player*>(&cell.getPlacedCharacter())) cout << "P ";
					else if (dynamic_cast<Enemy*>(&cell.getPlacedCharacter()))  cout << "E ";
				}
				else if ( &(cell.getTreasure()) )
				{
					cout << "T ";
				}
				else
					cout << "i ";
			}
			else
				cout << ". ";
		}

		cout << endl;
	}
}

//check valid position:
bool Map::checkValidPosition(MapPosition pos)
{
	if (pos.yPosition >= 0 && pos.yPosition < ySize &&
		pos.xPosition >= 0 && pos.xPosition < xSize)
		return true;
	else
		return false;
}

bool Map::checkValidPosition(MapDirectionPath pathDirection)
{
	if (checkValidPosition(pathDirection.positionFrom) && checkValidPosition(pathDirection.positionTo))
		return true;
	else
		return false;
}

//build roads:
void Map::buildPath(MapPosition pos1, MapPosition pos2, bool isVertical)
{
	if (checkValidPosition(pos1) && checkValidPosition(pos2))
	{
		int posFrom, posTo;

		//build road
		if (isVertical)
		{
			int xPos = pos1.xPosition;
			posFrom = pos1.yPosition;
			posTo = pos2.yPosition + 1;

			for (int i = posFrom; i < posTo; i++)
			{
				if (!map[i][xPos].checkPath())
					map[i][xPos] = MapCell(true);
			}
		}
		else
		{
			int yPos = pos1.yPosition;
			posFrom = pos1.xPosition;
			posTo = pos2.xPosition + 1;

			for (int i = posFrom; i < posTo; i++)
			{
				if (!map[i][yPos].checkPath())
					map[yPos][i] = MapCell(true);
			}
		}
	}
	else
	{
		//*throw exception...
		cout << "\n" << "  ERROR: Map positions is incorrect!" << endl;
	}
}

void Map::buildPath(MapDirectionPath pathDirection)
{
	if (checkValidPosition(pathDirection))
	{
		int posFrom, posTo;

		if (pathDirection.isVertical)
		{
			int xPos = pathDirection.positionFrom.xPosition;
			posFrom = pathDirection.positionFrom.yPosition;
			posTo = pathDirection.positionTo.yPosition + 1;

			for (int i = posFrom; i < posTo; i++)
				map[i][xPos] = MapCell(true);
		}
		else
		{
			int yPos = pathDirection.positionFrom.yPosition;
			posFrom = pathDirection.positionFrom.xPosition;
			posTo = pathDirection.positionTo.xPosition + 1;

			for (int i = posFrom; i < posTo; i++)
				map[yPos][i] = MapCell(true);
		}
	}
	else
	{
		//*throw exception...
		cout << "\n" << "  ERROR: Map positions is incorrect!" << endl;
	}
}

void Map::buildPath2(MapPosition posFrom, MapPosition posTo)
{
	if (checkValidPosition(posFrom) && checkValidPosition(posTo))
	{
		bool isVertical = true;
		int directionI = 1;
		int from, to;

		if (posFrom.yPosition > posTo.yPosition || posFrom.yPosition < posTo.yPosition)
		{
			if (posFrom.yPosition > posTo.yPosition)
				directionI = -1;
		}
		else if (posFrom.xPosition > posTo.xPosition || posFrom.xPosition < posTo.xPosition)
		{
			isVertical = false;

			if (posFrom.xPosition > posTo.xPosition)
				directionI = -1;
		}

		if (isVertical)
		{
			int xPos = posFrom.xPosition;
			from = posFrom.yPosition;
			to = posTo.yPosition + directionI;

			if (directionI > 0)
			{
				for (int i = from; i < to; i++)
					map[i][xPos] = MapCell(true);
			}
			else
			{
				for (int i = from; i > to; i--)
					map[i][xPos] = MapCell(true);
			}
		}
		else
		{
			int yPos = posFrom.yPosition;
			from = posFrom.xPosition;
			to = posTo.xPosition + directionI;

			if (directionI > 0)
			{
				for (int i = from; i < to; i++)
					map[yPos][i] = MapCell(true);
			}
			else
			{
				for (int i = from; i > to; i--)
					map[yPos][i] = MapCell(true);
			}
		}
	}
	else
	{
		//*throw exception...
		cout << "\n" << "  ERROR: Map positions is incorrect!" << endl;
	}
}

void Map::buildMap(vector<MapDirectionPath>& roads)
{
	for (MapDirectionPath directionPath : roads)
		buildPath(directionPath);

	for (MapDirectionPath path : roads)
	{
		map[path.positionFrom.yPosition][path.positionFrom.xPosition].setIsTurn(true);
		map[path.positionTo.yPosition][path.positionTo.xPosition].setIsTurn(true);
	}
}

//with cell actions:
MapCell & Map::getLinkCellByPostion(MapPosition position)
{
	if (checkValidPosition(position))
	{
		MapCell &returnCell = map[position.yPosition][position.xPosition];
		
		return returnCell;
	}
	else
	{
		// *throw exception...
		cout << "\n" << "  Error: map position is incorrect! You take empty MapCell!" << endl;

		MapCell emptyCell;
		MapCell &lCell = emptyCell;

		return lCell;
	}
}

void Map::placeCharacterOnPath(BasicCharacter &character, MapPosition position)
{
	if (checkValidPosition(position))
	{
		if (&character)
		{
			MapCell &cell = getLinkCellByPostion(position);

			if (cell.checkPath())
			{
				cell.setPlacedCharacter(character);
				character.setCurrentPosition(position);
			}
		}
	}
}

void Map::replaceCharacterFromPath(MapPosition pos)
{
	if (checkValidPosition(pos))
	{
		MapCell &cell = getLinkCellByPostion(pos);

		if (cell.checkPath())
			cell.replaceCharacter();
	}
}

void Map::placeTreasureOnPath(Treasure & treasure, MapPosition position)
{
	if (checkValidPosition(position))
	{
		if (&treasure)
		{
			MapCell &cell = getLinkCellByPostion(position);

			if (cell.checkPath())
				cell.setTreasure(treasure);
		}
	}
}

void Map::setUpTurns(vector<MapPosition> turns)
{
	for (MapPosition pos : turns)
	{
		if(checkValidPosition(pos) && checkPathByPosition(pos))
			map[pos.yPosition][pos.xPosition].setIsTurn(true);
	}
}

//turn directions:
vector<TurnDirection> Map::getValidTurnDirections(MapPosition position)
{
	vector<TurnDirection> returnDirections;

	if (checkValidPosition(position))
	{
		for (TurnDirection turnDirection : TURN_DIRECTIONS)
		{
			MapPosition positionOnTurn;

			if (turnDirection.isVertical)
			{
				positionOnTurn.yPosition = position.yPosition + turnDirection.directionNum;
				positionOnTurn.xPosition = position.xPosition;
			}
			else
			{
				positionOnTurn.yPosition = position.yPosition;
				positionOnTurn.xPosition = position.xPosition + turnDirection.directionNum;
			}

			if (checkValidPosition(positionOnTurn))
				if (getLinkCellByPostion(positionOnTurn).checkPath()) returnDirections.push_back(turnDirection);
		}
	}

	return returnDirections;
}

vector<TurnDirectionWithType> Map::getValidTurnDirectionsWithType(MapPosition position)
{
	vector<TurnDirectionWithType> returnDirections;

	if (checkValidPosition(position))
	{
		for (TurnDirection turnDirection : TURN_DIRECTIONS)
		{
			MapPosition positionOnTurn;

			if (turnDirection.isVertical)
			{
				positionOnTurn.yPosition = position.yPosition + turnDirection.directionNum;
				positionOnTurn.xPosition = position.xPosition;
			}
			else
			{
				positionOnTurn.yPosition = position.yPosition;
				positionOnTurn.xPosition = position.xPosition + turnDirection.directionNum;
			}

			if (checkValidPosition(positionOnTurn))
			{
				MapCell cell = getLinkCellByPostion(positionOnTurn);

				if (cell.checkPath())
				{
					if (&cell.getPlacedCharacter())
					{
						if (dynamic_cast<Enemy*>(&cell.getPlacedCharacter())) 
							returnDirections.push_back(TurnDirectionWithType{ Fight, turnDirection });
					}
					else if ( &(cell.getTreasure()) )
					{
						returnDirections.push_back(TurnDirectionWithType{ COLLECT, turnDirection });
					}
					else
						returnDirections.push_back(TurnDirectionWithType{ Turn, turnDirection });
				}
			}
		}
	}

	return returnDirections;
}

void Map::setUpTurnDescriptions(vector<TurnDescription> descriptions)
{
	for (TurnDescription turnDescription : descriptions)
	{
		vector<string> turnDescriptions;

		turnDescriptions.push_back(turnDescription.descriptions[0]);
		turnDescriptions.push_back(turnDescription.descriptions[1]);
		turnDescriptions.push_back(turnDescription.descriptions[2]);
		turnDescriptions.push_back(turnDescription.descriptions[3]);

		MapCell &cell = getLinkCellByPostion(turnDescription.position);

		cell.fillTurnDescriptions(turnDescriptions);
	}
}

//Calculate positionTo by turnDirection
MapPosition Map::getPositionToByTurnDirection(MapPosition positionFrom, TurnDirection direction)
{
	MapPosition positionToReturn = positionFrom;

	if (direction.isVertical)
	{
		int i = positionFrom.yPosition;
		i += direction.directionNum;

		while (i < ySize && i >= 0)
		{
			MapCell cell = map[i][positionFrom.xPosition];
			
			if (cell.checkPath() && !(&cell.getPlacedCharacter()) && !(&cell.getTreasure()) )
			{
				positionToReturn = MapPosition{ i, positionFrom.xPosition };
				if (cell.checkTurn())
					break;
			}
			else
				break;
			i += direction.directionNum;
		}
	}
	else
	{
		int i = positionFrom.xPosition;
		i += direction.directionNum;

		while (i < xSize && i >= 0)
		{
			MapCell cell = map[positionFrom.yPosition][i];

			if (cell.checkPath() && !(&cell.getPlacedCharacter()) && !(&cell.getTreasure()) )
			{
				positionToReturn = MapPosition{ positionFrom.yPosition, i };
				if (cell.checkTurn())
					break;
			}
			else
				break;
			i += direction.directionNum;
		}
	}

	return positionToReturn;
}

MapPosition Map::getNextPositionByTurnDirection(MapPosition positionFrom, TurnDirection direction)
{
	MapPosition positionToReturn = positionFrom;

	if (direction.isVertical)
		positionToReturn.yPosition += direction.directionNum;
	else
		positionToReturn.xPosition += direction.directionNum;

	if (!checkValidPosition(positionToReturn) && !getLinkCellByPostion(positionToReturn).checkPath())
		positionToReturn = positionFrom;

	return positionToReturn;
}

//Algorithms
void Map::buildMapViaDots(vector<MapPosition> dots)
{
	for (MapPosition dot : dots)
	{
		//set dots
		if (checkValidPosition(dot))
		{
			placeEmptyPath(dot);
			getLinkCellByPostion(dot).setIsTurn(true);
		}
	}

	for (MapPosition dot : dots)
	{
		//build roads
		MapPosition currentPosition = dot;

		if (checkValidPosition(dot))
		{
			for (int i = -1; i <= 1; i += 2)
			{
				bool isVertical = false;

				do
				{
					currentPosition = dot;
					movePosition(currentPosition, isVertical, i);

					bool pathExist = false;

					while (checkValidPosition(currentPosition))
					{
						//if (!checkPathByPosition(currentPosition))
						//	placeEmptyPath(currentPosition);
						//else
						//	break;

						if (checkPathByPosition(currentPosition))
						{
							pathExist = true;
							break;
						}

						movePosition(currentPosition, isVertical, i);
					}

					if(pathExist)
						buildPath(MapDirectionPath{dot, currentPosition, isVertical});

					isVertical = !isVertical;
				} while (isVertical);
			}
		}
	}
}

void Map::buildMapViaDots(vector<MapPositionWithDirections> dots)
{
	for (MapPositionWithDirections dot : dots)
	{
		//set dots
		if (checkValidPosition(dot.position))
		{
			placeEmptyPath(dot.position);
			getLinkCellByPostion(dot.position).setIsTurn(true);
		}
	}

	for (MapPositionWithDirections dot : dots)
	{
		MapPosition dotPosition = dot.position;

		for (Direction direction : dot.directions)
		{
			MapPosition currentPosition = dotPosition;
			movePositionByDirection(currentPosition, direction);

			while (checkValidPosition(currentPosition))
			{
				if (!checkPathByPosition(currentPosition))
					placeEmptyPath(currentPosition);
				else
					break;

				movePositionByDirection(currentPosition, direction);
			}
		}
	}
}

void Map::movePosition(MapPosition & position, bool isVertical, int directionNum)
{
	if (isVertical)
		position.yPosition = position.yPosition + directionNum;
	else
		position.xPosition = position.xPosition + directionNum;
}

void Map::movePositionByDirection(MapPosition & position, Direction direction)
{
	bool isVertical = false;
	int directionNum = -1;

	if (direction == RIGHT)
		directionNum = 1;
	else if (direction == UP)
		isVertical = true;
	else if (direction == DOWN)
	{
		isVertical = true;
		directionNum = 1;
	}

	movePosition(position, isVertical, directionNum);
}

bool Map::checkPathByPosition(MapPosition position)
{
	if (checkValidPosition(position))
		return map[position.yPosition][position.xPosition].checkPath();
	else
		return false;
}

void Map::placeEmptyPath(MapPosition position)
{
	if (checkValidPosition(position))
		map[position.yPosition][position.xPosition] = MapCell(true);
}

MapPosition Map::getRandomTurn()
{
	vector<MapPosition> randomTurns;

	while (randomTurns.size() == 0)
	{
		for (int i = 0; i < ySize; i++)
		{
			for (int j = 0; j < xSize; j++)
			{
				MapCell cell = map[i][j];
				MapPosition pos = MapPosition{ i,j };

				if (cell.checkTurn())
				{
					if (!checkEqualPosition(startPosition, pos) && !checkEqualPosition(endPosition, pos))
						randomTurns.push_back(pos);
				}
			}
		}
	}
	
	return randomTurns[getRandomNumberInRange(0, randomTurns.size()-1)];
}

int Map::getRandomNumberInRange(int fromI, int toI)
{
	//default_random_engine generator;
	//uniform_int_distribution<int> distribution(fromI, toI);
	//generator.seed(time(NULL));
	//
	//return distribution(generator);

	int i;

	srand((int)time(NULL));
	i = (rand() % toI) + fromI;
	srand((int)time(NULL));

	if (((rand() % toI) + fromI) == i)
	{
		default_random_engine generator;
		uniform_int_distribution<int> distribution(fromI, toI);
		generator.seed(time(NULL));

		i = distribution(generator);
		generator.seed(time(NULL));

		if (distribution(generator) == i)
		{
			random_device rd;
			mt19937 mt(rd());
			uniform_real_distribution<double> dist(fromI, toI);

			i = dist(mt);
		}
	}

	return i;
}

//*** Test ***
void mapTest()
{
	//LEVEL 1
	/*
	Map map(MapPosition{ 0, 1 }, MapPosition{ 6, 2 }, 7, 5);

		//build map:
	vector<MapDirectionPath> roads;
	roads.push_back(MapDirectionPath{ MapPosition{ 3, 3 }, MapPosition{ 5, 3 }, true });
	roads.push_back(MapDirectionPath{ MapPosition{ 3, 0 }, MapPosition{ 3, 3 }, false });
	roads.push_back(MapDirectionPath{ MapPosition{ 0, 1 }, MapPosition{ 3, 1 }, true });
	roads.push_back(MapDirectionPath{ MapPosition{ 5, 2 }, MapPosition{ 5, 4 }, false });
	vector<MapDirectionPath> &roadsL = roads;
		//build roads
	map.buildMap(roadsL);

		//place Player
	Player player(MapPosition{0, 1}, "Player", 100.0, 10.0, 10.0, 10.0);
	map.placeCharacterOnPath(player, MapPosition{ 0, 1 });

		//place Enemy
	Enemy enemy(Treasure(MapPosition{}, 100), MapPosition{ 3, 3 }, "Enemy", 100.0, 10.0, 10.0, 10.0);
	map.placeCharacterOnPath(enemy, MapPosition{ 3, 3 });
	*/

	//LEVEL 2
	
	Map map(MapPosition{ 0, 0 }, MapPosition{ 4, 0 }, 7, 7);

		//build map:
	vector<MapDirectionPath> roads;
			//horizontal
	roads.push_back(MapDirectionPath{ MapPosition{ 0, 0 }, MapPosition{ 0, 4 }, false });
	roads.push_back(MapDirectionPath{ MapPosition{ 2, 2 }, MapPosition{ 2, 6 }, false });
	roads.push_back(MapDirectionPath{ MapPosition{ 4, 0 }, MapPosition{ 4, 6 }, false });
			//vertical
	roads.push_back(MapDirectionPath{ MapPosition{ 0, 2 }, MapPosition{ 4, 2 }, true });
	roads.push_back(MapDirectionPath{ MapPosition{ 4, 2 }, MapPosition{ 6, 2 }, true });
	roads.push_back(MapDirectionPath{ MapPosition{ 0, 4 }, MapPosition{ 2, 4 }, true });
	roads.push_back(MapDirectionPath{ MapPosition{ 0, 6 }, MapPosition{ 4, 6 }, true });
			//build roads
	map.buildMap(roads);

		//place Player
	Player player(MapPosition{ 0, 0 }, "Player", 100.0, 10.0, 10.0, 10.0);
	map.placeCharacterOnPath(player, MapPosition{ 0, 0 });

		//place Enemy
	Enemy enemy(Treasure(MapPosition{}, 100), MapPosition{ 4, 2 }, "Enemy", 100.0, 10.0, 10.0, 10.0);
	map.placeCharacterOnPath(enemy, MapPosition{ 4, 2 });

	Enemy enemy2(Treasure(MapPosition{}, 100), MapPosition{ 4, 2 }, "Enemy", 100.0, 10.0, 10.0, 10.0);
	map.placeCharacterOnPath(enemy2, MapPosition{ 1, 6 });

		//place Treasure
	Treasure treasure(MapPosition{0,6}, 100);
	map.placeTreasureOnPath(treasure, MapPosition{ 0,6 });
	

	//LEVEL 3
	/*
	Map map(MapPosition{ 11, 8 }, MapPosition{ 14, 0 }, 15, 9);

		//build map:
	vector<MapDirectionPath> roads;
			//horizontal
	roads.push_back(MapDirectionPath{ MapPosition{ 11, 2 }, MapPosition{ 11, 8 }, false });
	roads.push_back(MapDirectionPath{ MapPosition{ 13, 2 }, MapPosition{ 13, 4 }, false });
	roads.push_back(MapDirectionPath{ MapPosition{ 9, 2 }, MapPosition{ 9, 8 }, false });
	roads.push_back(MapDirectionPath{ MapPosition{ 4, 0 }, MapPosition{ 4, 8 }, false });
	roads.push_back(MapDirectionPath{ MapPosition{ 2, 6 }, MapPosition{ 2, 8 }, false });
	roads.push_back(MapDirectionPath{ MapPosition{ 0, 6 }, MapPosition{ 0, 8 }, false });
			//vertical
	roads.push_back(MapDirectionPath{ MapPosition{ 4, 0 }, MapPosition{ 14, 0 }, true });
	roads.push_back(MapDirectionPath{ MapPosition{ 4, 2 }, MapPosition{ 11, 2 }, true });
	roads.push_back(MapDirectionPath{ MapPosition{ 4, 4 }, MapPosition{ 7, 4 }, true });
	roads.push_back(MapDirectionPath{ MapPosition{ 9, 4 }, MapPosition{ 11, 4 }, true });
	roads.push_back(MapDirectionPath{ MapPosition{ 11, 4 }, MapPosition{ 13, 4 }, true });
	roads.push_back(MapDirectionPath{ MapPosition{ 2, 6 }, MapPosition{ 4, 6 }, true });
	roads.push_back(MapDirectionPath{ MapPosition{ 4, 6 }, MapPosition{ 9, 6 }, true });
	roads.push_back(MapDirectionPath{ MapPosition{ 0, 8 }, MapPosition{ 9, 8 }, true });
		//build roads
	map.buildMap(roads);

		//place Player
	Player player(MapPosition{ 0, 0 }, "Player", 100.0, 10.0, 10.0, 10.0);
	map.placeCharacterOnPath(player, MapPosition{ 11, 8 });

		//place Enemy
	Enemy enemy(Treasure(MapPosition{}, 100), MapPosition{ 4, 2 }, "Enemy", 100.0, 10.0, 10.0, 10.0);
	map.placeCharacterOnPath(enemy, MapPosition{ 6, 4 });

	Enemy enemy2(Treasure(MapPosition{}, 100), MapPosition{ 4, 2 }, "Enemy", 100.0, 10.0, 10.0, 10.0);
	map.placeCharacterOnPath(enemy2, MapPosition{ 4, 8 });

	Enemy enemy3(Treasure(MapPosition{}, 100), MapPosition{ 4, 2 }, "Enemy", 100.0, 10.0, 10.0, 10.0);
	map.placeCharacterOnPath(enemy3, MapPosition{ 12, 0 });

		//place Treasure
	Treasure treasure(MapPosition{ 7, 4 }, 100);
	map.placeTreasureOnPath(treasure, MapPosition{ 7, 4 });

	Treasure treasure2(MapPosition{ 0, 6 }, 100);
	map.placeTreasureOnPath(treasure2, MapPosition{ 0, 6 });
	*/

	map.showMap();
}

void mapTestDotAlgorithm()
{
	Map map(MapPosition{ 0, 0 }, MapPosition{ 4, 0 }, 7, 7);

	//build
	vector<MapPosition> dots;
	dots.push_back(MapPosition{0, 0});
	dots.push_back(MapPosition{0, 4});
	dots.push_back(MapPosition{2, 2});
	dots.push_back(MapPosition{2, 6});
	dots.push_back(MapPosition{4, 0});
	dots.push_back(MapPosition{4, 6});
	dots.push_back(MapPosition{0, 2});
	dots.push_back(MapPosition{4, 2});
	dots.push_back(MapPosition{6, 2});
	dots.push_back(MapPosition{2, 4});
	dots.push_back(MapPosition{0, 6});

	//map.buildMapViaDots(dots);

	vector<MapPositionWithDirections> dotsWithDirections;
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{0, 0}, false, true, false, false });
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{0, 4}, true, false, false, true });
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{2, 2}, false, true, true, true });
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{2, 6}, true, false, true, false });
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{4, 0}, false, true, false, false });
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{4, 6}, true, false, true, false });
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{0, 2}, true, true, false, true });
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{4, 2}, true, true, true, true });
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{6, 2}, false, false, true, false });
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{2, 4}, true, true, true, false });
	dotsWithDirections.push_back(MapPositionWithDirections{ MapPosition{0, 6}, false, false, false, true });

	map.buildMapViaDots(dotsWithDirections);

	//show
	map.showMap();
}

void mapSubTest()
{
	Map *map = new Map (MapPosition{ 0,0 }, MapPosition{ 9,9 }, 10, 10);

	MapCell cell = map->getLinkCellByPostion(MapPosition{ 0,0 });

	cout << cell.checkTurn();
}

/*
	* Copyright (C) 2019 {Kirill Andreev}
	________
*/