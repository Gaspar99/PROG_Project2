#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits>
#include "Puzzle.h"
#include "utils.h"
#include <algorithm>

using namespace std;

Puzzle::Puzzle() : board(0, 0) {}

void Puzzle::greetUser()
{
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

	setcolor(WHITE, BLACK);

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
		loadPuzzle();
		break;
	}
	case '0': exit(0);

	default:
		setcolor(LIGHTRED);
		cerr << endl << "Please insert a valid option!" << endl;
		setcolor(WHITE);
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

void Puzzle::createPuzzle()
{
	string title = "CREATE PUZZLE";
	string inputFileName;
	unsigned int nRows, nCols, titleLength = title.length();

	cout << endl << string(titleLength, '-') << endl;
	cout << title << endl;
	cout << string(titleLength, '-') << endl;

	cout << "Dictionary file name ? ";
	cin >> inputFileName;

	this->dictionaryFile = inputFileName;

	dictionary = Dictionary(inputFileName);

	while (cout << "Board size (lines columns) ? " && !(cin >> nRows >> nCols)) {
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			nRows = 0;
			nCols = 0;

			setcolor(LIGHTRED);
			cerr << "Please insert numeric values only." << endl;
			setcolor(WHITE);
		}
	}

	board = Board(nRows, nCols);

	board.setWriteMode(1);
	cout << board;

	handleAddWord();
}

void Puzzle::handleAddWord()
{
	Board::coord initialCoord;
	char direction;
	string option;
	bool anotherChange = true;

	while (anotherChange) {
		cout << "Position (LCD / CTRL-Z = STOP) ?: ";

		cin >> initialCoord.first >> initialCoord.second >> direction;

		if (cin.fail() && cin.eof()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			handleWrite();
			exit(0);
		}

		if (parseCoordinates(initialCoord.first, initialCoord.second, direction)) // Check if coordinates are in the board
		{
			string word;

			cout << "Insert a valid word ('e' for additional options): ";
			cin >> word;

			capitalize(word);
			initialCoord.first = to_upper(initialCoord.first); // to_upper is defined in utils.h
			initialCoord.second = to_upper(initialCoord.second);
			direction = to_upper(direction);

			insertWord(word, initialCoord.first, initialCoord.second, direction);
		}
		else {
			setcolor(LIGHTRED);
			cerr << "Invalid position." << endl;
			setcolor(WHITE);
		}

		if (!board.isNotFull()) {

			cout << "Board is full. Do you want to make any changes right now (yes/no) ? ";

			do {

				cin >> option;
				if (option == "yes")
					handleAddWord();

				else if (option == "no") {
					anotherChange = false;
					handleWrite();
				}
				else {
					setcolor(LIGHTRED);
					cerr << "Insert a valid option (yes/no): ";
					setcolor(WHITE);
				}

			} while (option != "yes" && option != "no");
		}
	}


}

void Puzzle::insertWord(string word, char verCoord, char horCoord, char direction)
{
	string newWord;
	string coord;

	Board::coord initialCoord(verCoord, horCoord);

	string line = board.getLine(initialCoord.first, initialCoord.second, direction);

	coord.push_back(verCoord);
	coord.push_back(to_lower(horCoord));
	coord.push_back(direction);

	// Check if the word is in the dictionary
	if (isalpha(word[0]) && dictionary.isValid(word)) {
		// Check if the word fits in the board
		if (fits(word, verCoord, horCoord, direction)) {
			// Check if the word doesnt match any letters currently on the board
			if (!matches(word, line)) {
				setcolor(LIGHTRED);
				cerr << word << " does not match any letters on the line" << endl;
				setcolor(WHITE);

			}
			else if (dictionary.isCurrentWord(word)) {
				setcolor(LIGHTRED);
				cerr << word << " was already inserted" << endl;
				setcolor(WHITE);

			}
			else {
				dictionary.currentWords_insert(coord, word);
				board.addWord(word, initialCoord, direction);
			}
		}
		else {
			setcolor(LIGHTRED);
			cerr << word << " is too long or its limiter overwrites a character from an existing word." << endl;
			setcolor(WHITE);
		}
	}

	else if (word == "E") {
		cout << "Insert '-' to remove a word;" << endl;
		cout << "Insert 'R' to reset the board;" << endl;
		cout << "Insert '?' to get a list of words suggestions on the position you chose;" << endl;
		cout << "Insert '?A' to get a list of words suggestions on all positions of the board (limited to 10);" << endl;
		cout << endl << "Option (or valid word) ? ";
		cin >> newWord;
		capitalize(newWord);

		insertWord(newWord, verCoord, horCoord, direction);
	}
	else if (word == "-") {
		board.removeWord(initialCoord, direction);
		dictionary.currentWords_erase(coord);
	}

	else if (word == "?") {
		handleSuggestWords(verCoord, horCoord, direction);
	}

	else if (word == "?A") {
		cout << "Processing..." << endl;
		handleSuggestAllWords();
	}

	else if (word == "R") {
		handleReset();
	}

	else if (word == "+") {
		checkAutomaticWords(verCoord, horCoord, direction);
	}
	else {
		board.setWriteMode(1);
		cout << board;

		setcolor(LIGHTRED);
		cerr << "Invalid word, nothing added." << endl;
		setcolor(WHITE);

		return;
	}

	board.setWriteMode(1);
	cout << board;
}

void Puzzle::handleSuggestWords(char verCoord, char horCoord, char direction)
{
	string line, word;
	string coordinates = "";

	line = board.getLine(verCoord, horCoord, direction);

	horCoord = to_lower(horCoord);
	coordinates = coordinates + verCoord + horCoord + direction;

	horCoord = to_upper(horCoord);

	dictionary.storeSuggestions(coordinates, line);

	Board::coord previousV(static_cast<const char &>(verCoord - 1), horCoord);
	Board::coord previousH(verCoord, static_cast<const char &>(horCoord - 1));

	if ((direction == 'V' && isalpha(board.getValueAt(previousV))) || (direction == 'H' && isalpha(board.getValueAt(previousH)))) {
		setcolor(LIGHTRED);
		cout << "Coordinate is too close to a letter, if you wish to add a matching word, insert a different coordinate." << endl;
		setcolor(WHITE);
		return;
	}

	if (!dictionary.suggestions_is_empty()) {
		dictionary.showSuggestions();

		cout << "Which one of these words do you want to add ? ";
		cin >> word;
		capitalize(word);

		if (fits(word, verCoord, horCoord, direction)) {
			Board::coord initialCoord(verCoord, to_upper(horCoord));

			dictionary.currentWords_insert(coordinates, word);
			board.addWord(word, initialCoord, direction);
		}
		else {
			setcolor(LIGHTRED);
			cerr << "Invalid word or its limiter overwrites a character from an existing word" << endl;
			setcolor(WHITE);
		}

		dictionary.clearSuggestions();
	}
	else {
		setcolor(LIGHTRED);
		cout << "You can not form any valid word on the position " << coordinates << endl;
		setcolor(WHITE);
	}
}

void Puzzle::handleSuggestAllWords()
{
	unsigned int nCols = board.getNumOfCols();

	string coordinates, word, line;

	auto verCoord = static_cast<char>(64); //Character before 'A'. This way 'board.nextCoordinates' updates the coordinates to 'A' and 'a'.
	auto horCoord = static_cast<char>(65 + nCols - 1);; //Last character of the row in uppercase
	char direction = 'H';

	Board::coord initialCoord(verCoord, horCoord);

	while (board.nextCoordinates(verCoord, horCoord)) {
		char direction = 'H';

		for (int i = 0; i < 2; i++) {

			coordinates.push_back(verCoord);
			coordinates.push_back(to_lower(horCoord));
			coordinates.push_back(direction);

			horCoord = to_upper(horCoord);
			line = board.getLine(verCoord, horCoord, direction);

			dictionary.storeSuggestions(coordinates, line);

			direction == 'H' ? direction = 'V' : direction = 'H'; //switches between directions
			coordinates.clear();
		}
	}

	dictionary.showSuggestions();
	dictionary.clearSuggestions();
}

// handleReset() calls member board function reset() to remove all words from the board (replace with .) and clears currentWords
void Puzzle::handleReset()
{
	board.reset();
	dictionary.currentWords_clear();

	board.setWriteMode(1);
	cout << board;
}

void Puzzle::handleWrite()
{
	string option;
	ofstream outStream;
	static unsigned int boardID = 0;
	++boardID;

	ostringstream outFileName;
	outFileName << 'b' << setw(3) << setfill('0') << boardID << ".txt";

	string fileName = outFileName.str();

	do {
		cout << "Is the board finished? (yes/no): ";
		cin >> option;

		if (option == "yes") {
			board.finish();
			break;

		}
		else if (option != "no") {
			setcolor(LIGHTRED);
			cerr << "Insert a valid option." << endl;
			setcolor(WHITE);

		}
	} while (option != "yes" && option != "no");

	cout << "Writing to " << fileName << endl;

	outStream.open(fileName);
	outStream << dictionaryFile << endl;

	board.setWriteMode(0);
	outStream << board << endl;

	dictionary.currentWords_send(outStream);
	setcolor(WHITE);
	cout << "Writing finished." << endl;

	do {
		cout << "Back to menu? (yes/no): ";
		cin >> option;

		if (option == "yes") {
			handleReset();
			showMenu();
		}
		else if (option == "no") {
			exit(0);
		}
		else {
			setcolor(LIGHTRED);
			cerr << "Insert a valid option." << endl;
			setcolor(WHITE);
		}
	} while (option != "yes" && option != "no");
}

void Puzzle::loadPuzzle()
{
	ifstream boardFile, dictionaryFile;
	string boardFileName, dictionaryFileName, line, word;
	Board::coord initialCoord;
	char direction;
	string option, coord;

	unsigned int nRows = 0;

	cout << "Insert name of file to load board: ";
	cin >> boardFileName;

	boardFile.open(boardFileName);

	if (!boardFile.is_open()) {
		setcolor(LIGHTRED);
		cerr << "Opening of board file failed. Does it exist? ";
		setcolor(WHITE);
		exit(1);
	}

	getline(boardFile, dictionaryFileName);

	this->dictionaryFile = dictionaryFileName;

	dictionary = Dictionary(dictionaryFileName);

	boardFile.ignore(1000, '\n'); //ignores the second line of the file

	while (getline(boardFile, line)) {
		if (line.empty()) { //reached the end of the board
			break;
		}
		else {
			static unsigned int nCols = line.length() / 2;
			nRows++;
		}
	}

	while (!boardFile.eof()) {

		boardFile >> initialCoord.first >> initialCoord.second >> direction >> word;

		if (boardFile.fail()) {
			boardFile.clear();
			boardFile.ignore(1000, '\n');
			break;
		}

		coord.push_back(initialCoord.first);
		coord.push_back(to_lower(initialCoord.second));
		coord.push_back(direction);

		initialCoord.second = to_upper(initialCoord.second);
		dictionary.currentWords_insert(coord, word);
		board.addWord(word, initialCoord, direction);

		coord.clear();
	}

	board.setWriteMode(1);
	cout << board;

	do {
		setcolor(WHITE);
		cout << "Do you wish to continue the creation of this board? (yes/no): ";
		cin >> option;

		if (option == "yes") {
			handleAddWord();
		}
		else if (option == "no") {
			exit(0);
		}
		else {
			setcolor(LIGHTRED);
			cerr << "Insert a valid option." << endl;
			setcolor(WHITE);
		}
	} while (option != "yes" || option != "no");
}

// parseCoordinates returns true if the coordinate input by the user is inside the board and false if it is out of bounds
bool Puzzle::parseCoordinates(char xCoord, char yCoord, char direction)
{
	return !(isdigit(xCoord) || isdigit(yCoord)) && (toupper(direction) == 'V' || toupper(direction) == 'H') &&
		(toupper(xCoord) < board.getNumOfRows() + 65 || toupper(yCoord) < board.getNumOfCols() + 65);
}

void Puzzle::showInstructions()
{
	cout << "INSTRUCTIONS:" << endl;
	cout
		<< "In this Crosswords Puzzle Creator, you are able to create a new puzzle from scratch and save it so you can continue its creation later on."
		<< endl;
	cout
		<< "If you choose to create a new puzzle, you will be first asked for the number of rows and columns you want your board to have."
		<< endl;
	cout << "Then you will be asked for the position of the first letter of the word you want to insert on the board."
		<< endl;
	cout << "For the position, you must write three characters (LCD - Line, Column and Direction) :" << endl;
	cout << setw(3) << "The first one is the vertical coordinate and the second one is the horizontal coordinate."
		<< endl;
	cout << setw(3) << "The third one indicates the direction that you want to insert the word. Horizontal or Vertical."
		<< endl;
	cout
		<< "You are then asked for the word itself. You have a few options to help you with the creation of the puzzle. Insert:"
		<< endl;
	cout << setw(3) << "A valid word to put it on the board." << endl;
	cout << setw(3) << "The character '-' to remove the word that currently ocuppies the position you chose." << endl;
	cout << setw(3) << "The character '?' to receive a list of valid words you can put on the position you chose."
		<< endl;
	cout << setw(3)
		<< "The characters '?A' to receive a list of all possible words, and some of theirs synonyms, you can put on all positions."
		<< endl;
	cout << setw(3) << "The character 'R' to reset the puzzle." << endl;
	cout
		<< "Finally, when you are finished with the puzzle you can insert 'CTRL-Z' to end the creation and save the puzzle."
		<< endl;
}

bool Puzzle::fits(string &word, char verCoord, char horCoord, char direction)
{
	Board::coord coordinate(verCoord, horCoord);
	switch (direction) {
	case 'V': {
		Board::coord previous(static_cast<const char &>(coordinate.first - 1), coordinate.second);
		Board::coord after(static_cast<const char &>(coordinate.first + word.length()), coordinate.second);

		return word.length() <= (board.getNumOfRows() - verCoord + 65) &&
			((!isalpha(board.getValueAt(after)) && !isalpha(board.getValueAt(previous))) || board.getValueAt(after) == '\0' || board.getValueAt(previous) == '\0');
	}
	case 'H': {
		Board::coord previous(coordinate.first, static_cast<const char &>(coordinate.second - 1));
		Board::coord after(coordinate.first, static_cast<const char &>(coordinate.second + word.length()));

		return word.length() <= (board.getNumOfRows() - horCoord + 65) &&
			((!isalpha(board.getValueAt(after)) && !isalpha(board.getValueAt(previous))) || board.getValueAt(after) == '\0' || board.getValueAt(previous) == '\0');
	}
	default: break;
	}
}

bool Puzzle::matches(string word, string line)
{
	return wildcardMatch(word.c_str(), line.substr(0, word.length()).c_str());
}

void Puzzle::checkAutomaticWords(char line, char column, char direction)
{
	vector<string> automaticWords;
	string word = board.getLine(line, column, direction);
	int pos;
	string coord, option, wordToAdd;

	coord.push_back(line);
	coord.push_back(to_lower(column));
	coord.push_back(direction);

	if (dictionary.isInitialCoord(coord)) {
		setcolor(LIGHTRED);
		cerr << "The position " << coord << " is already occupied." << endl;
		setcolor(WHITE);

		handleAddWord();
		exit(0);
	}

	//Eliminates the dots on the line, remaining just the word itself
	pos = word.find('.');
	while (pos != string::npos) {
		word.erase(pos);
	}	

	if (word.empty()) {
		setcolor(LIGHTRED);
		cerr << "There is not any word on the position " << coord << '.' << endl;
		setcolor(WHITE);

		handleAddWord();
		exit(0);
	}

	while (!word.empty()) 
	{
		if (dictionary.isValid(word) && !dictionary.isCurrentWord(word)) {
			automaticWords.push_back(word);
		}

		word.erase(word.length() - 1);
	}

	if (!automaticWords.empty()) {
		cout << "Valid words automatically formed on the position " << coord << ":" << endl;

		for (string autoWord : automaticWords) {
			cout << autoWord << endl;
		}

		cout << "Do you want to add any of these words to the board word list (yes/no) ? ";

		do
		{
			cin >> option;
			if (option == "yes") 
			{
				cout << "Word ? ";
				cin >> wordToAdd;

				capitalize(wordToAdd);
				if (binary_search(automaticWords.begin(), automaticWords.end(), wordToAdd)) {
					dictionary.currentWords_insert(coord, wordToAdd);
				}

				setcolor(LIGHTGREEN);
				cout << wordToAdd << " added to board words list." << endl;
				setcolor(WHITE);
			}

			else if (option == "no") {
				handleAddWord();
				exit(0);
			}
		} while (option != "yes" && option != "no");
	}

	else {
		cout << "None valid word was automatically formed on the position " << coord << "." << endl;
	}

}
