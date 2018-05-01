#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include "Board.h"

using namespace std;

class Dictionary
{
public:
	Dictionary();
	Dictionary(string dictionaryName);

	void load(string dictionaryName);
	bool isValid(string word);
	void wordsSuggestions(string coordinates, string line);
private:
	map<string, vector<string>> synonymsList;
	vector<string> validWords;
	map<string, vector<string>> suggestions;
};

#endif 