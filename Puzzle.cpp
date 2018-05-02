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

    dictionary = Dictionary(inputFileName);

    cout << "Done" << endl;
    cout << "Insert the number of Rows and Columns in the board: ";
    cin >> nRows >> nCols;

    board = Board(nRows, nCols);

	dictionary.showValidWords();

    ///////////////////////////
    //board.showBoard();
    //
    //Board::coord coordinate;
    //char direction;
    //string word;
    //
    //cout << "Coordinates? Direction? ";
    //cin >> coordinate.first >> coordinate.second >> direction;
    //cout << "Word? ";
    //cin >> word;
    //board.addWord(word, coordinate, direction, 0);
    //board.showBoard();
}
void Puzzle::showMenu()
{
    unsigned char option = '0';

    while(true) {
        cout << string(8, '-') << endl;
        cout << "| MENU |" << endl;
        cout << string(8, '-') << endl;
        // The option names will be improved in future versions of this program
        cout << "1: Add a word to the board" << endl;
		cout << "2: Get a list of words suggestions" << endl;
        cout << "3: Reset board" << endl;
        cout << "4: Save to file" << endl;
        cout << "e: Exit" << endl;
        cout << "i: More information about each option" << endl;
        cout << "Select an option: " << flush;
        cin >> option;

        switch(option) {
        case '1': {
            //TODO: Check if input is valid
            Board::coord initialCoord;
            char direction;
            string word;

            cout << "Insert Line, Column and Direction (LCD): ";
            cin >> initialCoord.first >> initialCoord.second >> direction;
            cout << "Insert word to add: ";
            cin >> word;

            board.addWord(word, initialCoord, direction, 0);
            clrscr();

            cout << board;

            break;
        }
		case '2': {
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

			while (board.nextCoordinate(verCoord, horCoord)) {

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
			break;
		}

		case '3': {
			board.reset();
			clrscr();

			cout << board;
			break;
		}

        case '4': {
            ofstream out;
            string outFileName;

            cout << "Insert name of file to write: ";
            cin >> outFileName;

            out.open(outFileName);
            out << board;
            cout << "Writing finished." << endl;

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
