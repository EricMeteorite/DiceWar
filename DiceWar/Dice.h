#pragma once
#ifndef DICE_H
#define DICE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <time.h>
#include <algorithm>

using namespace std;

class Dice
{
private:
	int diceNumber;
	bool lock = false;

public:
	Dice();
	~Dice();
	int getValue() const;
	bool determineLock() const;
	void Lock();
	void notLock();
	void rollDice();
};





#endif // !DICE_H
