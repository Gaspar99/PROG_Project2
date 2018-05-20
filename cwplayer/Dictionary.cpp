#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Board.h"
#include "Dictionary.h"
#include "utils.h"
#include "Player.h"

using namespace std;

Dictionary::Dictionary() = default;

//Opens the dictionary and extracts its content to a map (synonymsList) and to a vector of strings (validWords)
//The keys of the map are the main words and the values are vectors of strings with each string being a synonym of the main word
//The validWords vector contains all the main words in the dictionary. This data base is useful to later check if a certain word is valid
Dictionary::Dictionary(string dictionaryName)
{
	load(dictionaryName);
}

void Dictionary::load(const string &dictionaryName)
{
	ifstream dictionary;
	string line;
	string mainWord;
	string word;
	unsigned int pos = 0;
	unsigned int index = 0;
	setcolor(WHITE);

	dictionary.open(dictionaryName);

	if (!dictionary.is_open()) {
	    setcolor(LIGHTRED);
		cerr << "Opening of dictionary file failed. Does it exist?" << endl;
		setcolor(WHITE);
		exit(1);
	}

	cout << "Extracting..." << endl;

while (getline(dictionary, line)) {

	index = line.find(':'); //searches for ':' in line, to get the string before it
	mainWord = line.substr(0, index);
	capitalize(mainWord);
	synonymsList.insert(pair<string, vector<string>>(mainWord, vector<string>()));
	validWords.insert(mainWord);

	pos = index + 2;

	index = line.find(',', pos);

	while (index != string::npos) {
		word = line.substr(pos, index - pos);

		if (word[0] != '[') {
			capitalize(word);
			synonymsList[mainWord].push_back(word);
		}

		pos = index + 2;
		index = line.find(',', pos);
	}

	if (line[line.length() - 1] != ',') {
		word = line.substr(pos, line.length() - pos);
		capitalize(word);
		synonymsList[mainWord].push_back(word);
	}
}
}

set<string> Dictionary::validWords;
//Searches for 'word' in the validWords vector. Returns true if found
bool Dictionary::isValid(string word) {
	bool valid;

	valid = binary_search(validWords.begin(), validWords.end(), word);

	return valid;
}

//This method goes through the map created by 'storeSuggestions' and prints each coordinates and the corresponding words that can be put there.
//It also prints, at maximum, 4 synonyms of each one of those words
void Dictionary::calculateClues()
{
	vector<string> synonyms;
	string coord, word, clue;
	size_t index;

	for (const auto &it : boardWords) {
		coord = it.first;
		word = it.second;
		synonyms = synonymsList.find(word)->second;
		index = rand() % synonyms.size();
		clue = synonyms[index];
		clues.insert(pair<string, string>(coord, clue));
	}
}

void Dictionary::showClues()
{
	string coord, clue;
	cout << "HORIZONTAL:" << endl;
	for (const auto &it : clues) 
	{
		if (it.first[2] == 'H') {
			clue = it.second;
			coord = it.first.substr(0, 2);
			cout << coord << " - " << clue << endl;
		}
	}

	cout << "VERTICAL:" << endl;
	for (const auto &it : clues)
	{
		if (it.first[2] == 'V') {
			clue = it.second;
			coord = it.first.substr(0, 2);
			cout << coord << " - " << clue << endl;
		}
	}
}

void Dictionary::boardWords_insert(string coord, string word)
{
	boardWords.insert(pair<string, string>(coord, word));
}

void Dictionary::showAnotherClue(string coord)
{
	string previousClue = clues.find(coord)->second;
	string word = boardWords.find(coord)->second;
	vector<string> synonyms = synonymsList.find(word)->second;
	int index = rand() % synonyms.size();

	while (synonyms[index] == previousClue)
	{
		if (synonyms.size() == 1) {
			cout << "The word on the position " << coord << " only has one synonym: " << endl;
			cout << previousClue << endl;
			return;
		}
		else
			index = rand() % synonyms.size();
	}
	cout << "New clue:" << endl;
	cout << coord << " - " << synonyms[index] << endl;

	clues.erase(coord);
	clues.insert(pair<string, string>(coord, synonyms[index]));
}