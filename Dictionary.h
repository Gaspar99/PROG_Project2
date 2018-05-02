#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <map>
#include <string>
#include <vector>

using namespace std;

class Dictionary
{
public:
	Dictionary();
	Dictionary(string dictionaryName);

	void load(string dictionaryName);
	bool isValid(string word);

private:
	map<string, vector<string>> synonymsList;
	vector<string> validWords;
};

#endif 