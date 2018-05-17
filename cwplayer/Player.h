#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <ctime>

using namespace std;

class Player
{
public:
	Player();
	Player(string playerName);
	void increaseCounter();
	void setStartTime();
	void setEndTime();
	void congratulate();
	void saveData();
private:
	string playerName;
	unsigned int counter = 0; //counter of how many times the player asked for help
	int startTime, endTime;
	double time;
};

#endif 