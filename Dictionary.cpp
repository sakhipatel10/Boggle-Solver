#include "Dictionary.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;


Dictionary::Dictionary()
{
	root = new NodeType;
	for (int i = 0; i < 26; i++) {
		root->next[i] = NULL;
		root->flag[i] = false;
	}
}

Dictionary::Dictionary(string file)
{
	numWords = 0;
	ifstream input_file(file);
	root = new NodeType;
	for (int i = 0; i < 26; i++) 
	{
		root->next[i] = NULL;
		root->flag[i] = false;
	}
	string word;
	while (getline(input_file, word)) {
		addWord(word);
	}
}

void Dictionary::addWord(string word)
{
	int index = 0;
	if (word.length() == 0)
		return;

	NodeType* currNode = root;
	for (unsigned int i = 0; i < word.length(); i++) {
		char c = word[i];
		int ascii_value = (int)c;
		index = ascii_value - 97;

		if (currNode->next[index] == NULL) {
			NodeType* newNode;
			newNode = new NodeType;
			for (int i = 0; i < 26; i++) {
				newNode->next[i] = NULL;
				newNode->flag[i] = false;
			}
			currNode->next[index] = newNode;
			currNode = currNode->next[index];
		}
		else {
			currNode = currNode->next[index];
		}
	}
	currNode->flag[index] = true;
	numWords++;
}

bool Dictionary::isWord(string word)
{
	int index;
	NodeType* currNode = root;

	for (unsigned int i = 0; i < word.length(); i++) {
		char c = word[i];
		int ascii_value = (int)c;
		index = ascii_value - 97;

		if (currNode->next[index] == NULL) {
			return false;
		}
		else {
			currNode = currNode->next[index];
		}
	}

	return (currNode->flag[index]);
}

bool Dictionary::isPrefix(string word)
{
	int index;
	NodeType* currNode = root;

	if (word.length() == 0)
		return false;

	for (unsigned int i = 0; i < word.length(); i++) {
		char c = word[i];
		int ascii_value = (int)c;
		index = ascii_value - 97;

		if (currNode->next[index] == NULL) {
			return false;
		}
		else {
			currNode = currNode->next[index];
		}
	}

	return true;
}

int Dictionary::wordCount()
{
	return numWords;
}
