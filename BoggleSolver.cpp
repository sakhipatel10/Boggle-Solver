#include "Dictionary.h"
#include <time.h>
#include <fstream>
#include "BoggleSolver.h"
int main()
{
	//Declaration
	string board[4][4];
	ofstream outFile;
	bool flag = false;
	Dictionary dict("dictionary.txt");
	Dictionary wordsFound = Dictionary();
	

	cout << dict.wordCount() << " No of words loaded!" << endl << endl;

	cout << "Do you intend to generate board randomly ? (y/n) " << endl;
	char randomChoice;
	cin >> randomChoice;

	if (randomChoice == 'y') 
	{
		generateRandom(board);						//method generates the 4x4 board randomly
		cout << "____________________________" << endl;
		cout << "The random board generated is as  follows: " << endl;
		Print_Board(board);
		cout << "____________________________" << endl;
	}
	else
	{
		string input;
		for (int i = 0; i < 4; i++) {
			cout << "Row [" << i << "] : ";
			for (int j = 0; j < 4; j++) {
				cin >> input;
				if (input <= "z" && input >= "a")
				board[i][j] = input;
				else {
					cout << "Not a valid Input. Only lower case letters are allowed " << endl;
					system("Pause");
					return 0;
				}
			}
		}
	}
		
		char toPlay;
		cout << "Play Boggle Solver: (y/n)" << endl;
		cin >> toPlay;

		if (toPlay == 'y' || toPlay == 'Y') {
			cout << "Thank you for choosing to play Boggle Solver" << endl;
			cout << "Enter your Choice: " << endl;
			cout << "1. Single Player" << endl;
			cout << "2. Double Players" << endl;
			int choice;
			cin >> choice;
			switch (choice) {
			case 1: {
				SolveBoard(board, dict, wordsFound, false, true, outFile, "");
				PlayBoggle(board, dict, wordsFound, 1, 0);			//single player
				break;
			}
			case 2: {
				SolveBoard(board, dict, wordsFound, false, true, outFile, "");
				PlayBoggle(board, dict, wordsFound, 1, 2);			//double players
				break;
			}
			default: {
				cout << "Invalid Choice" << endl;
			}
			}
		}
		else {
			cout << "Show Board ? (y/n): ";
			char choice;
			cin >> choice;
			string pathToFile = "";
			bool printBoard;
			if (choice == 'y' || choice == 'Y') {
				printBoard = true;
				cout << "Enter the path for the file to print (Ex: C:/File.txt) :" << endl;
				cin >> pathToFile;
			}
			else
				printBoard = false;

			SolveBoard(board, dict, wordsFound, printBoard, false, outFile, pathToFile);
		}

		system("Pause");
		return 0;
	

}

void  generateRandom(string board[4][4]) {
	char randomArr[16][6];
	srand(time(NULL)); //initializes the random seed
	for (unsigned int row = 0; row < 16; row++) {
		cout << "Dice [" << row << "]: ";
		for (unsigned int col = 0; col < 6; col++) {
			char a = 'a' + rand() % 26;
			cout << a << " ";
			randomArr[row][col] = a;
		}
		cout << endl;
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int randomCol = rand() % 6; //generates random value between 0-6
			board[i][j] = randomArr[i][randomCol];
		}
	}

	//Print_Board(board);
}


void SolveBoard(string board[4][4], Dictionary& dict, Dictionary& wordsFound, bool printBoard, bool isPlayerPlaying, ofstream & outfile, string pathToFile) {
	int steps[4][4];
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			steps[r][c] = 0;
		}
	}

	string word = "";
	int stepCount = 1;
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			SearchforWord(r, c, board, dict, wordsFound, steps, stepCount, word, printBoard, isPlayerPlaying, outfile, pathToFile);
		}
	}
}

void Print_Board(string board[4][4], int steps[4][4]) {
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			if (steps[r][c] != 0) {
				cout << "'" << board[r][c] << "'";
			}
			else {
				cout << " " << board[r][c] << " ";
			}
		}
		cout << "                   ";
		for (int c = 0; c < 4; c++) {
			cout << " " << steps[r][c] << " ";
		}
		cout << endl;
	}
}

void Print_Board(string board[4][4]) {
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			cout << board[r][c] << " ";
		}
		cout << endl;
	}
}

void PrintToFile(ofstream & outfile, string path, string board[4][4], int steps[4][4], Dictionary& wordsFound, string word)
{
	try {
		//outfile.open(path);
		outfile.open(path.c_str(), ios::out | ios::app);
		outfile << "Word Count: " << wordsFound.wordCount() << " | Word: " << word << endl;
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				if (steps[r][c] != 0) {
					outfile << "'" << board[r][c] << "'";
				}
				else {
					outfile << " " << board[r][c] << " ";
				}
			}
			outfile << "                   ";
			for (int c = 0; c < 4; c++) {
				outfile << " " << steps[r][c] << " ";
			}
			outfile << endl;
		}
		outfile << "------------------------------" << endl;
	}
	catch (const ifstream::failure& e) {
		cout << "Failure opening the file" << endl;
		exit(1);
	}
	outfile.close();
}

void PrintMatrix(int steps[4][4]) {
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			cout << steps[r][c] << " ";
		}
		cout << endl;
	}
}

void SearchforWord(int rowPos, int colPos, string board[4][4], Dictionary& dict, Dictionary& wordsFound, int steps[4][4], int stepCount, string word, bool printBoard, bool isPlayerPlaying, ofstream & outfile, string pathToFile) {

	if (rowPos >= 4 || rowPos < 0 || colPos >= 4 || colPos < 0)
		return;
	if (steps[rowPos][colPos] != 0)
		return;

	word += board[rowPos][colPos];
	if (!dict.isPrefix(word))
		return;

	steps[rowPos][colPos] = stepCount;

	if (dict.isWord(word) && !wordsFound.isWord(word))
	{
		wordsFound.addWord(word);
		if (!isPlayerPlaying && printBoard) {		//if player is not playing AND printBoard is true ==> show steps solution
			cout << "Word Count: " << wordsFound.wordCount() << " | Word: " << word << endl;
			if (pathToFile != "") {
				Print_Board(board, steps);
				PrintToFile(outfile, pathToFile, board, steps, wordsFound, word);
			}
			else
				cout << "No file path given!" << endl;
		}
		else if (!isPlayerPlaying && !printBoard) { //if player is not playing and printBoard is false => don't show steps,just show word list
			cout << wordsFound.wordCount() << ". " << word << endl;
		}
		else if (isPlayerPlaying && !printBoard) { //player is playing and printBoard is false => player is playing, don't show board
												   //do nothing
		}
	}

	//move in clockwise direction, recursively, to find the solution
	//north
	SearchforWord(rowPos - 1, colPos, board, dict, wordsFound, steps, stepCount + 1, word, printBoard, isPlayerPlaying, outfile, pathToFile);
	//north-east
	SearchforWord(rowPos - 1, colPos + 1, board, dict, wordsFound, steps, stepCount + 1, word, printBoard, isPlayerPlaying, outfile, pathToFile);
	//east
	SearchforWord(rowPos, colPos + 1, board, dict, wordsFound, steps, stepCount + 1, word, printBoard, isPlayerPlaying, outfile, pathToFile);
	//south-east
	SearchforWord(rowPos + 1, colPos + 1, board, dict, wordsFound, steps, stepCount + 1, word, printBoard, isPlayerPlaying, outfile, pathToFile);
	//south
	SearchforWord(rowPos + 1, colPos, board, dict, wordsFound, steps, stepCount + 1, word, printBoard, isPlayerPlaying, outfile, pathToFile);
	//south-west
	SearchforWord(rowPos + 1, colPos - 1, board, dict, wordsFound, steps, stepCount + 1, word, printBoard, isPlayerPlaying, outfile, pathToFile);
	//west
	SearchforWord(rowPos, colPos - 1, board, dict, wordsFound, steps, stepCount + 1, word, printBoard, isPlayerPlaying, outfile, pathToFile);
	//north-west
	SearchforWord(rowPos - 1, colPos - 1, board, dict, wordsFound, steps, stepCount + 1, word, printBoard, isPlayerPlaying, outfile, pathToFile);

	//Backtrack and find another solution
	steps[rowPos][colPos] = 0;
	return;
}

//Boggle Solver game initializer
void PlayBoggle(string board[4][4], Dictionary& dict, Dictionary& wordsFound, int player1, int player2) {
	string word;
	int score = 0;
	vector<string> playerWordList;

	cout << "Welcome To The Boggle Solver" << endl;
	cout << "============================" << endl;
	/*setlocale(LC_ALL, "");
	string b = "⏎";*/
	cout << "If you wish to exit please enter 'q' and press enter key!" << endl;
	cout << "============================" << endl;

	if (player1 != 0 && player2 != 0) {
		cout << "Enter Player One Name: " << endl;
		string playerOneName;
		cin >> playerOneName;
		cout << "Enter Player Two Name: " << endl;
		string playerTwoName;
		cin >> playerTwoName;
		cout << "============================" << endl;
		Play(playerOneName, playerTwoName, wordsFound, board);
	}
	else {
		cout << "Enter Player Name: " << endl;
		string playerOneName;
		cin >> playerOneName;
		string playerTwoName = "";
		Play(playerOneName, playerTwoName, wordsFound, board);
	}
}

//Helper method to play the Boggle Solver game
void Play(string playerOneName, string playerTwoName, Dictionary& wordsFound, string board[4][4]) {
	int playerOneScore, playerTwoScore, turn = 1;
	vector<string> playerOneWordList;
	vector<string> playerTwoWordList;
	string word;
	//cout << "Word Count: " << wordsFound.wordCount() << endl;
	if (playerOneName.length() != 0 && playerTwoName.length() == 0) {
		playerOneScore = 0;
		playerTwoScore = -1;
	}
	else {
		playerOneScore = 0;
		playerTwoScore = 0;
	}

	while (true) {
		//cout << playerOneWordList.size() << endl;
		//cout << playerTwoWordList.size() << endl;

		//check if no. of words found by player1 = total words OR
		//if no. of words found by player2 = total words OR 
		//if no. of words found by both players = Totla Words => stop the play

		if (playerOneWordList.size() == wordsFound.wordCount() ||
			playerTwoWordList.size() == wordsFound.wordCount() ||
			(playerOneWordList.size() + playerTwoWordList.size()) == wordsFound.wordCount())
		{
			cout << "All the words have been found!" << endl;
			if (playerOneScore > playerTwoScore && playerTwoScore == -1) {
				cout << playerOneName << "'s score: " << playerOneScore << endl;
				break;
			}
			else if (playerOneScore < playerTwoScore && playerTwoScore != -1) {
				cout << "Player: " << playerTwoName << " won!" << endl;
				cout << playerTwoName << "'s score: " << playerTwoScore << endl;
				cout << playerOneName << "'s score: " << playerOneScore << endl;
				break;
			}
			else if (playerOneScore == playerTwoScore && playerTwoScore != -1) {
				cout << "It was a tie!" << endl;
				cout << playerOneName << "'s score: " << playerOneScore << endl;
				cout << playerTwoName << "'s score: " << playerTwoScore << endl;
				break;
			}
			else if (playerOneScore > 0 && playerTwoScore == -1) {
				cout << "Final Score:  " << playerOneScore << endl;
				cout << "Thank you for playing!" << endl;
				break;
			}
		}

		Print_Board(board);
		if (turn == 1 && playerTwoName.length() != 0) {
			cout << "It is " << playerOneName << "'s turn!" << endl;
		}
		else if (turn == 2 && playerTwoName.length() != 0) {
			cout << "It is " << playerTwoName << "'s turn!" << endl;
		}

		cout << "Enter a word: ";
		cin >> word;
		cout << endl;
		if (word == "q") {
			cout << "Thank You for playing the Boggle Solver!" << endl << "Exiting..." << endl;
			break;
		}

		if (turn == 1) {
			if (wordsFound.isWord(word)) {
				//check if the word has been found by either of the players ==> reject
				if (std::find(playerOneWordList.begin(), playerOneWordList.end(), word) != playerOneWordList.end() ||
					std::find(playerTwoWordList.begin(), playerTwoWordList.end(), word) != playerTwoWordList.end())
				{
					cout << "The word: '" << word << "' is already found once! No props for you!" << endl;
				}
				else {
					playerOneScore++;
					cout << "'" << word << "' : is a valid word" << endl;
					playerOneWordList.push_back(word);
					cout << playerOneName << "'s Score: " << playerOneScore << endl;
					if (playerTwoScore != -1)
						cout << playerTwoName << "'s Score: " << playerTwoScore << endl;
				}
			}
			else {
				cout << "'" << word << "' is NOT a valid word" << endl;
			}

			if (playerTwoName.length() == 0)   //check if only one player is playing
												//=> Always assign turn=1 
				turn = 1;
			else
				turn = 2;					  //or else => turn = 2 => give second player chance
		}

		else if (turn == 2) {
			if (wordsFound.isWord(word)) {
				if (std::find(playerTwoWordList.begin(), playerTwoWordList.end(), word) != playerTwoWordList.end() ||
					std::find(playerOneWordList.begin(), playerOneWordList.end(), word) != playerOneWordList.end()) {
					cout << "The word: '" << word << "' is already found once! No props for you!" << endl;
				}
				else {
					playerTwoScore++;
					cout << "'" << word << "' : is a valid word" << endl;
					playerTwoWordList.push_back(word);
					cout << playerTwoName << "'s Score: " << playerTwoScore << endl;
					cout << playerOneName << "'s Score: " << playerOneScore << endl;
				}
			}
			else {
				cout << "'" << word << "' is not a valid word" << endl;
			}

			turn = 1;
		}

		cout << endl;
	}
}
