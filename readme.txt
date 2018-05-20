Report on the programming status of the second practical work developped for the Programming module.

All goals established in the project's specification were successfully fulfilled and other specifications were included.
The organization of the code is slightly different from the one proposed in the specification and is described below
in the section for the respective program. Both programs are fully compatible with windows-based systems and unix-based
ones, as well.

CWCREATOR:
    Extra functions:
        -> Added the option to get suggestions of words for all positions at a maximum of 10 words for each position:
             - When the user is asked for the word, he/she can insert '?a' or '?A' to see all those suggestions;
        -> Added the option to check if an automatically formed word is valid and then add it to the board:
             - In answer to the question "Insert a valid word" the user can input '+' so that the program checks if there's any valid word
               that was not yet added on the position previously given by the user;
    Code organization:
        -> Besides the two classes Board and Dictionary, we added another three classes for code simplification. Those classes include:
        -> Class Engine - Responsible for controlling the interactions between the user and the program
               by showing menus and calling functions from its parent classes after processing user input if required.
               Such processing involves checks for length of the word, COORDINATE validity and word matching;
        -> Class Logger - Offloads most strings from Engine.cpp so as to make the code there more readable and
               less polluted with c-outs and to handle the colorization process in a simpler and concise way. However, this class
               is not supposed to remove all c-outs from Engine.cpp, just the ones related to interfaces and errors.
        -> Class Utility - Defines useful functions that involve manipulation of strings and chars, colorization of the
               terminal/command line and string matching.

CWPLAYER:
    Extra functions:
        -> Added the possibilty to see a leaderboard with the information about the players that completed the puzzle:
             - This leaderboard is ranked considering the time each player took to complete the puzzle.
             - In the main menu, the user can choose the option to see a leaderboard of a certain board that they can choose, or, when the user
               completes the puzzle successfully it will automatically be presented to them.
    Code organization:
             - The Dictionary class, like the one in the CWCREATOR, represents the dictionary with the main words and the corresponding synonyms stored on a map and functions to manipulate it.
             - The class Board represents the board having a map with the positions and the corresponding content and functions to manipulate it: Insertion and removal of words, for example.
             - The class Player represents the player, this is, the user, containg information like his/her name, the amount of times he/she asked for an alternative clue and the time taken to complete the puzzle.
               It also has functions to manipulate all this, for example, a fucntion to save the player's information into a file.
             - The files utilis.h and utilis.cpp have the same role as the class Utilis from the CWCREATOR.

