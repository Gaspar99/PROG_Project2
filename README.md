# PROG_Project2
CWCREATOR:
Extra functions:
-Added the option to get suggestions of words for all positions at a maximum of 10 words for each position:
    -When the user is asked for the word, he/she can insert '?a' or '?A' to see all those suggestions;
-Added the option to check if an automatically formed word is valid and then add it to the board:
    -In answer to the question "Word ?" the user can input '+' so that the program checks if there's any valid word 
    that was not yet added on the position previously given by the user;
Code organization:
-Besides the two classes Board and Dictionary, we added another three classes for code simplification. Those classes include:
  - Class Engine - Is responsible for controlling the interactions between the user and the program
                   by showing menus and calling functions from its parent classes after processing user input if required.
                   Such processing involves checks for length of the word, COORDINATE validity and word matching.
   -Class Logger - The purpose of this class is to offload most strings from Engine.cpp so as to make the code there more readable and
                   less polluted with c-outs and to handle the colorization process in a simpler and concise way. However, this class
                   is not supposed to remove all c-outs from Engine.cpp, just the ones related to interfaces and errors.
    -Class Utility - Defines useful functions that involve manipulation of strings and chars, colorization of the
                     terminal/command line and string matching.
                     
                     
CWPLAYER:
Extra fucntions:
-Added the possibilty to see a leaderboard with the information about the players that completed the puzzle:
   -This leaderboard is ranked considering the time each player took to complete the puzzle.
   -On the main menu the user can choose the option to see a leaderboard of a certain board that he/she can choose, or, when the user
   completes the puzzle successfully it will automatically be presented to him/her.
Code organization:
