# PortfolioProject2019
CS201: Portfolio Project of Connect Four

This project is my version of Connect Four in C prgogramming language. 
The game is played with arbitrary board size based on user preference; however, anything above 
an estimate of 80 dimension will start to distort screen. 
The standard size board is 7 x 6, but the user may enter any number for the dimensions. 
There are two modes to the game: single mode (human player vs. computer) and double mode (human player vs. human player).
The game keeps track of each player's score (including the computer) and is displayed after each game ie. a winner has been announced, until the user decides to exit by entering 0 for the dimension of columns.
 
 General Notes about the Game to User:
 1) To exit out of program, you must enter "0" in for the opening prompt: Enter number of columns.
 2) You will be asked to enter the desired number of columns and then asked to enter the desired number of rows. If user attempts to enter anything other than an integer, it will be considered invalid and the game will end.  Note: For the prompt "Enter number of columns", if you enter 7 6 (enter) it will assume you meant 7 for columns and 6 for the next prompt, which would be rows.  Also, it is not logical to enter 0 for rows and the program will end in this case. 
 3) When choosing options for single(1) or double(2) mode, until you enter in a "1" or "2", the program will continue to prompt the user.
 4) The scoreboard keeps track of all game scores until the user exits out of the program. The score will display after each game.
 5) If the user does not enter in a valid column, the program will just keep prompting the user until a valid column has been entered.
 6) If the user enters a column number that is already full, the game will terminate and the program will exit because foul play will not be tolerated.
 7) The game board is represented by "0's" (empty cell slots).
 
 Notes about Single(1) Mode:
 1) Single mode means the user will play against the computer.
 2) The computer will always go second out of courtesy to the user and will be marked by "2".
 
 Notes about Double(2) Mode:
 1) Player 1 will always go first.
 
Compiling: There is only one .c file. It is named "FinalConnect.c". Any compilation on a typical compiler for C should suffice for running the game successfully; however, there is a Makefile. In terminal, go to wherever you downloaded the repository and then enter in "make". Then ./PortfolioProject
It is ascii interface based.

Built with Visual Studio Code.

Acknowledgments: https://www.techiedelight.com/implement-graph-data-structure-c/ helped form an idea and structure of graph implementation.

Link to Video Demonstration: https://youtu.be/ziZ9u1ChxLM


 
