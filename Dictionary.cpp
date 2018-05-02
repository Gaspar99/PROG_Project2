#include "Dictionary.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "Board.h"
#include <utility>
#include <iomanip>

using namespace std;

Dictionary::Dictionary() {
	map<string, vector<string>> synonymsList;
	set<string> validWords;
	map<string, vector<string>> suggestions;
}

//Opens the dictionary and extracts its content to a map (synonymsList) and to a vector of strings (validWords)
//The keys of the map are the main words and the values are vectors of strings with each string being a synonym of the main word
//The validWords vector contains all the main words in the dictionary. This data base is useful to later check if a certain word is valid
Dictionary::Dictionary(string dictionaryName)
{
	load(dictionaryName);
}

void Dictionary::load(string dictionaryName)
{
	ifstream dictionary;
	string line;
	string mainWord;
	string word;
	int pos = 0;
	int index = 0;

	dictionary.open(dictionaryName);

	if (!dictionary.is_open()) {
		cerr << "Opening of dictionary file failed. Does it exist?" << endl;
		exit(1);
	}

	cout << "Extracting, this may take a while..." << endl;

	while (getline(dictionary, line)) {

		index = line.find(":");
		mainWord = line.substr(0, index);
		capitalize(mainWord);
		synonymsList.insert(pair<string, vector<string>>(mainWord, vector<string>()));
		validWords.insert(mainWord);
		pos = index + 2;

		index = line.find(",", pos);

		while (index != string::npos) {
			word = line.substr(pos, index - pos);
			capitalize(word);
			synonymsList[mainWord].push_back(word);
			pos = index + 2;
			index = line.find(",", pos);
		}

		if (line[line.length() - 1] != ',') {
			word = line.substr(pos, line.length() - pos);
			capitalize(word);
			synonymsList[mainWord].push_back(word);
		}
	}
}

//Serches for 'word' in the validWords vector. Returns true if found
bool Dictionary::isValid(string word) {
	bool valid;

	valid = binary_search(validWords.begin(), validWords.end(), word);

	if (valid) { return true; }
	else { return false; }
}

//Looks for valid words that match with 'line' and then elimates the last element of 'line'
//to check all possible matches for the current coordinates
//If there is a match, this method stores the word in a map in wich the key is a string with the coordinates of the first letter of the word,
//and the value is a vector of strings with each string being a possible word to start on those coordinates
void Dictionary::storeSuggestions(string coordinates, string line)
{

	while (!line.empty()) {

		for (string word : validWords) {
		
			if (wildcardMatch(word.c_str(), line.c_str())) {
				suggestions.insert(pair<string, vector<string>>(coordinates, vector<string>()));
				suggestions[coordinates].push_back(word);
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

	for (auto it = suggestions.cbegin(); it != suggestions.cend(); ++it) {

		cout << "Coordinates: " << it->first << " - Words:" << endl;
		mainWords = it->second;

		for (string word : mainWords) {
			cout << setw(32);
			cout << word << " - ";
			synonyms = synonymsList.find(word)->second;

			for (int i = 0; i < 5 && i < synonyms.size(); i++)	{ cout << synonyms[i] << ", "; }
			cout << endl;
		}
	}
}
