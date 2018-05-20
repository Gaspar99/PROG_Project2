#ifndef PROG_PROJECT2_LOGGER_H
#define PROG_PROJECT2_LOGGER_H

#include <string>
#include <vector>
#include <map>
#include "Utility.h"

// The purpose of this class is to offload most strings from Engine.cpp so as to make the code there more readable and
// less polluted with c-outs and to handle the colorization process in a simpler and concise way. However, this class
// is not supposed to remove all c-outs from Engine.cpp, just the ones related to interfaces and errors.
class Logger : public Utility
{
public:
    Logger();

    void error(const std::string &errorKey);
    void error(const std::string &word, const std::string &errorKey);
    void log(const std::string &infoKey);
    void special(const std::string &coordinate, const std::string &errorKey);
    void header(const std::string &headerKey);
	void header(const std::string &word, const std::string &headerKey);

private:
    std::map<std::string, std::string> errors;
    std::map<std::string, std::string> info;
    std::map<std::string, std::string> headers;
};


#endif //PROG_PROJECT2_LOGGER_H
