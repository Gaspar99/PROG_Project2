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
	void clearSuggestions();
	bool suggestions_is_empty();
	void currentWords_insert(string coord, string word);
	void currentWords_erase(string coord);
	void currentWords_clear();
	void currentWords_send(ofstream &outStream);
	bool isCurrentWord(string word);
private:
	map<string, vector<string>> synonymsList;
	set<string> validWords;
	map<string, vector<string>> suggestions;
	map<string, string> currentWords;
};

#endif 