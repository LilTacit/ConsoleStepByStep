#pragma once
#ifndef H_MAP_NAVIGATION
#define H_MAP_NAVIGATION

/*
Specifier object`s, needed for map navigation.
*/

using namespace std;

struct MapPosition
{
	int yPosition = 0;
	int xPosition = 0;
};

const enum Direction
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct MapPositionWithDirections
{
	MapPosition position;
	vector<Direction> directions;

	MapPositionWithDirections(MapPosition position = MapPosition{}, bool isLeft = false, bool isRight = false, bool isUp = false, bool isDown = false)
		: position(position)
	{
		if (isLeft) directions.push_back(LEFT);
		if (isRight) directions.push_back(RIGHT);
		if (isUp) directions.push_back(UP);
		if (isDown) directions.push_back(DOWN);
	}
};

struct MapDirectionPath
{
	MapPosition positionFrom, positionTo;
	bool isVertical;
};

struct TurnDirection
{
	Direction direction;
	int directionNum;
	bool isVertical;
};

const enum DirectionType
{
	Turn,
	Fight,
	COLLECT
};

struct TurnDirectionWithType
{
	DirectionType directionType;
	TurnDirection turnDirection;
};

static const struct TurnDirection LEFT_DIRECTION { Direction{ LEFT }, -1, false };
static const struct TurnDirection RIGHT_DIRECTION { Direction{ RIGHT }, 1, false };
static const struct TurnDirection UP_DIRECTION { Direction{ UP }, -1, true };
static const struct TurnDirection DOWN_DIRECTION { Direction{ DOWN }, 1, true };

static const TurnDirection TURN_DIRECTIONS[4] = { LEFT_DIRECTION, RIGHT_DIRECTION, UP_DIRECTION, DOWN_DIRECTION };

struct TurnDescription
{
	MapPosition position = MapPosition{};
	string descriptions[4] = { "", "", "", "" };

	TurnDescription(MapPosition position = MapPosition{}, string left = "", string right = "", string up = "", string down = "")
	{
		if (position.xPosition >= 0 && position.yPosition >= 0)
			this->position = position;

		descriptions[0] = left;
		descriptions[1] = right;
		descriptions[2] = up;
		descriptions[3] = down;
	}
};

static const vector<TurnDescription> MAP_1_TURN_DESCRIPTIONS = 
{
	TurnDescription{ MapPosition{0, 1}, "", "", "", "You are entered in old forgotten house and see dark straight corridor." },
	TurnDescription{ MapPosition{3, 1}, "", "Very dark here, something is seen at the end of corridor.", "I`m scared, need to go back.", "" },
	TurnDescription{ MapPosition{3, 0}, "", "There is nothing more, need to go further.", "", "" },
	TurnDescription{ MapPosition{3, 2}, "I`m scared, need to go back.", "", "", "" },
	TurnDescription{ MapPosition{3, 3}, "I`m scared... Little less, but need to go back.", "", "", "Big forgotten hall, a lot candles and broken furniture around." },
	TurnDescription{ MapPosition{4, 3}, "", "", "I forgote turn stove off, need to go back.", "" },
	TurnDescription{ MapPosition{5, 3}, "Lit Corner, little bit a breeze is blowing.", "Opened door.", "", "" },
	TurnDescription{ MapPosition{5, 4}, "There is nothing, need to go further.", "", "", "" },
	TurnDescription{ MapPosition{5, 2}, "", "Go back?", "", "At the end be visible basement." },
	TurnDescription{ MapPosition{6, 2}, "", "", "I`m scared, need to go back.", "Go to basement." },
};

/*bool operator==(MapPosition pos1, MapPosition pos2)
{
	return pos1.xPosition == pos2.xPosition && pos1.yPosition == pos2.yPosition;
}*/

#endif // !H_MAP_NAVIGATION
