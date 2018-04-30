#include "Dictionary.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

void capitalize(string &word)
{
	transform(word.begin(), word.end(), word.begin(), toupper);
}

Dictionary::Dictionary()
{
	synonymsList[""] = { "" };
	validWords = { "" };
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

bool Dictionary::isValid(string word) {
	bool valid;

	sort(validWords.begin(), validWords.end());
	valid = binary_search(validWords.begin(), validWords.end(), word);

	if (valid) { return true; }
	else { return false; }
}

