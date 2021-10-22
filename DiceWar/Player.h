#pragma once
#ifndef PLAYER_H
#define PLATER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <time.h>
#include <algorithm>

using namespace std;

class Player
{
private:
	string playerName;
	int playerTotalScore;
	int playerCurrentScore;
public:
	Player(string name);
	~Player();
	string getName() const;
	int getTotalScore() const;
	int getCurrentScore() const;
	void resetScore(int tempScore);
	void updateScore(int tempScore);

};





#endif // !PLAYER_H
