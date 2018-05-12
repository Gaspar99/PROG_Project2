#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits>
#include "Puzzle.h"
#include "utils.h"

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
        cin.ignore(1000, '\n');
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
    cout << "Board size (lines columns) ? ";
    cin >> nRows >> nCols;

    board = Board(nRows, nCols);

    board.setWriteMode(1);
    cout << board;

    handleAddWord();
}

void Puzzle::handleAddWord()
{
    Board::coord initialCoord;
    char direction;

    while (board.isNotFull()) {
        cout << "Position (LCD / CTRL-Z = STOP) ?: ";

        cin >> initialCoord.first >> initialCoord.second >> direction;

        if (cin.fail() && cin.eof()) {
            cin.clear();
            cin.ignore(1000, '\n');
            handleWrite();
            exit(0);
        }

        if (parseCoordinates(initialCoord.first, initialCoord.second, direction)) // Check if coordinates are in the board
        {
            string word;

            cout << "Insert a valid word ('e' for additional options) ";
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

<<<<<<< HEAD
    if (isalpha(word[0]) && dictionary.isValid(word)) {// Check if the word is in the dictionary
        // Check if the word is bigger than the number of lines or columns of the board depending on the direction
        if ((direction == 'V' && word.length() > (board.getNumOfRows() - verCoord + 65)) || (direction == 'H' && word.length() > (board.getNumOfCols() - horCoord + 65))) {
            cout << word << " is too long." << endl;
            return;

        } else if (dictionary.isCurrentWord(word)) {
=======
    // Check if the word is in the dictionary
    if (isalpha(word[0]) && dictionary.isValid(word)) {
        // Check if the word matches any letters currently on the board
        if (matches(word, line)) {
            // Check if the word doesnt fit in the board
            if (notFits(word, verCoord, horCoord, direction)) {
                setcolor(LIGHTRED);
                cout << word << " is too long." << endl;
                setcolor(WHITE);

            } else if (wordInMap(word)) {
                setcolor(LIGHTRED);
                cout << word << " was already inserted" << endl;
                setcolor(WHITE);

            } else {
                currentWords.insert(pair<string, string>(coord, word));
                board.addWord(word, initialCoord, direction);
            }
        } else {
>>>>>>> 2a7b17a2dede453aa5929fe4086c0544244f961d
            setcolor(LIGHTRED);
            cerr << word << " does not match any letters on the line" << endl;
            setcolor(WHITE);
<<<<<<< HEAD

        } else {

			dictionary.currentWords_insert(coord, word);

            board.addWord(word, initialCoord, direction);
=======
>>>>>>> 2a7b17a2dede453aa5929fe4086c0544244f961d
        }
    }

    else if (word == "E") {
		cout << "Insert '-' to remove a word;" << endl;
		cout << "Insert 'R' to reset the board;" << endl;
		cout << "Insert '?' to get a list of words suggestions on the position you chose;" << endl;
		cout << "Insert '?A' to get a list of words suggestions on all positions of the board;" << endl;
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
	
    dictionary.storeSuggestions(coordinates, line);

	if (!dictionary.suggestions_is_empty()) {
		dictionary.showSuggestions();

		cout << "Which one of these words do you want to add ? ";
		cin >> word;
		capitalize(word);

		if (dictionary.isValid(word)) {
			Board::coord initialCoord(verCoord, horCoord);
			board.addWord(word, initialCoord, direction);
			dictionary.currentWords_insert(coordinates, word);
		}
		else {
			cout << "Invalid word." << endl;
		}

		dictionary.clearSuggestions();
	}
	else {
		cout << "You can not form any valid word on the position " << coordinates << endl;
	}
}

void Puzzle::handleSuggestAllWords() 
{
    unsigned int nCols = board.getNumOfCols();
<<<<<<< HEAD
    char verCoord =
       (char) 64; //Character before 'A'. This way 'board.nextCoordinates' updates the coordinates to 'A' and 'a'.
    char horCoord = (char) (97 + nCols - 1);; //Last character of the row.
    string coordinates = "";
	string word;
=======
    auto verCoord = static_cast<char>(64); //Character before 'A'. This way 'board.nextCoordinates' updates the coordinates to 'A' and 'a'.
    auto horCoord = static_cast<char>(97 + nCols - 1);; //Last character of the row.
    char direction = 'H';
    string coordinates;
	string line, word;
	Board::coord initialCoord(verCoord, horCoord);
>>>>>>> 2a7b17a2dede453aa5929fe4086c0544244f961d

    while (board.nextCoordinates(verCoord, horCoord)) {
		char direction = 'H';
		
        for (int i = 0; i < 2; i++) {
<<<<<<< HEAD
            coordinates = coordinates + verCoord + horCoord + direction;
            string line = board.getLine(verCoord, horCoord, direction);
=======
            coordinates.push_back(verCoord);
            coordinates.push_back(horCoord);
            coordinates.push_back(direction);

            line = board.getLine(verCoord, horCoord, direction);
>>>>>>> 2a7b17a2dede453aa5929fe4086c0544244f961d
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
<<<<<<< HEAD
	dictionary.currentWords_clear();

=======
    currentWords.clear();

    board.setWriteMode(1);
>>>>>>> 2a7b17a2dede453aa5929fe4086c0544244f961d
    cout << board;
}

void Puzzle::handleWrite()
{
    string option;
    ofstream outStream;
    static unsigned int boardID = 0;
    ++boardID;

    ostringstream outFileName;
    outFileName << setw(3) << setfill('0') << boardID << ".txt";

    string fileName = outFileName.str();

    do {
        cout << "Is the board finished? (yes/no): ";
        cin >> option;

        if (option == "yes") {
            board.finish();
            break;

        } else if (option != "no") {
            setcolor(LIGHTRED);
            cerr << "Insert a valid option." << endl;
            setcolor(WHITE);

        }
    } while (option != "yes" || option != "no");

<<<<<<< HEAD
	unsigned int nRows = board.getNumOfRows();
	unsigned int nCols = board.getNumOfCols();

    cout << "Insert name of file to write: ";
    cin >> outFileName;
=======
    cout << "Writing to " << fileName << endl;
>>>>>>> 2a7b17a2dede453aa5929fe4086c0544244f961d

    outStream.open(fileName);
    outStream << "Words taken from: " << dictionaryFile << endl;
<<<<<<< HEAD
	outStream << nRows << " " << nCols << endl;
=======

    board.setWriteMode(0);
    outStream << board << endl;
>>>>>>> 2a7b17a2dede453aa5929fe4086c0544244f961d

	outStream << board;

	dictionary.currentWords_send(outStream);

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
    } while (option != "yes" || option != "no");
}

<<<<<<< HEAD
void Puzzle::loadPuzzle()
{
	ifstream boardFile, dictionaryFile;
	string boardFileName, dictionaryFileName, line;
	unsigned int nRows, nCols;

	cout << "Insert name of file to load board: ";
	cin >> boardFileName;

	boardFile.open(boardFileName);

	if (!boardFile.is_open()) {
		cerr << "Opening of board file failed. Does it exist? ";
		exit(1);
	}
	
	getline(boardFile, line);
	int pos = line.find(':');
	dictionaryFileName = line.substr(pos + 2, line.length() - pos - 2);

	this->dictionaryFile = dictionaryFileName;

	dictionary = Dictionary(dictionaryFileName);

	boardFile >> nRows >> nCols;
	Board board(nRows, nCols);

	for (int i = 0; i < nRows + 1; i++) {
		boardFile.ignore(1000, '\n');
	}

	while (getline(boardFile, line)) {

		if (!line.empty()) {
			Board::coord initialCoord(line[0], line[1]);
			char direction = line[2];
			string word = line.substr(4, line.length() - 4);

			board.addWord(word, initialCoord, direction);
		}
	}

	cout << board;
	handleAddWord();
}
=======
>>>>>>> 2a7b17a2dede453aa5929fe4086c0544244f961d

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
<<<<<<< HEAD
=======

bool Puzzle::wordInMap(string word)
{
    for (const auto &it : currentWords) {
        if (it.second == word)
            return true;
    }

    return false;
}

bool Puzzle::notFits(string word, char verCoord, char horCoord, char direction)
{
    return (direction == 'V' && word.length() > (board.getNumOfRows() - verCoord + 65)) ||
        (direction == 'H' && word.length() > (board.getNumOfCols() - horCoord + 65));
}

bool Puzzle::matches(string word, string line)
{
    return wildcardMatch(word.c_str(), line.substr(0, word.length()).c_str());
}
>>>>>>> 2a7b17a2dede453aa5929fe4086c0544244f961d
