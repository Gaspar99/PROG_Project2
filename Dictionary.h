#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <map>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Dictionary
{
public:
	Dictionary();
	void load(string dictionaryName);
	bool isValid(string word);
private:
	map<string, vector<string>> synonymsList;
	vector<string> validWords;
};

#endif 