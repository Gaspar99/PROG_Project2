#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits>
#include "Puzzle.h"
#include "utils.h"
#include "Player.h"

using namespace std;

Puzzle::Puzzle() : board(0, 0) {}

void Puzzle::greetUser()
{
	string inputFileName, playerName;
    string greeting = "CROWSSWORDS PLAYER";

    //Title
    cout << greeting << endl;
    cout << string(greeting.length(), '=') << endl << endl;

	cout << "Insert you Player Name: ";
	cin >> playerName;

	player = Player(playerName);

	cout << "Hello " << playerName << "!" << endl;

	showInstructions();
    cout << endl;
}

void Puzzle::showMenu()
{
    unsigned char option = '0';

    cout << string(11, '-') << endl;
    cout << "| OPTIONS |" << endl;
    cout << string(11, '-') << endl;

    cout << "1 - Play CrossWords" << endl;
    cout << "0 - Exit" << endl;
    cout << "Select an option: " << flush;
    cin >> option;

    switch (option) {
    case '1': {
		loadBoard();
		handleAddWord();
        break;
    }
    case '0': exit(0);

    default:
        setcolor(LIGHTRED);
        cerr << endl << "Please insert a valid option!" << endl;
        setcolor(WHITE);
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void Puzzle::handleAddWord()
{
    Board::coord initialCoord;
	char direction;
	string option;

	dictionary.showClues();

    while (!boardIsFull()) {
        cout << "Position (LCD format) ?: ";

        cin >> initialCoord.first >> initialCoord.second >> direction;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
			setcolor(LIGHTRED);
			cerr << "Invalid input. " << endl;
			setcolor(WHITE);
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
    }

	cout << "Board is complete. Do you wish to make any changes (yes/no) ? ";

	do
	{
		cin >> option;
		if (option == "yes") {
			handleAddWord();
		}
		else if (option == "no") {
			//checkInsertedWords();
		}
		else {
			setcolor(LIGHTRED);
			cerr << "Insert a valid option (yes/no): ";
			setcolor(WHITE);
		}
	} while (option != "yes" || option != "no");
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
    if (isalpha(word[0])) {
        // Check if the word fits in the board
        if (fits(word, verCoord, horCoord, direction)) {
            // Check if the word doesnt match any letters currently on the board
            if (!matches(word, line)) {
                setcolor(LIGHTRED);
                cerr << word << " does not match any letters on the line" << endl;
                setcolor(WHITE);
            } else {
				insertedWords.insert(pair<string, string>(coord, word));
				board.addWord(word, initialCoord, direction);
            }
        } else {
            setcolor(LIGHTRED);
            cout << word << " is too long." << endl;
            setcolor(WHITE);
        } 
    }

    else if (word == "E") {
		cout << "Insert '-' to remove a word;" << endl;
		cout << "Insert 'R' to reset the board;" << endl;
		cout << "Insert '?' to get another synonym for the position " << coord << endl;
		cout << endl << "Option (or valid word) ? ";
		cin >> newWord;
		capitalize(newWord);

		insertWord(newWord, verCoord, horCoord, direction);
	}
    else if (word == "-") {
        board.removeWord(initialCoord, direction);
		insertedWords.erase(coord);
    }

    else if (word == "?") {
		//dictionary.showAnotherClue();
    }

    else if (word == "R") {
        handleReset();
    }
    else {
        cout << board;

        setcolor(LIGHTRED);
        cerr << "Invalid word, nothing added." << endl;
        setcolor(WHITE);

        return;
    }

    cout << board;
}

// handleReset() calls member board function reset() to remove all words from the board (replace with .) and clears currentWords
void Puzzle::handleReset()
{
    board.reset();

    cout << board;
}

void Puzzle::loadBoard()
{
	ifstream boardFile, dictionaryFile;
	string boardFileName, dictionaryFileName, line, word;
	Board::coord initialCoord;
	char direction;
	string option, coord;

	unsigned int nRows = 0;
	unsigned int nCols;

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
			nCols = line.length() / 2;
			nRows++;
		}
	}

	board = Board(nRows, nCols);

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

		dictionary.currentWords_insert(coord, word);

		initialCoord.second = to_upper(initialCoord.second);
		board.insertBlackCells(word, initialCoord, direction);

		coord.clear();
	}

	cout << board;
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

bool Puzzle::fits(string word, char verCoord, char horCoord, char direction)
{
    return !((direction == 'V' && word.length() > (board.getNumOfRows() - verCoord + 65)) ||
        (direction == 'H' && word.length() > (board.getNumOfCols() - horCoord + 65)));
}

bool Puzzle::matches(string word, string line)
{
    return wildcardMatch(word.c_str(), line.substr(0, word.length()).c_str());
}

bool Puzzle::boardIsFull()
{
	if (insertedWords.size() == dictionary.currentWords_size()) {
		return true;
	}
	else {
		return false;
	}

}


