#include <fstream>
#include <iomanip>
#include "Puzzle.h"
#include "utils.h"

using namespace std;

Puzzle::Puzzle() : board(0, 0)
{
	clrscr();
	greetUser();
	showMenu();
}

void Puzzle::greetUser()
{
	unsigned int nRows;
	unsigned int nCols;

	string inputFileName;
	string greeting = "CROWSSWORDS PUZZLE CREATOR";

	//Title
	cout << greeting << endl;
	cout << string(greeting.length(), '=') << endl << endl;

	showInstructions();
	cout << endl;
}

void Puzzle::showMenu()
{
	unsigned char option = '0';

	cout << string(11, '-') << endl;
	cout << "| OPTIONS |" << endl;
	cout << string(11, '-') << endl;
	
	cout << "1 - Create Puzzle" << endl;
	cout << "2 - Resume Puzzle" << endl;
	cout << "0 - Exit" << endl;
	cout << "Select an option: " << flush;
	cin >> option;

	switch (option) {
	case '1': {
		createPuzzle();
		break;
	}
	case '2': {
		//loadPuzzle();
		break;
	}
	case '0':
		exit(0);

	default:
		cerr << endl << "Please insert a valid option!" << endl;
		cin.clear();
		cin.ignore(1000, '\n');
	}
}

void Puzzle::createPuzzle()
{
	string title = "CREATE PUZZLE";
	string inputFileName;
	unsigned int nRows, nCols;

	cout << string(title.length(), '-') << endl;
	cout << title << endl;
	cout << string(title.length(), '-') << endl;

	cout << "Dictionary file name ? ";
	cin >> inputFileName;

	this->dictionaryFile = inputFileName;

	dictionary = Dictionary(inputFileName);

	cout << "Board size (lines columns) ? ";
	cin >> nRows >> nCols;

	board = Board(nRows, nCols);

	cout << board;
	handleAddWord();
}

void Puzzle::handleAddWord()
{
	//TODO: Check if input is valid
	Board::coord initialCoord;
	char direction;
	string word;

	while (true)
	{
		cout << "Position ( LCD / CTRL-Z = STOP) ?: ";
		cin >> initialCoord.first >> initialCoord.second >> direction;
		if (cin.eof())
		{
			cin.clear();
			cin.ignore(1000, '\n');
			handleWrite();
			exit(0);
		}
		 
		if (parseCoordinates(initialCoord.first, initialCoord.second, direction))
		{
			cout << "Insert word to add: ";
			cin >> word;
			capitalize(word);
			handleInsertedWord(word, initialCoord.first, initialCoord.second, direction);
		}
		else {
			cerr << "Invalid position." << endl;
		}
	}
}

void Puzzle::handleInsertedWord(string word, char verCoord, char horCoord, char direction)
{
	Board::coord initialCoord(verCoord, horCoord);

	if (isalpha(word[0])) {
		if (dictionary.isValid(word)) {
			board.addWord(word, initialCoord, direction);
		}
	}

	else if (word == "-") {
		board.removeWord(word, initialCoord, direction);
	}

	else if (word == "?") {

	}

	else if (word == "?A") {
		cout << "Processing..." << endl;
		handleSuggestAllWords();
	}

	else if (word == "R") {
		handleReset();
	}
	else {
		cerr << "Invalid word." << endl;
	}

	clrscr();
	cout << board;
}

void Puzzle::handlSuggestWords(char verCoord, char horCoord, char direction)
{
	string line;
	string coordinates = coordinates + verCoord + horCoord + direction;

	line = board.getLine(verCoord, horCoord, direction);
	dictionary.storeSuggestions(coordinates, line);
	
	dictionary.showSuggestions();
	dictionary.clearSuggestions();
}

void Puzzle::handleSuggestAllWords()
{
	unsigned int nCols = board.getNumOfCols();
	char verCoord = (char)64; //Character before 'A'. This way 'boad.nextCoordinates' updates the coordinates to 'A' and 'a'.
	char horCoord = (char)(97 + nCols - 1);; //Last character of the row. 
	char direction = 'H';
	string coordinates = "";
	string line;

	while (board.nextCoordinates(verCoord, horCoord))
	{
		for (int i = 0; i < 2; i++) {
			coordinates = coordinates + verCoord + horCoord + direction;
			line = board.getLine(verCoord, horCoord, direction);
			dictionary.storeSuggestions(coordinates, line);

			direction == 'H' ? direction = 'V' : direction = 'H'; //switches between directions
			coordinates.clear();
		}
	}

	dictionary.showSuggestions();
	dictionary.clearSuggestions();
}

void Puzzle::handleReset()
{
	board.reset();
	clrscr();

	cout << board;
}

void Puzzle::handleWrite()
{
	ofstream outStream;
	string outFileName;

	cout << "Insert name of file to write: ";
	cin >> outFileName;

	outStream.open(outFileName);
	outStream << "Words taken from: " << dictionaryFile;
	outStream << board;
	cout << "Writing finished." << endl;
}

bool Puzzle::parseCoordinates(char xCoord, char yCoord, char direction)
{
	toupper(xCoord);
	toupper(yCoord);
	toupper(direction);

	if ((unsigned int)xCoord > board.getNumOfRows() + 65 || (unsigned int)yCoord > board.getNumOfCols() + 65) {
		return false;
	}
	return true;
}

void Puzzle::showInstructions()
{
	cout << "INSTRUCTIONS:" << endl;
	cout << "In this Crosswords Puzzle Creator, you are able to create a new puzzle from scratch and save it so you can continue its creation later on." << endl;
	cout << "If you choose to create a new puzzle, you will be first asked for the number of rows and columns you want your board to have." << endl;
	cout << "Then you will be asked for the position of the first letter of the word you want to insert on the board." << endl;
	cout << "For the position, you must write three characters (LCD - Line, Column and Direction) :" << endl;
	cout << setw(3) << "The first one is the vertival coordinate and the second one is the horizontal coordinate." << endl;
	cout << setw(3) << "The third one indicates the direction that you want to insert the word. Horizontal or Vertical." << endl;
	cout << "You are then asked for the word itself. You have a few options to help you with the creation of the puzzle. Insert:" << endl;
	cout << setw(3) << "A valid word to put it on the board." << endl;
	cout << setw(3) << "The character '-' to remove the word that currently ocupies the position you chose." << endl;
	cout << setw(3) << "The character '?' to receive a list of valid words you can put on the position you chose." << endl;
	cout << setw(3) << "The characters '?A' to receive a list of all possible words, and some of theirs synonyms, you can put on all positions." << endl;
	cout << setw(3) << "The character 'R' to reset the puzzle." << endl;
	cout << "Finally, when you are finished with the puzzle you can insert 'CTRL-Z' to end the creation and save the puzzle." << endl;
}