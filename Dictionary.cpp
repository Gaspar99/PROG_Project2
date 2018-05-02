#include "Dictionary.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "Board.h"
#include <utility>

using namespace std;

//////////////////////////////////////////////////////////////////////////
// WildcardMatch
// str - Input string to match
// strWild - Match mask that may contain wildcards like ? and *
//
// A ? sign matches any character, except an empty string.
// A * sign matches any string inclusive an empty string.
// Characters are compared caseless.
//
// ADAPTED FROM:
// https://www.codeproject.com/Articles/188256/A-Simple-Wildcard-Matching-Function
bool wildcardMatch(const char *str, const char *strWild)
{
	// We have a special case where string is empty ("") and the mask is "*".
	// We need to handle this too. So we can't test on !*str here.
	// The loop breaks when the match string is exhausted.
	while (*strWild) {
		// Single wildcard character
		if (*strWild == '.') {
			// Matches any character except empty string
			if (!*str)
				return false;
			// OK next
			++str;
			++strWild;
		}
		else if (*strWild == '*') {
			// Need to do some tricks.
			// 1. The wildcard * is ignored.
			// So just an empty string matches. This is done by recursion.
			// Because we eat one character from the match string,
			// the recursion will stop.
			if (wildcardMatch(str, strWild + 1))
				// we have a match and the * replaces no other character
				return true;
			// 2. Chance we eat the next character and try it again,
			// with a wildcard * match. This is done by recursion.
			// Because we eat one character from the string,
			// the recursion will stop.
			if (*str && wildcardMatch(str + 1, strWild))
				return true;
			// Nothing worked with this wildcard.
			return false;
		}
		else {
			// Standard compare of 2 chars. Note that *str might be 0 here,
			// but then we never get a match on *strWild
			// that has always a value while inside this loop.
			if (toupper(*str++) != toupper(*strWild++))
				return false;
		}
	}
	// Have a match? Only if both are at the end...
	return !*str && !*strWild;
}

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

		for (string s : validWords) {
			if (wildcardMatch(s.c_str(), line.c_str())) {
				suggestions.insert(pair<string, vector<string>>(coordinates, vector<string>()));
				suggestions[coordinates].push_back(s);
			}
		}
		line.erase(line.length() - 1);
	}
}

void Dictionary::showSuggestions()
{
	for (auto it = suggestions.cbegin(); it != suggestions.cend(); ++it) {
		cout << "Coordinates: " << it->first << " - Words: ";
		for (size_t i = 0; i < it->second.size(); i++) {
			cout << it->second[i] << endl;
		}
	}
}

void Dictionary::showValidWords()
{
	for (string s : validWords) {
		cout << s << endl;
	}
}
