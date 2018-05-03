#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include "Board.h"
#include <set>

using namespace std;

class Dictionary
{
public:
	Dictionary();
	Dictionary(string dictionaryName);

	void load(string dictionaryName);
	bool isValid(string word);
	void storeSuggestions(string coordinates, string line);
	void showSuggestions();
	void showValidWords(); // for debugging purposes
private:
	map<string, vector<string>> synonymsList;
	set<string> validWords;
	map<string, vector<string>> suggestions;
};

#endif 