#ifndef PROG_PROJECT2_DICTIONARY_H
#define PROG_PROJECT2_DICTIONARY_H


#include <string>
#include <set>
#include "Logger.h"

class Dictionary
{
public:
    Dictionary();
    void load(const std::string &filename);

    bool isValid(std::string word);
    void storeSuggestions(std::string coordinates, std::string line);
    void showSuggestions();
    void clearSuggestions();
    bool suggestions_is_empty();
    void currentWords_insert(std::string coord, std::string word);
    void currentWords_erase(std::string coord);
    void currentWords_clear();
    void currentWords_send(std::ofstream &outStream);
    bool isCurrentWord(std::string word);
    bool isInitialCoord(std::string coord);

private:
    Logger logger;
    Utility utility;

	void showSynonyms(std::string word);

protected:
    std::map<std::string, std::vector<std::string>> synonymsList;
    std::set<std::string> validWords;
    std::map<std::string, std::vector<std::string>> suggestions;
    std::multimap<std::string, std::string> currentWords;
};


#endif //PROG_PROJECT2_DICTIONARY_H
