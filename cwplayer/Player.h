#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <ctime>

class Player
{
public:
    Player();
    Player(const std::string &playerName);

    void setBoardFileName(const std::string &boardFileName);
    void increaseCounter();
    void setStartTime(); //Sets the time in which the player started the game
    void setEndTime(); //Sets the time in which the player ended the game

    //After winning the game, the player is congratulated and informed of how much time he/she took
    //to solve the puzzle and the number of times he/she asked for help
    void congratulate();

    //Appends the player data to the file where the data of previous players that completed the puzzle is stored
    void saveData();
private:
    std::string playerName;
    unsigned int counter = 0; //counter of how many times the player asked for help
    int startTime, endTime;
    double time;
    std::string boardFileName;
};

#endif 