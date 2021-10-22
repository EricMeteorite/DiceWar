#pragma once
#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <time.h>
#include <map>
#include <algorithm>
#include "Player.h"
#include "Dice.h"

using namespace std;

class Game
{
private:
	void displayAbout(string filename);
	bool playGame(Player& playCall);
	void playLoop();
	//-----------------------------------//Display 
	void displayPlayerInfo(const Player& playCall);
	void checkResult(string PlayerName);
	void displayDice(const vector<Dice> &showDice);
	//-----------------------------------//Input 
	void infoInput();
	char askCharInput(string inputRequire);
	int askIntInput(string inputRequire);
	string askStrInput(string inputRequire);
	//-----------------------------------//Game mechanics
	bool rollingDice(Player& playCall);
	int calculateScore(const vector<Dice> &vectorDice);
	bool farkleDice(const vector<Dice>& vectorDice);
	bool currentInputValue(vector<Dice>& vectorDice, int numChoose, int &calScore);
	void temporaryScore(Player& playCall);
	//-----------------------------------//Save and Load the result
	void saveData(string Playername);
	void delResult();
	void displayResult();
	//-----------------------------------//AI mechanics
	bool AI(Player& playCall);
	int displayAIInfo(const Player& playCall);
	int chooseDiceAI(const vector<Dice>& vectorDice);
	bool checkPrepare(Player& playCall);
	//-----------------------------------//Player List and Player to Dice <map> Type
	vector<Player> playerList;
	map<string, vector<Dice> > playerDice;
	//-----------------------------------
	int countFarkled = 0;//Calculate the player farkled times.
	int countAIFarkled = 0;//Calculate the Computer farkled times.
	bool farkledScore = false;
	int Enum = 0;//Enumerated parameters
	

public:
	void gameMenu();

};





#endif // !GAME_H
