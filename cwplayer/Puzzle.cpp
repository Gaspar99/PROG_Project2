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
    cout << string(greeting.length(), '=') << endl;
    setcolor(WHITE);

    cout << "Insert your name: ";

    while (true) {
        cin >> playerName;

        if (!cin.fail()) { break; }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
	cout << "2 - See LeaderBoard" << endl;
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
	case '2': {
		string boardName = getBoardName();
		boardName = boardName.substr(0, 4);
		showLeaderBoards(boardName);

		showMenu();
		break;
	}
    case '0': exit(0);

    default:setcolor(LIGHTRED);
        cerr << endl << "Please insert a valid option!" << endl;
        setcolor(WHITE);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

            capitalize(word);

            insertWord(word, initialCoord.first, initialCoord.second, direction);
        }
        else {
            setcolor(LIGHTRED);
            cerr << "Invalid position." << endl;
            cout << board;
            setcolor(WHITE);
        }

        if (boardIsFull()) {
            setcolor(LIGHTGREEN);
            cout << "Board is complete. Do you wish to make any changes (yes/no) ? ";
            setcolor(WHITE);

            do {
                cin >> option;
                if (option == "yes") {
                    handleAddWord();
                }
                else if (option == "no") {
                    anotherChange = false;
                    cout << "Doing a final checking of the words." << endl;

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

void Puzzle::insertWord(const string &word, char verCoord, char horCoord, char direction)
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

    cout << endl << board;
    setcolor(WHITE);
}

// handleReset() calls member board function reset() to remove all words from the board (replace with .) and clears currentWords
void Puzzle::handleReset()
{
    board.reset();

    cout << board;
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

	boardFileName = getBoardName();

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

    boardFile.ignore(numeric_limits<streamsize>::max(), '\n'); //ignores the second line of the file

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
            boardFile.ignore(numeric_limits<streamsize>::max(), '\n');
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
    setcolor(WHITE);
}

string Puzzle::getBoardName()
{
	string boardFileName;

	cout << "Insert name of file to load board: ";
	cin >> boardFileName;

	return boardFileName;
}

// parseCoordinates returns true if there is a word in the coordinate inputed by the user 
bool Puzzle::parseCoordinates(char verCoord, char horCoord, char direction)
{
    string coord;
    coord.push_back(verCoord);
    coord.push_back(to_lower(horCoord));
    coord.push_back(direction);

    return !(boardWords.find(coord) == boardWords.end());
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

int Puzzle::fits(const string &word, const string &coord)
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

bool Puzzle::matches(const string &word, const string &line)
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
		string boardName = player.getBoardName();
        player.setEndTime();
        player.congratulate();
        player.saveData();
		showLeaderBoards(boardName);

        while (true) {
			cout << "Do you want to play again (yes/no) ? ";
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

void Puzzle::showLeaderBoards(string& boardName)
{
	ifstream playersFile;
	string option;

	ostringstream outFileName;
	outFileName << boardName << "_p.txt";
	string playersFileName = outFileName.str();

	playersFile.open(playersFileName);

	if (!playersFile.is_open()) {
	    setcolor(LIGHTRED);
	    cerr << "Unable to show leaderboard." << endl;
		cerr << "No player has completed the board " << boardName << " yet." << endl;
		setcolor(WHITE);

		do {
			cout << "Do you want to try to complete it (yes/no) ? " << endl;
			cin >> option;
			if (option == "yes") {
				dictionary.calculateClues();
				player.setStartTime();

				handleAddWord();
			}
			else if (option != "no") {
				setcolor(LIGHTRED);
				cerr << "Invalid option." << endl;
				setcolor(WHITE);
			}
		} while (option != "yes" && option != "no");
		exit(0);
	}

	map<double, vector<string>> playersData;
	string line, playerName, time, counter;
	unsigned int colonPosition, semicolonPosition, counterPosition;
	double timeKey;

	while (getline(playersFile, line))
	{
		colonPosition = line.find(':');
		semicolonPosition = line.find(';');
		playerName = line.substr(colonPosition + 1, semicolonPosition - colonPosition - 1);

		counterPosition = line.find("times", semicolonPosition);
		counter = line.substr(semicolonPosition + 9, 1);

		semicolonPosition = line.find(';', counterPosition);
		time = line.substr(semicolonPosition + 9, 6);
		timeKey = atof(time.c_str());

		playersData.insert(pair<double, vector<string>>(timeKey, vector<string>()));
		playersData[timeKey].push_back(playerName);
		playersData[timeKey].push_back(counter);
		playersData[timeKey].push_back(time);
	}

	int rank = 1;
	setcolor(YELLOW);
	cout << endl << "LEADERBOARD" << endl;
	cout << right << "Rank" << setw(10) << "Name" << setw(10) << "Help" << setw(10) << "Time" << endl;
	setcolor(WHITE);

	for (const auto &it : playersData)
    {
        double currentTime = it.first;
        string name = it.second[0];

        for (const auto &vit : playersData)
            if (atof(vit.second[2].c_str()) < currentTime && vit.second[0] == name)
                playersData.erase(currentTime);
    }

    float ten = 10.0;
    float printLimit = min(ten, static_cast<float>(playersData.size()));
    float i = 0;

	for (const auto &it : playersData) {
	    if (i == printLimit) break;
		cout << rank << setw(13) << it.second[0] << setw(7) << it.second[1] << setw(14) << it.second[2] << endl;
		rank++;
		i++;
	}

}
