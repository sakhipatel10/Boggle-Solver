#pragma once
#include "Dictionary.h"
#include <time.h>
#include <fstream>

//Random initializer for 4x4 input board
void generateRandom(string board[4][4]);

void SolveBoard(string board[4][4], Dictionary& dict, Dictionary& wordsFound, bool printBoard, bool isPlayerPlaying, ofstream & outfile, string pathToFile);

//Recursive function 
void SearchforWord(int r, int c, string board[4][4], Dictionary& dict, Dictionary& wordsFound, int steps[4][4], int stepCount, string word, bool printBoard, bool isPlayerPlaying, ofstream & outfile, string pathToFile);

//Prints input board
void Print_Board(string board[4][4]);

//Prints input board with steps solution | Overloaded method
void Print_Board(string board[4][4], int steps[4][4]);

//Method initates Boggle Solver game
void PlayBoggle(string board[4][4], Dictionary& dict, Dictionary& wordsFound, int player1, int player2);

//Helper method to play Boggle Solver game
void Play(string playerOneName, string playerTwoName, Dictionary& wordsFound, string board[4][4]);

//Print to an output file
void PrintToFile(ofstream & outfile, string path, string board[4][4], int steps[4][4], Dictionary& wordsFound, string word);
