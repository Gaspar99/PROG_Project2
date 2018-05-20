#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <ctime>

class Player
{
public:
	Player();
	Player(std::string playerName);
	void setBoardFileName(std::string boardFileName);
	void increaseCounter();
	void setStartTime();
	void setEndTime();
	void congratulate();
	void saveData();
private:
	std::string playerName;
	unsigned int counter = 0; //counter of how many times the player asked for help
	int startTime, endTime;
	double time;
	std::string boardFileName;
};

#endif 