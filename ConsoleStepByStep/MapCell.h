#pragma once
#ifndef H_MAP_CELL
#define H_MAP_CELL

#include "BasicCharacter.h"
#include "Treasure.h"

using namespace std;

/*
	DESCRIPTION AND OBJECTIVE:
Bit of a Map. Independent class.
Contains methods only work with self.

In point it`s container, containing bit of a map information.
*/

class MapCell
{
private:
	bool isPath, isTurn;
	BasicCharacter *placedCharacter = nullptr;
	vector<Direction> turnDirections;
	Treasure *treasure = nullptr;
	vector<string> turnDescriptions = { "", "", "", "" };

public:
	MapCell(bool isPath = false,
		string leftDescription = "", string rightDescription = "", string upDescription = "", string downDescription = "");

	//getter
	bool checkPath() { return isPath; }
	bool checkTurn() { return isTurn; }
	BasicCharacter& getPlacedCharacter() { return *placedCharacter; }
	Treasure& getTreasure() { return *treasure; }
	//setters
	void setPlacedCharacter(BasicCharacter& character) { placedCharacter = &character; }
	void setIsTurn(bool is = false) { isTurn = is; }
	void setTreasure(Treasure& treasure) { this->treasure = &treasure; }

	//replace
	void replaceCharacter();
	void replaceTreasure();
	
	//Turn directions
	void fillDirections(std::vector<Direction> &directions);
	void fillTurnDescriptions(std::vector<string> descriptions);
	string getTurnDescription(Direction direction);

	//overloads operators
	operator bool() const;

};

#endif // !H_MAP_CELL