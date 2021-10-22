#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <map>
#include <time.h>
#include <algorithm>
#include <sstream>
#include "Game.h"
#include "Player.h"
#include "Dice.h"

using namespace std;

void Game::gameMenu()
{
	system("cls");
	playerList.clear();//Clear vector playerList.
	playerDice.clear();//Clear vector playerDice.
	countFarkled = 0;//Reset farkled times.
	countAIFarkled = 0;//Reset AI farkled times.
	srand((int)time(NULL));
	int choice = 0;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "                                   Dice War                                     \n";
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "   ---------       ---------       ---------       ---------       ---------    \n";
	cout << "   [1]Start        [2]About         [3]Load        [4]Delete        [5]Exit     \n";
	cout << "   ---------       ---------       ---------       ---------       ---------    \n";
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	choice = askIntInput("Please choose the number to select: ");
	switch (choice)
	{
	case 1:
		playLoop();
		break;
	case 2:
		displayAbout("diceWarsAbout.txt");
		break;
	case 3:
		displayResult();
		break;
	case 4:
		delResult();
		break;
	case 5:
		cout << "Thank you to play Dice Wars!!\n";
		system("pause");
		return;
	default:
		break;

	}
	return gameMenu();
}

void Game::playLoop()
{
	system("cls");
	infoInput();//Input information for each player.
	vector<Player>::iterator iterPlayer;// Create a iterator to point playerList.
	iterPlayer = playerList.begin();//Point the first player.
	while (true)
	{
		farkledScore = false;
		if ((playGame(*iterPlayer)))//PlayGame loop for each player, return true or false.
		{
			string PlayerName = iterPlayer->getName();
			checkResult(PlayerName);//If return true, display the winner.
			saveData(PlayerName);//Saved data.
			break;
		}
		if (countFarkled >= 3)
		{
			cout << "Sorry! " << iterPlayer->getName() << " Farkled 3 times! You lose!\n";//If a player farkled 3 times.
			system("pause");
			iterPlayer = playerList.erase(iterPlayer);//This player become a loser.
			if (playerList.size() == 1)
			{
				countFarkled = 0;
				checkResult(iterPlayer->getName());
				saveData(iterPlayer->getName());
				break;
			}
			iterPlayer--;
			countFarkled = 0;//Farkled times clear.
		}
		iterPlayer++;//Point to the next player.
		if (playerList.end() == iterPlayer)//If point to the last player, then return to the first player.
		{
			iterPlayer = playerList.begin();
		}
		if (iterPlayer->getName() == "Computer")//If the player name is "Computer", then goto AI function.
		{
			if ((AI(*iterPlayer)))//This function return true or false.
			{
				string PlayerName = iterPlayer->getName();
				checkResult(PlayerName);//If return true, then Computer is the winner!
				saveData(PlayerName);//Saved data
				break;
			}
			if (countAIFarkled >= 3)//If Computer farkled 3 times
			{
				cout << "Sorry! " << iterPlayer->getName() << " Farkled 3 times! You lose!\n";
				system("pause");
				iterPlayer = playerList.erase(iterPlayer);//Then, the computer is a loser.
				iterPlayer--;
				countAIFarkled = 0;
				checkResult(iterPlayer->getName());//Player1 is winner.
				saveData(iterPlayer->getName());
				break;
			}
			iterPlayer++;
		}
		if (playerList.size() == 1)//For multiplayer, if there is only one player, then that player is winning.
		{
			checkResult(iterPlayer->getName());
			saveData(iterPlayer->getName());
			break;
		}
		if (playerList.end() == iterPlayer)//If point to the last player, then return to the first player.
		{
			iterPlayer = playerList.begin();
		}
	}
}

void Game::checkResult(string Playername)
{
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "######################## Dice Wars ############################\n";
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "#### The Winner is " << Playername << "!" << "####\n";
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	system("pause");
}

bool Game::playGame(Player& playCall)
{
	system("cls");
	bool checkScore = true;//To check the Score 5000 or not.
	while (true)
	{
		system("cls");
		displayPlayerInfo(playCall);//Display the player information.
		char inputChar = askCharInput("You can choose [q]uit, [b]ank or [r]oll: ");//Player can choose Quit, Bank or Roll.
		while (inputChar != 'b' && inputChar != 'r' && inputChar != 'q')
		{
			inputChar = askCharInput("Please enter again! [q]uit, [b]ank or [r]oll: ");
		}
		if (inputChar == 'r')
		{
			if (!rollingDice(playCall))//Rolling dice function is return true or false.
			{
				temporaryScore(playCall);//If return false, that means the player is farkled or there are NO dices can roll, and then, get the score.
				break;
			}
		}
		else if (inputChar == 'b')
		{
			temporaryScore(playCall);//Just get the score.
			break;
		}
		else if (inputChar == 'q')
		{
			cout << "Thank you for play DiceWars!" << endl;
			system("pause");
			exit(0);
		}

	}
	system("pause");
	if (!(playCall.getTotalScore() >= 5000))
	{
		checkScore = false;
	}

	return checkScore;
}

void Game::displayAbout(string filename)
{
	string str;
	ifstream Read("diceWarsAbout.txt");//Find the diceWarsAbout.txt
	if (Read.is_open())//If it can open.
	{
		while (!Read.eof())
		{
			getline(Read, str);
			cout << str << endl;//Display the diceWarsAbout.txt
		}
		Read.close();
	}
	else//If it cannot open.
	{
		cout << "Unable to open: " << filename << " " << endl;
	}
	cout << endl;
	system("pause");
	return;
}

int Game::askIntInput(string inputRequire)
{
	int inputNum;
	cout << inputRequire;
	while (!(cin >> inputNum))// If cannot read data from cin stream
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');// Clear current line
		cout << "Please enter again: ";
	}
	return inputNum;
}

string Game::askStrInput(string inputRequire)
{
	string inputString;
	cout << inputRequire;
	cin >> inputString;
	return inputString;
}

char Game::askCharInput(string inputRequire)
{
	char inputChar;
	cout << inputRequire;
	cin >> inputChar;
	return inputChar;
}

void Game::infoInput()
{
	int playerNum;
	playerNum = askIntInput("Please enter the number of players: ");//Input the number of players
	while (playerNum < 1)
	{
		playerNum = askIntInput("The number of players cannot be less than 1, Please enter again: ");//If input less than 1.
	}
	for (int i = 0; i < playerNum; i++)//Let each player to input their name
	{
		string enterPlayerName;
		stringstream strs;
		strs << "Please enter your name ---- Player" << i + 1 << ": ";//Using StringStream, and it is easy to display player number.
		enterPlayerName = askStrInput(strs.str());
		Player player(enterPlayerName);//Using the Player class constructer.
		playerList.push_back(player);//Put the player name into playerList.
		for (int a = 0; a < 6; a++)
		{
			playerDice[enterPlayerName].push_back(Dice());//Every player can see 6 dices and can choose them, I choose to use <map> to achieve this function, because <map> data is "key to value".
		}
	}
	if (playerNum == 1)//If only has one player, then add one Computer.
	{
		Player player("Computer");
		playerList.push_back(player);
		for (int a = 0; a < 6; a++)
		{
			playerDice["Computer"].push_back(Dice());//Comptuer also need to roll 6 dices.
		}
	}
}

void Game::displayPlayerInfo(const Player& playCall)
{
	string currentPlayerName;
	currentPlayerName = playCall.getName();//Get the player name.
	map<string, vector<Dice> >::iterator iterDices = playerDice.find(currentPlayerName);//Because playerDice is data of <map> type, so I use <map> type iterator to find the value. 
	vector<Dice>& vectorDice = iterDices->second;//vectorDice is equal [iterDice point to the Value of <map>].
	int leftDice = 0;
	for (int i = 0; i < vectorDice.size(); i++)
	{
		if (!(vectorDice[i].determineLock()))//Determine how many dice are not locked.
		{
			leftDice++;
		}
	}
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "Player: " << currentPlayerName << "   Score: " << playCall.getTotalScore() << "   TemporaryScore: " << playCall.getCurrentScore() << "   LeftDices: " << leftDice << "\n";
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

}

void Game::displayDice(const vector<Dice>& showDice)
{
	string line1, line2, line3, line4, line5;//Create 5 lines string value.
	vector<Dice>::const_iterator iter;//This iterator need to be const, because dice cannot change.
	for (iter = showDice.begin(); iter != showDice.end(); iter++)
	{
		int currentNum;
		currentNum = (*iter).getValue();//Get dices value.
		if (!((*iter).determineLock()))//If the dice is not lock.
		{
			switch (currentNum)
			{
			case 1:
			{
				line1.append("   +-----+   ");
				line2.append("   |     |   ");
				line3.append("   |  o  |   ");
				line4.append("   |     |   ");
				line5.append("   +-----+   ");
				break;
			}
			case 2:
			{
				line1.append("   +-----+   ");
				line2.append("   |  o  |   ");
				line3.append("   |     |   ");
				line4.append("   |  o  |   ");
				line5.append("   +-----+   ");
				break;
			}
			case 3:
			{
				line1.append("   +-----+   ");
				line2.append("   |o    |   ");
				line3.append("   |  o  |   ");
				line4.append("   |    o|   ");
				line5.append("   +-----+   ");
				break;
			}
			case 4:
			{
				line1.append("   +-----+   ");
				line2.append("   |o   o|   ");
				line3.append("   |     |   ");
				line4.append("   |o   o|   ");
				line5.append("   +-----+   ");
				break;
			}
			case 5:
			{
				line1.append("   +-----+   ");
				line2.append("   |o   o|   ");
				line3.append("   |  o  |   ");
				line4.append("   |o   o|   ");
				line5.append("   +-----+   ");
				break;
			}
			case 6:
			{
				line1.append("   +-----+   ");
				line2.append("   |o   o|   ");
				line3.append("   |o   o|   ");
				line4.append("   |o   o|   ");
				line5.append("   +-----+   ");
				break;
			}
			default:
				break;
			}
		}
		else//If the dice is lock.
		{
			line1.append("   +-----+   ");
			line2.append("   |-----|   ");
			line3.append("   |--X--|   ");
			line4.append("   |-----|   ");
			line5.append("   +-----+   ");
		}
	}  
	cout << line1 << "\n";
	cout << line2 << "\n";
	cout << line3 << "\n";
	cout << line4 << "\n";
	cout << line5 << "\n";
	for (int i = 0; i < showDice.size(); i++)
	{
		cout << "     " << "[" <<  i + 1 << "]" << "     ";//Display the number of dices.
	}
	cout << endl;

}

void Game::temporaryScore(Player& playCall)
{
	if (!farkledScore)//If the player is not farkled, the score is normal.
	{
		playCall.resetScore(playCall.getTotalScore() + playCall.getCurrentScore());//Calculate the total score for this round.
		playCall.updateScore(0);
		displayPlayerInfo(playCall);//Display the player information.
		string currentPlayerName;
		currentPlayerName = playCall.getName();
		map<string, vector<Dice> >::iterator iter = playerDice.find(currentPlayerName);//Use <map> iterator to find the player is exist or not.
		vector<Dice>& vectorDice = iter->second;//iter point to the Value of <map>.
		vector<Dice>::iterator iterDice;//Create a iterator to let 6 dices become unlock status.
		for (iterDice = vectorDice.begin(); iterDice != vectorDice.end(); ++iterDice)
		{
			iterDice->notLock();
		}
	}
	else//If the player is farkled, then the temporary score is 0, for this round that player cannot get any point.
	{
		playCall.resetScore(playCall.getTotalScore() + 0);//The total score for this round is 0.
		playCall.updateScore(0);
		displayPlayerInfo(playCall);//Display the player information.
		string currentPlayerName;
		currentPlayerName = playCall.getName();
		map<string, vector<Dice> >::iterator iter = playerDice.find(currentPlayerName);//Use <map> iterator to find the player is exist or not.
		vector<Dice>& vectorDice = iter->second;
		vector<Dice>::iterator iterDice;//Create a iterator to let 6 dices become unlock status.
		for (iterDice = vectorDice.begin(); iterDice != vectorDice.end(); ++iterDice)
		{
			iterDice->notLock();
		}
	}
	
}

bool Game::rollingDice(Player& playCall)
{
	string currentPlayerName;
	currentPlayerName = playCall.getName();//Get the player name to rolling dice.
	map<string, vector<Dice> >::iterator iterPlayer;//Create the iterator the find the player name.
	iterPlayer = playerDice.find(currentPlayerName);
	vector<Dice>& vectorDice = iterPlayer->second;//Point to the Value of <map>.
	vector<Dice> vectorNotLockDice;
	vector<Dice>::iterator iterDice;
	for (iterDice = vectorDice.begin(); iterDice != vectorDice.end(); iterDice++)
	{
		iterDice->rollDice();//Rolling each dice 1-6.
		if (!iterDice->determineLock())//Determine the dice is lock or not.
		{
			vectorNotLockDice.push_back(*iterDice);//Put all the "Not locked dice" into the vectorNotLockDice.
		}
	}
	if (vectorNotLockDice.size() == 0)//If there is not exist NotLockDice. 
	{
		return false;
	}
	displayDice(vectorDice);//Display Dices (ASCII)
	if (farkleDice(vectorNotLockDice))//Determine the dice is farkled or not.
	{
		cout << "######## " << currentPlayerName << " Farkled ########" << endl;
		countFarkled++;//If the player is farkled, then calculate the farkled times.
		farkledScore = true;
		return false;
	}
	int currentPlayerScore = 0;//Reset the temporary score.
	int numChoose;
	numChoose = askIntInput("Please choose dice (Don't include spaces): ");//Player choose dices for this round.
	while (!currentInputValue(vectorDice, numChoose, currentPlayerScore))//Determine the input is right or not.
	{
		numChoose = askIntInput("Please enter again: ");//If the player input the error number, input again.
	}
	playCall.updateScore(currentPlayerScore + playCall.getCurrentScore());//Update the temporary score for this round.
	bool returnLock = true;
	for (iterDice = vectorDice.begin(); iterDice != vectorDice.end(); iterDice++)
	{
		if (!iterDice->determineLock())//Determine dices is locked or not.
		{
			returnLock = false;//If not locked, this round is not finish.
		}
	}
	return !returnLock;//Prevent wrong judgment.
}

int Game::calculateScore(const vector<Dice>& vectorDice)
{
	int calScore = 0;
	int counter = 0;
	vector<int> sizeOfDice;//Create the vector to store all the input dices.
	vector<int> sizeOfDiceCounter;//Create the second vector to store all UNIQUE dices.

	for (int i = 0; i < vectorDice.size(); i++)
	{
		int everyDiceValue = vectorDice[i].getValue();//Get input dices value.
		sizeOfDice.push_back(everyDiceValue);//Put the value into the sizeOfDice.
		sizeOfDiceCounter.push_back(everyDiceValue);//Put the value into the sizeOfDiceCounter.
		
	}

	sort(sizeOfDiceCounter.begin(), sizeOfDiceCounter.end());//Sort all the data in sizeOfDiceCounter.
	vector<int>::iterator repeat = unique(sizeOfDiceCounter.begin(), sizeOfDiceCounter.end());
	sizeOfDiceCounter.erase(repeat, sizeOfDiceCounter.end());//Delete the UNIQUE dices in sizeOfDiceCounter.

	vector<int>::iterator iterPoint;

	for (iterPoint = sizeOfDiceCounter.begin(); iterPoint != sizeOfDiceCounter.end(); iterPoint++)//Use the value in sizeOfDiceCounter to find and count the UNIQUE dices in sizeOfDice.
	{
		int everyDiceValue = (*iterPoint);
		int countSameDice = count(sizeOfDice.begin(), sizeOfDice.end(), everyDiceValue);//calculate the UNIQUE dices in sizeOfDice.
		if (countSameDice >= 2)
		{
			counter++;
			
		}
		else
		{
			counter += countSameDice;
		}
	}
	int countDifferentNum = counter;//This is the different kind of dices in player input.
	int numInputDice = vectorDice.size();//This is the number of dices in player input.


	switch (numInputDice)
	{
	case 1://If player just input one dice.
	{
		int temporaryDice = vectorDice[0].getValue();
		if (temporaryDice == 1)
		{
			calScore = 100;
		}
		else if (temporaryDice == 5)
		{
			calScore = 50;
		}
		break;
	}
	case 2://Two dice is Irregular.
	{
		break;
	}
	case 3://If player input 3 dices.
	{
		if (countDifferentNum > 1)//Error input
		{
			break;
		}
		int temporaryDice = vectorDice[0].getValue();//Get the first value in player input.
		if (temporaryDice == 1)// 1 1 1
		{
			calScore = 300;
		}
		else if (temporaryDice == 2)// 2 2 2
		{
			calScore = 200;
		}			
		else if (temporaryDice == 3)// 3 3 3
		{
			calScore = 300;
		}
		else if (temporaryDice == 4)// 4 4 4
		{
			calScore = 400;
		}
		else if (temporaryDice == 5)// 5 5 5
		{
			calScore = 500;
		}
		else if (temporaryDice == 6)// 6 6 6 
		{
			calScore = 600;
		}
		break;
	}
	case 4://If player input 4 dices.
	{
		if (countDifferentNum == 1)//If input dices just has one kind.
		{
			calScore = 1000;
		}
		break;
	}
	case 5://If player input 5 dices.
	{
		if (countDifferentNum == 1)//If input dices just has one kind.
		{
			calScore = 2000;
		}
		break;
	}
	case 6://If player input 6 dices.
	{
		if (countDifferentNum == 1)//Just one kind of dice.
		{
			calScore = 3000;
		}
		else if (countDifferentNum == 2)//If there are two kind of dices.
		{
			for (int a = 0; a < sizeOfDice.size(); a++)
			{
				int temporaryDice = vectorDice[a].getValue();
				int SameNum = count(sizeOfDice.begin(), sizeOfDice.end(), temporaryDice);
				if (SameNum == 4 || SameNum == 2)//The dice are repeated 4 or twice
				{
					calScore = 1500;//AABBBB or AAAABB
					break;
				}
				else if (SameNum == 3)//The dice are repeated 3 times.
				{
					calScore = 2500;//AAABBB
					break;
				}

			}
		}
		else if (countDifferentNum == 3)//If there has 3 kind of dices.
		{
			for (int a = 0; a < sizeOfDice.size(); a++)
			{
				int temporaryDice = vectorDice[a].getValue();
				int SameNum = count(sizeOfDice.begin(), sizeOfDice.end(), temporaryDice);
				if (SameNum != 2)
				{
					calScore = 0;//Irregular.
					break;
				}
				if (SameNum == 2)//AABBCC 3 Pairs
				{
					calScore = 1500;
				}
				
			}
		}
		else if (countDifferentNum == 6)//123456 Straight.
		{
			calScore = 1500;
		}
		break;
	}
	default:
	{
		break;
	}

	}
	return calScore;
}

bool Game::farkleDice(const vector<Dice>& vectorDice)
{
	int calScore = 0;
	int counter = 0;
	vector<int> sizeOfDice;//Create the vector to store all the input dices.
	vector<int> sizeOfDiceCounter;//Create the second vector to store all UNIQUE dices.

	for (int i = 0; i < vectorDice.size(); i++)
	{
		int everyDiceValue = vectorDice[i].getValue();//Get input dices value.
		sizeOfDice.push_back(everyDiceValue);//Put the value into the sizeOfDice.
		sizeOfDiceCounter.push_back(everyDiceValue);//Put the value into the sizeOfDiceCounter.

	}

	sort(sizeOfDiceCounter.begin(), sizeOfDiceCounter.end());//Sort all the data in sizeOfDiceCounter.
	vector<int>::iterator repeat = unique(sizeOfDiceCounter.begin(), sizeOfDiceCounter.end());
	sizeOfDiceCounter.erase(repeat, sizeOfDiceCounter.end());//Delete the UNIQUE dices in sizeOfDiceCounter.

	vector<int>::iterator iterPoint;

	for (iterPoint = sizeOfDiceCounter.begin(); iterPoint != sizeOfDiceCounter.end(); iterPoint++)//Use the value in sizeOfDiceCounter to find and count the UNIQUE dices in sizeOfDice.
	{
		int everyDiceValue = (*iterPoint);
		int countSameDice = count(sizeOfDice.begin(), sizeOfDice.end(), everyDiceValue);//calculate the UNIQUE dices in sizeOfDice.
		if (countSameDice >= 2)
		{
			counter++;

		}
		else
		{
			counter += countSameDice;
		}
	}
	int countDifferentNum = counter;


	vector<int>::iterator it1 = find(sizeOfDice.begin(), sizeOfDice.end(), 1);
	vector<int>::iterator it2 = find(sizeOfDice.begin(), sizeOfDice.end(), 5);
	if (it1 != sizeOfDice.end() || it2 != sizeOfDice.end())
	{
		return false;//If there are 1 or 5 in all dices.
	}

	bool checkNum = false;
	int counterNumber = 0;
	vector<int>::iterator iterPointer;
	for (iterPointer = sizeOfDiceCounter.begin(); iterPointer != sizeOfDiceCounter.end(); iterPointer++)
	{
		int temporaryDice = (*iterPointer);
		int SameNum = count(sizeOfDice.begin(), sizeOfDice.end(), temporaryDice);//Calculate the UNIQUE dices.
		if (SameNum > 2)//If the dices appears times more than 2
		{
			return false;
		}
		else 
		{
			if (SameNum == 2)//If the dices appears times = 2, but it can¡¯t judge Farkled
			{
				counterNumber++;//So use counterNumber to determine input is Irregular or Not.
			}
			else
			{
				checkNum = true;//Just appears one times, also cannot determine input is Irregular or Not.
			}

		}

	}
	if (counterNumber == 3)//3 Pairs.
	{
		return false;//Not farkled.
	}
	if (checkNum)
	{
		return true;//Farkled
	}


	return true;
}

bool Game::currentInputValue(vector<Dice> &vectorDice, int numChoose, int &calScore)
{
	vector<int> chooseDiceNum;
	int arrayOfDice[5] = { 100000, 10000, 1000, 100, 10 };//Determine whether the input meets the rules.
	if (numChoose < 1)
	{
		return false;
	}
	int checkCalculate;
	for (int a = 0; a < 5; a++)
	{
		if (numChoose > arrayOfDice[a])
		{
			checkCalculate = numChoose / arrayOfDice[a];//Divide by the number in A to determine whether the input is correct
			if (checkCalculate > 6)
			{
				return false;
			}
			chooseDiceNum.push_back(checkCalculate);//Put the input number into chooseDcieNum.
			numChoose = numChoose % arrayOfDice[a];
		}
	}
	if (numChoose > 6)
	{
		return false;
	}
	chooseDiceNum.push_back(numChoose);
	sort(chooseDiceNum.begin(), chooseDiceNum.end());//Sort the number in chooseDiceNum.
	vector<int>::iterator pos = unique(chooseDiceNum.begin(), chooseDiceNum.end());
	chooseDiceNum.erase(pos, chooseDiceNum.end());//Remove the repeated value in chooseDiceNum.
	vector<Dice> vectorTemporaryDice;
	vector<int>::iterator findIter;
	for (findIter = chooseDiceNum.begin(); findIter != chooseDiceNum.end(); findIter++)
	{
		Dice diceValue = vectorDice.at((*findIter) - 1);
		if (diceValue.determineLock())
		{
			return false;//If player input dice is locked.
		}
		vectorTemporaryDice.push_back(diceValue);
	}
	calScore = calculateScore(vectorTemporaryDice);
	if (calScore <= 0)
	{
		return false;//If the Score is 0, then input not correct.
	}
	for (findIter = chooseDiceNum.begin(); findIter != chooseDiceNum.end(); findIter++)
	{
		vectorDice.at((*findIter) - 1).Lock();//Lock the player selectd dices.
	}
	return true;
}

void Game::delResult()
{
	char chooseDel;
	chooseDel = askCharInput("Do you want to delete all data? (y/n): ");
	if (chooseDel == 'y')
	{
		fstream Del("savedPlayerData.txt");
		if (!Del.is_open()) //If not exist txt
		{
			cout << " File not found. " << endl;
			cout << endl;
			system("pause");
			return;
		}
		else
		{
			char checkFile;
			Del >> checkFile;
			if (Del.eof())//If the txt is empty.
			{
				cout << " File is empty. " << endl;
				cout << endl;
				Del.close();
				system("pause");
				return;
			}
			else
			{
				fstream Del("savedPlayerData.txt", ios::out);
				Del.close();
				cout << "Data emptied." << endl;
				system("pause");
				return;
			}
		}

	}
	else if (chooseDel == 'n')
	{
		return;
	}
	cout << "Error input!\n";
	system("pause");
	return;
}

void Game::saveData(string Playername)
{
	ofstream Save("savedPlayerData.txt", ios::app);
	Save << "\n";
	Save << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	Save << "######################## Dice Wars ############################\n";
	Save << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	Save << "#### The Winner is " << Playername << "!" << "####\n";
	Save << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	Save.close();
}

void Game::displayResult()
{
	fstream Load("savedPlayerData.txt");
	if (!Load.is_open()) // if not exist txt
	{
		cout << " File not found. " << endl;
		cout << endl;
		system("pause");
		return;
	}
	else // if exist txt
	{
		char checkFile;
		Load >> checkFile;
		if (Load.eof())
		{
			cout << " File is empty. " << endl;
			cout << endl;
			system("pause");
			return;
		}
		else
		{
			string data;
			while (!Load.eof()) // getline untill finish read
			{
				getline(Load, data);
				cout << data << endl;
			}
			Load.close();
			system("pause");
			return;
		}
		
	}
}
//--------------------------------------------------------------
//---------------------AI---------------------------------------
bool Game::AI(Player& playCall)
{
	system("cls");
	bool checkAIScore = true;
	while (true)
	{
		system("cls");
		int leftDice = displayAIInfo(playCall);
		if (leftDice > 3)//If the number of dices Computer can choose >3
		{
			cout << "Computer choose [r]olling dice!";//Computer Rolling.
			system("pause");
			if (!checkPrepare(playCall))//Rolling dices, return true or false. 
			{
				temporaryScore(playCall);//If return false, that means the player is farkled or there are NO dices can roll, and then, get the score.
				break;
			}
		}
		else
		{
			cout << "Computer choose [b]ank the score!";//If <=3 can choose, Computer choose Bank.
			system("pause");
			temporaryScore(playCall);//Bank the temporary score.
			break;
		}

	}
	system("pause");
	if (!(playCall.getTotalScore() >= 5000))//If the Computer score is not more than 5000, return false.
	{
		checkAIScore = false;
	}

	return checkAIScore;

}

int Game::displayAIInfo(const Player& playCall)
{
	string currentPlayerName;
	currentPlayerName = playCall.getName();
	map<string, vector<Dice> >::iterator iterDices = playerDice.find(currentPlayerName);
	vector<Dice>& vectorDice = iterDices->second;
	int leftDice = 0;
	for (int i = 0; i < vectorDice.size(); i++)
	{
		if (!(vectorDice[i].determineLock()))
		{
			leftDice++;
		}
	}
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "Player: " << currentPlayerName << "   Score: " << playCall.getTotalScore() << "   TemporaryScore: " << playCall.getCurrentScore() << "   LeftDices: " << leftDice << "\n";
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	return leftDice;//Return the number of Left dices, and Computer can determine roll or bank.
}

int Game::chooseDiceAI(const vector<Dice>& vectorDice)
{
	int calScore = 0;
	int counter = 0;
	vector<int> sizeOfDice;//Create the vector to store all the input dices.
	vector<int> sizeOfDiceCounter;//Create the second vector to store all UNIQUE dices.

	for (int i = 0; i < vectorDice.size(); i++)
	{
		int everyDiceValue = vectorDice[i].getValue();//Get input dices value.
		sizeOfDice.push_back(everyDiceValue);//Put the value into the sizeOfDice.
		sizeOfDiceCounter.push_back(everyDiceValue);//Put the value into the sizeOfDiceCounter.

	}

	sort(sizeOfDiceCounter.begin(), sizeOfDiceCounter.end());//Sort all the data in sizeOfDiceCounter.
	vector<int>::iterator repeat = unique(sizeOfDiceCounter.begin(), sizeOfDiceCounter.end());
	sizeOfDiceCounter.erase(repeat, sizeOfDiceCounter.end());//Delete the UNIQUE dices in sizeOfDiceCounter.

	vector<int>::iterator iterPoint;

	for (iterPoint = sizeOfDiceCounter.begin(); iterPoint != sizeOfDiceCounter.end(); iterPoint++)//Use the value in sizeOfDiceCounter to find and count the UNIQUE dices in sizeOfDice.
	{
		int everyDiceValue = (*iterPoint);
		int countSameDice = count(sizeOfDice.begin(), sizeOfDice.end(), everyDiceValue);//calculate the UNIQUE dices in sizeOfDice.
		if (countSameDice >= 2)
		{
			counter++;

		}
		else
		{
			counter += countSameDice;
		}
	}
	int countDifferentNum = counter;


	bool checkNum = false;
	int counterNumber = 0;
	vector<int>::iterator iterPointer;
	for (iterPointer = sizeOfDiceCounter.begin(); iterPointer != sizeOfDiceCounter.end(); iterPointer++)
	{
		int temporaryDice = (*iterPointer);
		int SameNum = count(sizeOfDice.begin(), sizeOfDice.end(), temporaryDice);//Calculate the UNIQUE dices.
		if (SameNum > 2)//Enumerate all possibilities.
		{
			if (SameNum == 6)
			{
				return 123456;
			}
			if (SameNum == 5)
			{
				if (Enum == 0)
				{
					return 12345;
				}
				else
				{
					return 23456;
				}
			}
			if (SameNum == 4)
			{
				if (Enum == 0)
				{
					return 1234;
				}
				else if (Enum == 1)
				{
					return 1235;
				}
				else if (Enum == 2)
				{
					return 1236;
				}
				else if (Enum == 3)
				{
					return 1245;
				}
				else if (Enum == 4)
				{
					return 1246;
				}
				else if (Enum == 5)
				{
					return 1256;
				}
				else if (Enum == 6)
				{
					return 1345;
				}
				else if (Enum == 7)
				{
					return 1346;
				}
				else if (Enum == 8)
				{
					return 1356;
				}
				else if (Enum == 9)
				{
					return 1456;
				}
				else if (Enum == 10)
				{
					return 2345;
				}
				else if (Enum == 11)
				{
					return 2346;
				}
				else if (Enum == 12)
				{
					return 2356;
				}
				else if (Enum == 13)
				{
					return 2456;
				}
				else if (Enum == 14)
				{
					return 3456;
				}
			}
			if (SameNum == 3)
			{
				if (Enum == 0)
				{
					return 123;
				}
				else if (Enum == 1)
				{
					return 124;
				}
				else if (Enum == 2)
				{
					return 125;
				}
				else if (Enum == 3)
				{
					return 126;
				}
				else if (Enum == 4)
				{
					return 134;
				}
				else if (Enum == 5)
				{
					return 135;
				}
				else if (Enum == 6)
				{
					return 136;
				}
				else if (Enum == 7)
				{
					return 145;
				}
				else if (Enum == 8)
				{
					return 146;
				}
				else if (Enum == 9)
				{
					return 156;
				}
				else if (Enum == 10)
				{
					return 234;
				}
				else if (Enum == 11)
				{
					return 235;
				}
				else if (Enum == 12)
				{
					return 236;
				}
				else if (Enum == 13)
				{
					return 245;
				}
				else if (Enum == 14)
				{
					return 246;
				}
				else if (Enum == 15)
				{
					return 256;
				}
				else if (Enum == 16)
				{
					return 345;
				}
				else if (Enum == 17)
				{
					return 346;
				}
				else if (Enum == 18)
				{
					return 356;
				}
				else if (Enum == 19)
				{
					return 456;
				}
			}
		}
		else
		{
			if (SameNum == 2)
			{
				counterNumber++;
			}
			else
			{
				checkNum = true;
			}

		}

	}
	if (counterNumber == 3)
	{
		return 123456;
	}

	vector<int>::iterator it1 = find(sizeOfDice.begin(), sizeOfDice.end(), 1);
	vector<int>::iterator it2 = find(sizeOfDice.begin(), sizeOfDice.end(), 5);
	if (it1 != sizeOfDice.end() || it2 != sizeOfDice.end())//Enumerate all possibilities.
	{
		if (Enum == 0)
		{
			return 1;
		}
		else if (Enum == 1)
		{
			return 2;
		}
		else if (Enum == 2)
		{
			return 3;
		}
		else if (Enum == 3)
		{
			return 4;
		}
		else if (Enum == 4)
		{
			return 5;
		}
		else if (Enum == 5)
		{
			return 6;
		}
	}
	if (checkNum)
	{
		return 0;
	}

	
	return 0;
}

bool Game::checkPrepare(Player& playCall)
{
	string currentPlayerName;
	currentPlayerName = playCall.getName();//Get the player name to rolling dice.
	map<string, vector<Dice> >::iterator iterPlayer;//Create the iterator the find the player name.
	iterPlayer = playerDice.find(currentPlayerName);
	vector<Dice>& vectorDice = iterPlayer->second;//Point to the Value of <map>.
	vector<Dice> vectorNotLockDice;
	vector<Dice>::iterator iterDice;
	for (iterDice = vectorDice.begin(); iterDice != vectorDice.end(); iterDice++)
	{
		iterDice->rollDice();//Rolling each dice 1-6.
		if (!iterDice->determineLock())//Determine the dice is lock or not.
		{
			vectorNotLockDice.push_back(*iterDice);//Put all the "Not locked dice" into the vectorNotLockDice.
		}
	}
	if (vectorNotLockDice.size() == 0)//If there is not exist NotLockDice.
	{
		return false;
	}
	displayDice(vectorDice);//Display Dices (ASCII)
	if (farkleDice(vectorNotLockDice))//Determine the dice is farkled or not.
	{
		cout << "######## " << currentPlayerName << " Farkled ########" << endl;
		countAIFarkled++;//Calaulate how many times AI farkled.
		farkledScore = true;
		return false;
	}
	int currentPlayerScore = 0;//Reset the temporary score.
	int numChoose;
	numChoose = chooseDiceAI(vectorNotLockDice);//Computer choose dices for this round.
	while (!currentInputValue(vectorDice, numChoose, currentPlayerScore))
	{
		Enum++;//Enumerate all possibilities need to use a parameter.
		numChoose = chooseDiceAI(vectorNotLockDice);
	}
	Enum = 0;
	cout << "Computer choose: " << numChoose << endl;//Display Computer choose
	system("pause");
	playCall.updateScore(currentPlayerScore + playCall.getCurrentScore());//Update the temporary score for this round.
	bool returnLock = true;
	for (iterDice = vectorDice.begin(); iterDice != vectorDice.end(); iterDice++)
	{
		if (!iterDice->determineLock())//Determine dices is locked or not.
		{
			returnLock = false;//If not locked, this round is not finish.
		}
	}
	return !returnLock;//Prevent wrong judgment.
}
