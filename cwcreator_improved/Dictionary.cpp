#include <fstream>
#include <string>
#include <iostream>
#include <utility>
#include <algorithm>
#include <iomanip>
#include "Dictionary.h"

using namespace std;

Dictionary::Dictionary() = default;

//Opens the dictionary and extracts its content to a map (synonymsList) and to a vector of strings (validWords)
//The keys of the map are the main words and the values are vectors of strings with each string being a synonym of the main word
//The validWords vector contains all the main words in the dictionary. This data base is useful to later check if a certain word is valid
void Dictionary::load(const string &filename)
{
    ifstream dictionary;
    string line;
    string mainWord;
    string word;
    unsigned int pos = 0;
    unsigned int index = 0;

    dictionary.open(filename);

    if (!dictionary.is_open()) {
        logger.error("DictionaryOpenFailed");
        exit(1);
    }

    cout << "Extracting..." << endl;

    while (getline(dictionary, line)) {
        index = line.find(':'); //searches for ':' in line, to get the string before it
        mainWord = line.substr(0, index);

        utility.capitalize(mainWord);

        synonymsList.insert(pair<string, vector<string>>(mainWord, vector<string>()));
        validWords.insert(mainWord);
        pos = index + 2; //Updates the position where line.find is going to start searching for a comma

        index = line.find(',', pos);

		//Goes through the strings between comas and pushes them to the second element of the map
		//When while loop ends, there is still a word after the last comma
		//However there are some lines where this does not happen
        while (index != string::npos) {
            word = line.substr(pos, index - pos);

            if (word[0] != '[') { //Ignores synonyms with [see 'another synonym']
                utility.capitalize(word);
                synonymsList[mainWord].push_back(word);
            }

            pos = index + 2; 
            index = line.find(',', pos);
        }

		//If there is not a word after the comma, accesing the substring after it would cause an error
		//Checks if the last character of the line is a comma to avoid an error when accessing the substring after the comma
        if (line[line.length() - 1] != ',') {
            word = line.substr(pos, line.length() - pos);
            utility.capitalize(word);
            synonymsList[mainWord].push_back(word);
        }
    }
}

//Searches for 'word' in the validWords vector. Returns true if found
bool Dictionary::isValid(string word)
{
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
    while (!line.empty()) 
	{
        for (const string &word : validWords) 
		{
            if (utility.wildcardMatch(word.c_str(), line.c_str())) 
			{
                if (!isCurrentWord(word) && !isInitialCoord(coordinates)) {
                    suggestions.insert(pair<string, vector<string>>(coordinates, vector<string>()));
                    suggestions[coordinates].push_back(word);
                }
            }
        }
        line.erase(line.length() - 1);
    }
}

//This method goes through the map created by 'storeSuggestions' and prints each coordinates and the corresponding words that can be put there.
//It also calls showSynonyms, defined below, to print, at maximum, 4 synonyms of each one of those words
void Dictionary::showSuggestions()
{
    vector<string> synonyms;
    vector<string> mainWords;
	vector<string> shownSuggestions;

    for (const auto &suggestion : suggestions) {
        cout << "Coordinates: " << suggestion.first << " - Words:" << endl;
        mainWords = suggestion.second;

        const int length = mainWords.size();
        int i = min(10, length);

		if (i == length)
		{
			for (string word : mainWords) {
				showSynonyms(word);
			}
		}

		else {
			while (i > 0) 
			{
				int index = rand() % length;
				string word = mainWords[index];

				if (find(shownSuggestions.begin(), shownSuggestions.end(), word) == shownSuggestions.end()) {
					showSynonyms(word);
					shownSuggestions.push_back(word);
				}
				
				--i;
			}
		}
    }
}

void Dictionary::showSynonyms(string word)
{
	vector<string> synonyms = synonymsList.find(word)->second;

	cout << setw(32);
	cout << word << " - ";

	for (unsigned int j = 0; j < 5 && j < synonyms.size(); j++) 
	{ cout << synonyms[j] << ", "; }

	cout << endl;
}

//Necessary so every time the user asks for suggestions, it wont show the suggestions of the previous time he/she asked for them
void Dictionary::clearSuggestions()
{
    suggestions.clear();
}

//Necessary to give a proper message to the user in case there are no matches on the given position
bool Dictionary::suggestions_is_empty()
{
    return suggestions.empty();
}

//--------------------------------------------------------------------------------------------------
//METHODS TO HANDLE THE CURRENT WORDS MAP
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

multimap<string, string> Dictionary::currentWords;

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

bool Dictionary::isInitialCoord(string coord)
{
    return (currentWords.find(coord) != currentWords.end());
}