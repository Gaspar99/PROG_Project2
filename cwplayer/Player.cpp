#include "Player.h"
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

	cout << "Congratulations " << playerName << "!! You have succesfully solved the puzzle!" << endl;
	cout << "You asked for help " << counter << " times." << endl;
	cout << "It took you " << time << " seconds to complete the puzzle." << endl;
}

void Player::saveData()
{
	string title;
	ofstream file;
	static unsigned int fileID = 0;
	++fileID;

	ostringstream outFileName;
	outFileName << 'b' << setw(3) << setfill('0') << fileID << "_p.txt";

	string fileName = outFileName.str();

	cout << "Saving your data to " << fileName << endl;
	file.open(fileName);

	title = "PLAYER INFO";
	string s(title.length(), '=');
	file << s << endl;
	file << title << endl;
	file << s << endl << endl;

	file << "Name: " << playerName << ";\n";
	file << "Number of times " << playerName << " asked for help: " << counter << ";\n";
	file << "Time taken to solve puzzle: " << time << " seconds.\n";;
	
	cout << "Your data was saved." << endl;
	exit(0);
}


