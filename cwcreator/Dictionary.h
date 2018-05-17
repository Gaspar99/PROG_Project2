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
	explicit Dictionary(string dictionaryName);

	void load(const string &dictionaryName);
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

	multimap<string, string> getCurrentWords();

private:
	map<string, vector<string>> synonymsList;
	set<string> validWords;
	map<string, vector<string>> suggestions;
	multimap<string, string> currentWords;
};

#endif 