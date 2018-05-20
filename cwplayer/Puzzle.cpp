#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits>
#include "Puzzle.h"
#include "utils.h"
#include "Player.h"

using namespace std;

Puzzle::Puzzle()
    : board(0, 0)
{}

void Puzzle::greetUser()
{
    string inputFileName, playerName;
    string greeting = "CROWSSWORDS PLAYER";

    //Title
    setcolor(YELLOW);
    cout << greeting << endl;
    cout << string(greeting.length(), '=') << endl << endl;
    setcolor(WHITE);

    cout << "Insert your name: ";

    while (true) {
        cin >> playerName;

        if (!cin.fail()) { break; }
        else {
            cin.clear();
            cin.ignore(1000, '\n');
            setcolor(LIGHTRED);
            cerr << "Invalid input. Player Name ? ";
            setcolor(WHITE);
        }
    }

    player = Player(playerName);

    cout << "Hello " << playerName << "!" << endl;

    showInstructions();
}

void Puzzle::showMenu()
{
    unsigned char option = '0';

    setcolor(YELLOW);
    cout << string(11, '-') << endl;
    cout << "| OPTIONS |" << endl;
    cout << string(11, '-') << endl;
    setcolor(WHITE);

    cout << "1 - Play CrossWords" << endl;
    cout << "0 - Exit" << endl;
    cout << "Select an option: " << flush;
    cin >> option;

    switch (option) {
    case '1': {
        loadBoard();
        setcolor(WHITE);

        dictionary.calculateClues();

        player.setStartTime();
        handleAddWord();
        break;
    }
    case '0': exit(0);

    default:setcolor(LIGHTRED);
        cerr << endl << "Please insert a valid option!" << endl;
        setcolor(WHITE);
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void Puzzle::handleAddWord()
{
    COORDINATE initialCoord;
    char direction;
    string option;

    bool anotherChange = true;

    while (anotherChange) {
        dictionary.showClues();
        setcolor(WHITE);
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

        initialCoord.first = to_upper(initialCoord.first); // to_upper is defined in utils.h
        initialCoord.second = to_upper(initialCoord.second);
        direction = to_upper(direction);

        if (parseCoordinates(initialCoord.first,
                             initialCoord.second,
                             direction)) // Check if coordinates are in the board
        {
            string word;

            cout << "Insert a valid word ('e' for additional options): ";
            cin >> word;
            cout << endl;
            capitalize(word);

            insertWord(word, initialCoord.first, initialCoord.second, direction);
        }
        else {
            setcolor(LIGHTRED);
            cerr << "Invalid position." << endl;
            setcolor(WHITE);
        }

        if (boardIsFull()) {
            setcolor(LIGHTGREEN);
            cout << "Board is complete. ";
            setcolor(WHITE);
            cout <<  "Do you wish to make any more changes (yes/no) ? ";

            do {
                cin >> option;
                if (option == "yes") {
                    handleAddWord();
                }
                else if (option == "no") {
                    anotherChange = false;
                    cout << endl << "Doing a final checking of the words." << endl;

                    if (finalChecking() == "1") {
                        cout << "Every word on the board is valid." << endl;
                        checkInsertedWords();
                    }
                    else {
                        cout << "The word " << finalChecking() << " is not valid.";
                    }

                }
                else {
                    setcolor(LIGHTRED);
                    cerr << "Insert a valid option (yes/no): ";
                    setcolor(WHITE);
                }
            }
            while (option != "yes" && option != "no");
        }
    }
}

void Puzzle::insertWord(string word, char verCoord, char horCoord, char direction)
{
    string newWord;
    string coord;
    int fitsResult;

    COORDINATE initialCoord(verCoord, horCoord);

    string line = board.getLine(initialCoord.first, initialCoord.second, direction);

    coord.push_back(verCoord);
    coord.push_back(to_lower(horCoord));
    coord.push_back(direction);

    // Check if the word is in the dictionary
    if (isalpha(word[0]) && Dictionary::isValid(word)) {
        // Check if the word has the same length of the word in the board
        fitsResult = fits(word, coord);
        if (fitsResult == 0) {
            // Check if the word doesnt match any letters currently on the board
            if (!matches(word, line)) {
                setcolor(LIGHTRED);
                cerr << word << " does not match any letters on the line" << endl;
                setcolor(WHITE);
            }
            else {
                insertedWords.insert(pair<string, string>(coord, word));
                board.addWord(word, initialCoord, direction);
            }

        }
        else if (fitsResult == -1) {
            setcolor(LIGHTRED);
            cerr << word << " is too short." << endl;
            setcolor(WHITE);
        }
        else {
            setcolor(LIGHTRED);
            cout << word << " is too long." << endl;
            setcolor(WHITE);
        }
    }

    else if (word == "E") {
        cout << "Insert '-' to remove a word;" << endl;
        cout << "Insert 'R' to reset the board;" << endl;
        cout << "Insert '?' to get another random synonym different from the previous." << endl;
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
        player.increaseCounter();
        dictionary.showAnotherClue(coord);
    }

    else if (word == "R") {
        handleReset();
    }
    else {
        setcolor(WHITE);
        cout << board;
        cout << endl;

        setcolor(LIGHTRED);
        cerr << "Invalid word, nothing added." << endl;
        setcolor(WHITE);

        return;
    }

    cout << board;
    cout << endl;
    setcolor(WHITE);
}

// handleReset() calls member board function reset() to remove all words from the board (replace with .) and clears currentWords
void Puzzle::handleReset()
{
    board.reset();

    cout << board;
    cout << endl;
    setcolor(WHITE);
}

void Puzzle::loadBoard()
{
    ifstream boardFile, dictionaryFile;
    string boardFileName, dictionaryFileName, line, word;
    COORDINATE initialCoord;
    char direction;
    string option, coord;

    unsigned int nRows = 0;
    unsigned int nCols = 0;

    cout << "Insert name of file to load: ";
    cin >> boardFileName;

    player.setBoardFileName(boardFileName);

    boardFile.open(boardFileName);

    if (!boardFile.is_open()) {
        setcolor(LIGHTRED);
        cerr << "Opening of board file failed. Does it exist? " << endl;
        setcolor(WHITE);
        exit(1);
    }

    getline(boardFile, dictionaryFileName);

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

        boardWords.insert(pair<string, string>(coord, word));
        dictionary.boardWords_insert(coord, word);

        initialCoord.second = to_upper(initialCoord.second);
        board.insertWhiteCells(word, initialCoord, direction);

        coord.clear();
    }

    cout << board;
    cout << endl;
    setcolor(WHITE);
}

// parseCoordinates returns true if there is a word in the coordinate inputed by the user 
bool Puzzle::parseCoordinates(char verCoord, char horCoord, char direction)
{
    string coord;
    coord.push_back(verCoord);
    coord.push_back(to_lower(horCoord));
    coord.push_back(direction);

    if (boardWords.find(coord) == boardWords.end()) {
        return false;
    }
    else return true;
}

void Puzzle::showInstructions()
{
    cout << endl
         << "INSTRUCTIONS:" << endl
         << "In this Crosswords Puzzle Game, you are asked for the name of the file containing the board that you will try to solve."
         << endl
         << "An empty board will be shown to you containing only white and black cells."
         << endl
         << "A list of clues will be presented containing the position of each word and a correponding random synonym."
         << endl
         << "This list is divided into two section: Horizontal words and Vertical words."
         << endl
         << "For the position, you must write three characters (LCD - Line, Column and Direction) :" << endl
         << setw(3) << "The first one is the vertical coordinate and the second one is the horizontal coordinate."
         << endl
         << setw(3) << "The third one indicates the direction that you want to insert the word. Horizontal or Vertical."
         << endl
         << "You are then asked for the word itself. You have a few options to help you. Insert:"
         << endl
         << setw(3) << "A valid word to put it on the board." << endl
         << setw(3) << "The character '-' to remove the word that currently ocuppies the position you chose." << endl
         << setw(3) << "The character '?' to get another synonym for the word on the position you chose."
         << endl
         << setw(3) << "The character 'R' to reset the puzzle." << endl
         << "When the board is full you will be asked if you want to make another change. If no the game will check if you won!"
         << endl << endl;
}

int Puzzle::fits(string word, string coord)
{
    string boardWord;

    boardWord = boardWords.find(coord)->second;

    if (word.length() == boardWord.length())
        return 0;

    else if (word.length() < boardWord.length())
        return -1;
    else
        return 1;
}

bool Puzzle::matches(string word, string line)
{
    return wildcardMatch(word.c_str(), line.substr(0, word.length()).c_str());
}

bool Puzzle::boardIsFull()
{
    return insertedWords.size() == boardWords.size();
}

void Puzzle::checkInsertedWords()
{
    string option;
    if (equalMaps()) {
        player.setEndTime();
        player.congratulate();
        player.saveData();

        cout << "Do you want to play again (yes/no) ? ";

        while (true) {
            cin >> option;
            if (option == "yes") {
                showInstructions();
                loadBoard();
                dictionary.showClues();
                player.setStartTime();
                handleAddWord();
                break;
            }

            else if (option == "no") {
                exit(0);
            }
            else {
                setcolor(LIGHTRED);
                cerr << "Invalid option." << endl;
                setcolor(WHITE);
            }
        }
    }
}

string Puzzle::finalChecking()
{
    string word;
    for (const auto &it : insertedWords) {
        word = it.second;
        if (!Dictionary::isValid(word)) {
            return word;
        }
    }
    return "1"; //return "1" means that every word on the board is valid
}

bool Puzzle::equalMaps()
{
    for (const auto &it : boardWords) {
        if (it.second != insertedWords.find(it.first)->second)
            return false;
    }
    return true;
}
