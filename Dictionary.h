#pragma once
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;


struct NodeType {
	NodeType* next[26];
	bool flag[26];
};

class Dictionary
{
public:
	Dictionary();
	Dictionary(string file);
	void addWord(string word);
	bool isWord(string word);
	bool isPrefix(string word);
	int wordCount();

private:
	NodeType* root;
	int numWords;
	// Any private methods you need/want
};