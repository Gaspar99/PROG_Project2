#include <fstream>
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

    //Unsure if this is the right approach
    cout << "Insert the name of the file containing the dictionary: ";

    cin >> inputFileName;
    this->dictionaryFile = inputFileName;

    dictionary = Dictionary(inputFileName);

    cout << "Done" << endl << endl;
    cout << "Insert the number of Rows and Columns in the board: ";
    cin >> nRows >> nCols;

    board = Board(nRows, nCols);

}

void Puzzle::showMenu()
{
    unsigned char option = '0';

    while(true) {
        cout << string(8, '-') << endl;
        cout << "| MENU |" << endl;
        cout << string(8, '-') << endl;
        // TODO: The option names will be improved in future versions of this program
        cout << "1: Add a word to the board" << endl;
		cout << "2: Get a list of word suggestions" << endl;
        cout << "3: Reset board" << endl;
        cout << "4: Save to file" << endl;
        cout << "e: Exit" << endl;
        cout << "i: More information about each option" << endl;
        cout << "Select an option: " << flush;
        cin >> option;

        switch(option) {
        case '1': {
            handleAddWord();
            break;
        }
		case '2': {
		    handleSuggestWords();
			break;
		}

		case '3': {
			handleReset();
			break;
		}

        case '4': {
            handleWrite();
            break;
        }
        case 'e':
            exit(0);

        default:
            cerr << endl << "Please insert a valid option!" << endl;
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

void Puzzle::handleAddWord()
{
    //TODO: Check if input is valid
    Board::coord initialCoord;
    char direction;
    string word;

    cout << "Insert Line, Column and Direction (LCD): ";
    cin >> initialCoord.first >> initialCoord.second >> direction;

    cin.clear();
    cin.ignore(1000, '\n');

    parseCoordinates(initialCoord.first, initialCoord.second, direction);

    cout << "Insert word to add: ";
    cin >> word;
    capitalize(word);

    board.addWord(word, initialCoord, direction);
    clrscr();

    cout << board;
}

void Puzzle::handleSuggestWords()
{
    char verCoord = 'A';
    char horCoord = 'a';
    string coordinates = "";
    string line;

    coordinates = coordinates + verCoord + horCoord + 'V';
    line = board.column(verCoord, horCoord);
    dictionary.storeSuggestions(coordinates, line);

    coordinates.clear();
    coordinates = coordinates + verCoord + horCoord + 'H';
    line = board.row(verCoord, horCoord);
    dictionary.storeSuggestions(coordinates, line);

    while (board.nextCoordinates(verCoord, horCoord)) {

        coordinates.clear();
        coordinates = coordinates + verCoord + horCoord + 'V';
        line = board.column(verCoord, horCoord);
        dictionary.storeSuggestions(coordinates, line);

        coordinates.clear();
        coordinates = coordinates + verCoord + horCoord + 'H';
        line = board.row(verCoord, horCoord);
        dictionary.storeSuggestions(coordinates, line);
    }

    dictionary.showSuggestions();
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

    if (xCoord > board.getRows() + 65 || yCoord > board.getColumns() + 65) {
        cout << "Invalid coordinates" << endl;
        return false;
    }
}
