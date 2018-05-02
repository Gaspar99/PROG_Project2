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

	//Transforms a string puting all its characters in uppercase
	void capitalize(string &word)
{
	transform(word.begin(), word.end(), word.begin(), toupper);
}

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

Dictionary::Dictionary()
{
	synonymsList[""] = { "" };
	validWords = { "" };
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

		index = line.find_first_of(":");
		mainWord = line.substr(0, index);
		capitalize(word);
		synonymsList.insert(pair<string, vector<string>>(mainWord, vector<string>()));
		validWords.push_back(word);
		pos = index + 2;

		index = line.find_first_of(",", pos);

		while (index != string::npos) {
			word = line.substr(pos, index - pos);
			capitalize(word);
			synonymsList[mainWord].push_back(word);
			pos = index + 2;
			index = line.find_first_of(",", pos);
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

	sort(validWords.begin(), validWords.end());
	valid = binary_search(validWords.begin(), validWords.end(), word);

	if (valid) { return true; }
	else { return false; }
}

//Looks for valid words that match with 'line' and then elimates the last element of 'line'
//to check all possible matches for the current coordinates
void Dictionary::wordsSuggestions(string coordinates, string line)
{
	while (!line.empty()) {

		for (string s : validWords) {
			if (wildcardMatch(s.c_str(), line.c_str())) {
				suggestions.insert(pair<string, vector<string>>(coordinates, vector<string>()));
				suggestions[coordinates].push_back(line);
			}
		}
		line.erase(line.size() - 1);
	}
}

