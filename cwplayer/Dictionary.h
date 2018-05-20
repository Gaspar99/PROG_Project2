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
    Dictionary(std::string dictionaryName);

	void load(const std::string &dictionaryName);
	static bool isValid(std::string word);
    void calculateClues();
	void showClues();
	void boardWords_insert(std::string coord, std::string word);
	void showAnotherClue(std::string coord);

private:
	std::map<std::string, std::vector<std::string>> synonymsList;
	static std::set<std::string> validWords;
	std::map<std::string, std::string> boardWords;
	std::map<std::string, std::string> clues;
};

#endif 