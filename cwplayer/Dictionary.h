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

	void load(const string &dictionaryName);
	static bool isValid(string word);
	void showClues();
	void boardWords_insert(string coord, string word);
	void showAnotherClue(string coord);

private:
	map<string, vector<string>> synonymsList;
	static set<string> validWords;
	map<string, string> boardWords;
	map<string, string> clues;
};

#endif 