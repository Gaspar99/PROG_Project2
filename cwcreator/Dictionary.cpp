#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include "Board.h"
#include "Dictionary.h"
#include "utils.h"

using namespace std;

Dictionary::Dictionary() 
{
	synonymsList[""] = { "" };
	validWords = {""};
	suggestions[""] = {""};
}

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

//Searches for 'word' in the validWords vector. Returns true if found
bool Dictionary::isValid(string word) {
	bool valid;

	valid = binary_search(validWords.begin(), validWords.end(), word);

    return valid;
}

//Looks for valid words that match with 'line' and then eliminates the last element of 'line'
//to check all possible matches for the current coordinates
//If there is a match, this method stores the word in a map in which the key is a string with the coordinates of the first letter of the word,
//and the value is a vector of strings with each string being a possible word to start on those coordinates
void Dictionary::storeSuggestions(string coordinates, string line)
{

	while (!line.empty()) {

		for (const string &word : validWords) {
		
			if (wildcardMatch(word.c_str(), line.c_str())) {

				if (!isCurrentWord(word)) {

					suggestions.insert(pair<string, vector<string>>(coordinates, vector<string>()));
					suggestions[coordinates].push_back(word);
				}
			}
		}
		line.erase(line.length() - 1);
	} 
}

//This method goes through the map created by 'storeSuggestions' and prints each coordinates and the corresponding words that can be put there.
//It also prints, at maximum, 4 synonyms of each one of those words
void Dictionary::showSuggestions()
{
	vector<string> synonyms;
	vector<string> mainWords;

	for (const auto &suggestion : suggestions) {
		cout << "Coordinates: " << suggestion.first << " - Words:" << endl;
		mainWords = suggestion.second;

		const int length = mainWords.size();
		int i = min(10, length);

		while (i > 0) {
		    int index = rand() % mainWords.size();
		    string word = mainWords[index];

            cout << setw(32);
            cout << word << " - ";
            synonyms = synonymsList.find(word)->second;

            for (unsigned int j = 0; j < 5 && j < synonyms.size(); j++)	{ cout << synonyms[j] << ", "; }
            cout << endl;

            --i;
		}
	}
}

void Dictionary::clearSuggestions()
{
	suggestions.clear();
}

bool Dictionary::suggestions_is_empty()
{
	return suggestions.empty();
}

void Dictionary::currentWords_insert(string coord, string word)
{
	currentWords.insert(pair<string, string>(coord, word));
}

void Dictionary::currentWords_erase(string coord)
{
	currentWords.erase(coord);
}

void Dictionary::currentWords_clear()
{
	currentWords.clear();
}

bool Dictionary::isCurrentWord(string word)
{
	for (const auto &it : currentWords) {
		if (it.second == word)
			return true;
	}

	return false;
}

void Dictionary::currentWords_send(ofstream &outStream)
{
	for (auto &it : currentWords) {
		outStream << it.first << " " << it.second << endl;
	}
}

multimap<string, string> Dictionary::getCurrentWords()
{
	return currentWords;
}

bool Dictionary::isInitialCoord(string coord)
{
	return (currentWords.find(coord) != currentWords.end());
}
