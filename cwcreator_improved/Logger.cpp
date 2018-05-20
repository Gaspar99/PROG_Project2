#include <iostream>
#include <sstream>
#include "Logger.h"

using namespace std;

Logger::Logger()
{
    string greeting = "CROWSSWORDS PUZZLE CREATOR";
    string options = "OPTIONS";
    string title = "CREATE PUZZLE";
    string limiter1 = string(greeting.length(), '=');
    string limiter2 = string(options.length(), '-');
    string limiter3 = string(title.length(), '-');

    ostringstream oss;
    oss << greeting << endl << limiter1;

    headers["greeting"] = oss.str();
    oss.str(string());

    oss << "INSTRUCTIONS:" << endl
        << "In this Crosswords Puzzle Creator, you are able to create a new puzzle from scratch and save it so you can continue its creation later on."
        << endl
        << "If you choose to create a new puzzle, you will be first asked for the number of rows and columns you want your board to have."
        << endl
        << "Then you will be asked for the position of the first letter of the word you want to insert on the board."
        << endl
        << "For the position, you must writeDialogue three characters (LCD - Line, Column and Direction) :" << endl
        << "The first one is the vertical COORDINATE and the second one is the horizontal COORDINATE." << endl
        << "The third one indicates the direction that you want to insert the word. Horizontal or Vertical." << endl
        << "You are then asked for the word itself. You have a few options to help you with the creation of the puzzle. Insert:"
        << endl
        << "A valid word to put it on the board." << endl
        << "The character '-' to remove the word that currently ocuppies the position you chose." << endl
        << "The character '?' to receive a list of valid words you can put on the position you chose." << endl
        << "The characters '?A' to receive a list of all possible words, and some of theirs synonyms, you can put on all positions."
        << endl
        << "The character 'R' to reset the puzzle." << endl
		<< "The character '+' to check if an automatically formed word on the given position is valid." << endl
        << "Finally, when you are finished with the puzzle you can insert 'CTRL-Z' to end the creation and save the puzzle." << endl;

    info["instructions"] = oss.str();
    oss.str(string());

    oss << limiter2 << endl
        << options << endl
        << limiter2;

    headers["options"] = oss.str();
    oss.str(string());

    oss << endl << limiter3 << endl
        << title << endl
        << limiter3 << endl;

    headers["CreatePuzzle"] = oss.str();
    oss.str(string());

	headers["AddedToList"] = " added to board words list.";

    info["options"] = "1 - Create Puzzle\n2 - Resume Puzzle\n0 - Exit\nSelect an option: ";

	oss << "Insert '-' to remove a word;" << endl
		<< "Insert 'R' to reset the board;" << endl
		<< "Insert '?' to get a list of words suggestions on the position you chose;" << endl
		<< "Insert '?A' to get a list of words suggestions on all positions of the board (limited to 10);" << endl
		<< "Insert '+' to check if an automatically formed word on the given position is valid." << endl
        << endl
        << "Option (or valid word) ? ";

    info["InfoDialogue"] = oss.str();

    info["extracting"] = "Extracting...";
    info["WriteFinished"] = "Writing finished.";

    errors["InvalidOption"] = "Please insert a valid option!";
    errors["InvalidValues"] = "Please insert numeric values only!.";
    errors["DictionaryOpenFailed"] = "Opening of dictionary file failed. Does it exist?";
    errors["InvalidPosition"] = "Invalid position.";
    errors["NoMatch"] = " does not match any letters on the line";
    errors["AlreadyInserted"] = " was already inserted";
    errors["Length"] = " is too long or its limiter overwrites a character from an existing word.";
    errors["ProximityError"] =
        "Coordinate is too close to a letter, if you wish to add a matching word, insert a different COORDINATE.";
    errors["InvalidWord"] = "Invalid word, nothing added.";
    errors["LimiterError"] = "Invalid word or its limiter overwrites a character from an existing word";
    errors["NoValidWords"] = "You can not form any valid word on the position ";
    errors["NoWord"] = "There is not any word on the position ";
    errors["NoAuto"] = "No valid word was automatically formed on the position";
    errors["AlreadyOccupied"] = " is already occupied.";
	errors["NotWordAbove"] = "Insert only one of the words shown above.";
    errors["BoardOpenFailed"] = "Opening of board file failed. Does it exist?";
}

void Logger::error(const string &errorKey)
{
    setcolor(LIGHTRED);
    cerr << errors[errorKey] << endl;
    setcolor(WHITE);
}

void Logger::error(const string &word, const string &errorKey)
{
    setcolor(LIGHTRED);
    cerr << word << errors[errorKey] << endl;
    setcolor(WHITE);
}

void Logger::log(const string &infoKey)
{
    setcolor(WHITE);
    cout << info[infoKey] << endl;
}

void Logger::special(const string &coordinate, const string &errorKey)
{
    setcolor(LIGHTRED);
    cerr << errors[errorKey] << coordinate << " ." << endl;
    setcolor(WHITE);
}

void Logger::header(const string &headerKey)
{
    setcolor(YELLOW);
    cout << headers[headerKey] << endl;
    setcolor(WHITE);
}

void Logger::header(const std::string &word, const std::string &headerKey) 
{
	setcolor(GREEN);
	cout << word << headers[headerKey] << endl;
	setcolor(WHITE);
}