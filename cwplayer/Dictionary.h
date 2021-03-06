#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <map>
#include <string>
#include <vector>
#include <set>
#include "Board.h"

class Dictionary
{
public:
    Dictionary();
    Dictionary(const std::string &dictionaryName);

    //Opens the dictionary and stores its content to the map 'synonymsList'
    void load(const std::string &dictionaryName);
    static bool isValid(const std::string &word);
    void calculateClues(); //Calculates random synonyms for each word position and stores them into the map 'clues'
    void showClues(); //Shows clues seperated by two sections. Horizontal and vertical words
    void boardWords_insert(const std::string &coord, const std::string &word); //Insertes a new key and value to board words map

    //Shows another clue for the given position considering the previous that was shown
    void showAnotherClue(const std::string &coord);

private:
    //The key is the main word and the value is a vector with the corresponding synonyms
    std::map<std::string, std::vector<std::string>> synonymsList;

    //A set with the main words (keys) of the synonymsList map
    static std::set<std::string> validWords;

    //The keys are the positions and the values are the corresponding words that were inserted by the player
    std::map<std::string, std::string> boardWords;

    //The keys are the positions and the values are the corresponding calculated clues
    std::map<std::string, std::string> clues;
};

#endif 