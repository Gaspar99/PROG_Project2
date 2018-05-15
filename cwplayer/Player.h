#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>

using namespace std;

class Player
{
public:
	Player();
	Player(string playerName);
private:
	string playerName;
	unsigned int counter = 0; //counter of how many times the player asked for help
	//int time;
};

#endif 