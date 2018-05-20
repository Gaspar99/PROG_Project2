#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <map>
#include <string>
#include <vector>
#include <set>
#include "Board.h"

using namespace std;

class Dictionary
{
public:
	Dictionary();
	Dictionary(string dictionaryName);

	void load(const string &dictionaryName); //Opens the dictionary and stores its content to the map 'synonymsList'
	static bool isValid(string word);
	void calculateClues(); //Calculates random synonyms for each word position and stores them into the map 'clues'
	void showClues(); //Shows clues seperated by two sections. Horizontal and vertical words
	void boardWords_insert(string coord, string word); //Insertes a new key and value to board words map
	void showAnotherClue(string coord); //Shows another clue for the given position considering the previous that was shown
private:
	map<string, vector<string>> synonymsList; //The key is the main word and the value is a vector with the corresponding synonyms
	static set<string> validWords; //A set with the main words (keys) of the synonymsList map
	map<string, string> boardWords; //The keys are the positions and the values are the correponding words that were inserted by the player
	map<string, string> clues; //The keys are the positions and the values are the corresponding calculated clues
};

#endif 