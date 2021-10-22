#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <time.h>
#include <algorithm>
#include "Dice.h"

using namespace std;

Dice::Dice()
{
	diceNumber = 0;
	lock = false;
}

Dice::~Dice()
{

}

int Dice::getValue() const
{
	return diceNumber;
}

bool Dice::determineLock() const
{
	return lock;
}

void Dice::Lock()
{
	lock = true;
}

void Dice::notLock()
{
	lock = false;
}

void Dice::rollDice()//Rolling Dices for all players.
{
	if (lock)//If the dices is locked, then over.
	{
		return;
	}
	else
	{
		diceNumber = 1 + rand() % 6;//If not lock, roll dices for 1-6 value.
	}
	
}
