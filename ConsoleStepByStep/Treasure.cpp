#include "pch.h"
#include "Treasure.h" 

//Gold
Gold::Gold(int amount)
{
	this->amount = amount;
}

	//Overload operators:
Gold operator+(Gold & gold1, Gold & gold2)
{
	return Gold(gold1.amount + gold2.amount);
}

Gold & Gold::operator+=(const Gold & gold2)
{
	this->amount += gold2.amount;
	return *this;
}

//Treasure
Treasure::Treasure(MapPosition position, int goldAmount)
{
	this->position = position;
	this->gold = Gold(goldAmount);
}

void treasureTest()
{
	Gold gold1(10);
	Gold gold2(101);

	Gold gold3 = gold1 + gold2;

	gold2 += gold3;

	cout << gold2.getAmount();
}

/*
	* Copyright (C) 2019 {Kirill Andreev}
	________
*/