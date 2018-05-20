#include "Player.h"
#include "utils.h"
#include <fstream>
#include <iomanip>
#include <chrono>
#include <sstream>


Player::Player()
{
	playerName = "";
}

Player::Player(string playerName)
{
	this->playerName = playerName;
}

void Player::setBoardFileName(string boardFileName)
{
	this->boardFileName = boardFileName.substr(0,4); //For the board b001.txt, this updates its name to b001
}

void Player::increaseCounter()
{
	counter++;
}

void Player::setStartTime()
{
	startTime = clock();
}

void Player::setEndTime()
{
	endTime = clock();
}

void Player::congratulate()
{
	time = (double)(endTime - startTime) / CLOCKS_PER_SEC;

	setcolor(GREEN);
	cout << "Congratulations " << playerName << "!! You have succesfully solved the puzzle!" << endl;
	setcolor(WHITE);

	cout << "You asked for help " << counter << " times." << endl;
	cout << "It took you " << time << " seconds to complete the puzzle." << endl;
}

void Player::saveData()
{
	string title, line;
	ofstream file;

	ostringstream outFileName;
	outFileName << boardFileName << "_p.txt";

	string fileName = outFileName.str();

	cout << "Saving your data to " << fileName << endl;

	file.open(fileName, ios_base::app);

	file << "Player Name: " << playerName << "; "
		<< " Help: " << counter << " times; "
		<< " Time: " << time << " seconds." << endl;

	cout << "Your data was saved." << endl;
}


