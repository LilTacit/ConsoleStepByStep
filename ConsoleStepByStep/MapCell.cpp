#include "pch.h"
#include "MapCell.h"

/*MapCell::MapCell(bool isPath)
{
	//definition
	this->isPath = isPath;
	this->isTurn = false;
	this->placedCharacter = nullptr;
}*/

MapCell::MapCell(bool isPath, string leftDescription, string rightDescription, string upDescription, string downDescription)
{
	//definition
	this->isPath = isPath;
	this->isTurn = false;
	this->placedCharacter = nullptr;

	turnDescriptions[0] = leftDescription;
	turnDescriptions[1] = rightDescription;
	turnDescriptions[2] = upDescription;
	turnDescriptions[3] = downDescription;
}

void MapCell::replaceCharacter()
{
	placedCharacter = nullptr;
}

void MapCell::replaceTreasure()
{
	treasure = nullptr;
}

void MapCell::fillDirections(vector<Direction>& directions)
{
	turnDirections.clear();
	turnDirections.resize(directions.size());

	for (Direction direction : directions)
		turnDirections.push_back(direction);
}

void MapCell::fillTurnDescriptions(std::vector<string> descriptions)
{
	if (descriptions.size() == 4)
	{
		turnDescriptions[0] = descriptions[0];
		turnDescriptions[1] = descriptions[1];
		turnDescriptions[2] = descriptions[2];
		turnDescriptions[3] = descriptions[3];
	}
}

string MapCell::getTurnDescription(Direction direction)
{
	return turnDescriptions[direction];
}

MapCell::operator bool() const
{
	if (this)
	{
		return this->isPath;
	}
	else
		return false;
}

/*
	* Copyright (C) 2019 {Kirill Andreev}
	________
*/