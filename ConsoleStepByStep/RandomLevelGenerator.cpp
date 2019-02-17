#include "pch.h"
#include "RandomLevelGenerator.h"

/*
Requirements:
	- different y & x length
	- y & x shoud be odd
*/

RandomLevelGeneratorV1::RandomLevelGeneratorV1(int ySize, int xSize)
{
	//check odd
	if (!checkOdd(ySize))
		this->ySize += 1;

	if (!checkOdd(xSize))
		this->xSize += 1;

	//check different vector length
	if (ySize == xSize)
	{
		int expandInt = rng(2, ySize / 2);
		if (!checkOdd(expandInt))
			expandInt += 1;

		if(rng(1,2) > 1)
			xSize += expandInt;
		else
			ySize += expandInt;
	}
	
	//definition sizes
	this->ySize = ySize; this->xSize = xSize; this->yHalf = this->ySize / 2; this->xHalf = this->xSize / 2;

	//random section
	int yRand = rng(0, ySize - 1);
	int xRand = rng(0, xSize - 1);
	Sections randSection = determineSection(yRand, xRand);

	//definition enter & exit positions
	enterPos = getRandomDotOnSectionBorder(randSection);
	exitPos = getRandomDotOnSectionBorder(getOppositeSection(randSection));
}

Map * RandomLevelGeneratorV1::getRandomMapPtr()
{
	Map *map = new Map(enterPos, MapPosition{ exitPos }, ySize, xSize);

	//generate random path
	generateMainPath(*map);
	generateRandomTurns(*map);

	return map;
}

bool RandomLevelGeneratorV1::checkOdd(int i)
{
	return i / 2 * 2 == i;
}

Sections RandomLevelGeneratorV1::determineSection(int y, int x)
{
	Sections sectionToReturn = ZERO;

	bool isFirstHalfY = y < yHalf;
	bool isFirstHalfX = x < xHalf;

	if (isFirstHalfY && !isFirstHalfX)
		sectionToReturn = ONE;
	else if (!isFirstHalfY && isFirstHalfX)
		sectionToReturn = TWO;
	else if (!isFirstHalfY && !isFirstHalfX)
		sectionToReturn = THREE;

	return sectionToReturn;
}

Sections RandomLevelGeneratorV1::getOppositeSection(Sections section)
{
	Sections sectionToReturn = ZERO;

	if (section == ZERO)
		sectionToReturn = THREE;
	else if (section == ONE)
		sectionToReturn = TWO;
	else if (section == TWO)
		sectionToReturn = ONE;

	return sectionToReturn;
}

MapPosition RandomLevelGeneratorV1::getRandomDotOnSectionBorder(Sections section)
{
	bool isVertical = rng(1, 2) > 1;

	int yRand = 0;
	int xRand = 0;

	if (isVertical)
	{
		//select horizontal
		if (section == ZERO || section == TWO)
			xRand = 0;
		else
			xRand = xSize - 1;

		//select vertical
		if (section == ZERO || section == ONE)
			yRand = rng(0, ySize / 2 - 1);
		else
			yRand = rng(ySize / 2, ySize - 1);
	}
	else
	{
		//select vertical
		if (section == ZERO || section == ONE)
			yRand = 0;
		else
			yRand = ySize - 1;

		//select horizontal
		if (section == ZERO || section == TWO)
			xRand = rng(0, xSize / 2 - 1);
		else
			xRand = rng(xSize / 2, xSize - 1);
	}

	return MapPosition{ yRand, xRand };
}

MapPosition RandomLevelGeneratorV1::getNextRandomPositionByDirection(Map &map, MapPosition positionFrom, Direction direction)
{
	MapPosition position = positionFrom;
	MapPosition tmpPosition = position;
	int directionI = 1;

	int i = 0;

	if (direction == LEFT || direction == RIGHT)
	{
		//horisontal
		if (direction == LEFT)
			directionI = -1;
		
		do
		{
			tmpPosition.xPosition = position.xPosition + ( directionI * (rng(1, xSize / 3)) );
			i++;
		} while (!map.checkValidPosition(tmpPosition) || i < 10);

		position = tmpPosition;
	}
	else
	{
		//vertical
		if (direction == UP)
			directionI = -1;

		do
		{
			tmpPosition.yPosition = position.yPosition + ( directionI * (rng(1, ySize / 3)) );
			i++;
		} while (!map.checkValidPosition(tmpPosition) || i < 10);

		position = tmpPosition;
	}

	return position;
}

/*bool RandomLevelGeneratorV1::checkEqualPosition(MapPosition pos1, MapPosition pos2)
{
	bool isVertical = false;
	bool isHorizontal = false;

	if (pos1.yPosition == pos2.yPosition)
		isVertical = true;

	if (pos1.xPosition == pos2.xPosition)
		isHorizontal = true;

	return isVertical * isHorizontal;
}*/

void RandomLevelGeneratorV1::cleanPathDotList(vector<MapPosition> & pathDots)
{
	vector<MapPosition> cleanList;
	int size = pathDots.size();

	MapPosition startPos = pathDots[0];
	cleanList.push_back(startPos);

	for (int i = 1; i < size; i++)
	{
		if (i == size - 1)
			break;

		//cout << "lil";

		bool isVertical = true;
		MapPosition nextDot = pathDots[i];

		if (startPos.xPosition != nextDot.xPosition)
			isVertical = false;

		//parse path rudiments
		for (int j = i; j < size; j++)
		{
			MapPosition tmpDot = pathDots[j];
			
			if (isVertical)
			{
				if (startPos.xPosition != tmpDot.xPosition)
				{
					nextDot = pathDots[j - 1];
					i = j;
					break;
				}
			}
			else
			{
				if (startPos.yPosition != tmpDot.yPosition)
				{
					nextDot = pathDots[j - 1];
					i = j;
					break;
				}
			}
		}

		startPos = nextDot;

		cleanList.push_back(nextDot);
	}

	//clear list & instant fill clean list
	pathDots.clear();
	for (MapPosition pos : cleanList)
		pathDots.push_back(pos);
}

void RandomLevelGeneratorV1::generateMainPath(Map &map)
{
	MapPosition currentDot = enterPos;
	vector<MapPosition> pathDots;
	pathDots.push_back(currentDot);

	while (!map.checkEqualPosition(currentDot, exitPos))
	{
		//validate directions
		vector<Direction> actionDirections;

		//valid vertical
		if (currentDot.yPosition < exitPos.yPosition)
		{
			if (checkValidDirection(map, currentDot, DOWN))
				actionDirections.push_back(DOWN);
		}
		else if (currentDot.yPosition > exitPos.yPosition)
		{
			if (checkValidDirection(map, currentDot, UP))
				actionDirections.push_back(UP);
		}

		//valid horizontal
		if (currentDot.xPosition < exitPos.xPosition)
		{
			if (checkValidDirection(map, currentDot, RIGHT))
				actionDirections.push_back(RIGHT);
		}
		else if (currentDot.xPosition > exitPos.xPosition)
		{
			if (checkValidDirection(map, currentDot, LEFT))
				actionDirections.push_back(LEFT);
		}

		Direction direction;

		if (rng(0, 100) > 50)
			direction = actionDirections[0];
		else
			direction = actionDirections[actionDirections.size() / 2];

		MapPosition nextDot = getNextRandomPositionByDirection(map, currentDot, direction);

		pathDots.push_back(nextDot);

		currentDot = nextDot;
	}

	pathDots.push_back(currentDot);
	cleanPathDotList(pathDots);

	for (int i = 0; i < pathDots.size()-1; i++)
	{
		MapPosition &from = pathDots[i];
		MapPosition &to = pathDots[i + 1];

		map.buildPath2(from, to);
	}

	map.setUpTurns(pathDots);
}

void RandomLevelGeneratorV1::generateRandomTurns(Map & map)
{
	//turnsAmount & turnsList
	//random turnsList
	//for turnsList buildTurns
		//rand L, R, U, D
			//for direction L, R, U, D
				//check direction nextPos
					//rand length
						//check valid length
							//buildPath

	vector<MapPosition> turnsList = map.getTurnsList();

	if (turnsList.size() > 0)
	{
		//decide turns amount
		int turnsAmount = turnsList.size();
		turnsAmount = rng(1, turnsAmount);

		vector<MapPosition> turns;

		for (int i = 0; i < turnsAmount; i++)
		{
			//fill random turns vector
			int turnNum = rng(0, turnsList.size() - 1);
			turns.push_back(turnsList[turnNum]);

			vector<MapPosition> tmpTurns;

			for (int i = 0; i < turnsList.size(); i++)
			{
				if (i != turnNum)
					tmpTurns.push_back(turnsList[i]);
			}

			turnsList.clear();
			turnsList = tmpTurns;
			tmpTurns.clear();
		}

		Direction directions[4] = { LEFT, RIGHT, UP, DOWN };

		for (MapPosition pos : turns)
		{
			int directionNum = rng(0, 3);
			bool isDirectionValid = false;

			//cycle directions with loop on the end
			for (int i = directionNum; i < 4; i++)
			{
				if (map.isNextPositionByDirectionValid(pos, directions[i]))
				{
					isDirectionValid = true;
					directionNum = i;
					break;
				}
			}

			if (!isDirectionValid)
			{
				for (int i = 0; i < 4; i++)
				{
					if (map.isNextPositionByDirectionValid(pos, directions[i]))
					{
						isDirectionValid = true;
						directionNum = i;
						break;
					}
				}
			}

			if (isDirectionValid)
			{
				//build path
				Direction direction = directions[directionNum];
				int length = rng( 1, ((ySize + xSize) / 2 / 3) );
				map.buildPathByLength(pos, direction, length);
			}
		}
	}
}

bool RandomLevelGeneratorV1::checkValidDirection(vector<Direction> validDirections, Direction direction)
{
	bool toReturn = false;

	for (Direction d : validDirections)
	{
		if (d == direction)
		{
			toReturn = true;
			break;
		}
	}

	return toReturn;
}

bool RandomLevelGeneratorV1::checkValidDirection(Map &map, MapPosition position, Direction direction)
{
	MapPosition nextPosition = position;

	if (direction == LEFT)
		nextPosition = MapPosition{ position.yPosition, position.xPosition - 1 };
	else if(direction == RIGHT)
		nextPosition = MapPosition{ position.yPosition, position.xPosition + 1 };
	else if(direction == UP)
		nextPosition = MapPosition{ position.yPosition - 1, position.xPosition };
	else if(direction == DOWN)
		nextPosition = MapPosition{ position.yPosition + 1, position.xPosition };

	return map.checkValidPosition(nextPosition);
}

Level RandomLevelGeneratorV1::getRandomLevel()
{
	//map
	Map *map = getRandomMapPtr();

			//*! Trouble
	//enemy
	vector<Enemy> enemies;
		//getRandomTurn + enemy on randomTurn
	WeakEnemy enemy(Treasure{}, map->getRandomTurn());
	enemies.push_back(enemy);

	//treasure
	vector<Treasure> treasures;
		//getRandomTurn + treasure on randomTurn
	Treasure treasure(map->getRandomTurn());
	treasures.push_back(treasure);

	return Level{ map, enemies, treasures };
}

int RandomLevelGeneratorV1::rng(int fromI, int toI)
{
	
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
	

	/*
	default_random_engine generator;
	uniform_int_distribution<int> distribution(fromI, toI);
	generator.seed(time(NULL));

	return distribution(generator);
	*/
	

	/*
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<double> dist(fromI, toI);

	return dist(mt);
	*/
}

void testRandonMapGenerateV1()
{
	RandomLevelGeneratorV1 mapGenerator = RandomLevelGeneratorV1(20, 20);

	mapGenerator.getRandomLevel();
}

/*
	* Copyright (C) 2019 {Kirill Andreev}
	________
*/