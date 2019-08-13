/*
 * BSTArchiverTest.cpp
 *
 *  Created on: Aug 13, 2019
 *      Author: alex
 */

#include "BST.h"
#include <random>
#include <iostream>
using std::cout;

int main() {
	const int NUM_INSERTIONS = 10;
	std::random_device rd;
	std::mt19937 eng(rd());

	std::uniform_int_distribution<> charDistr(0,25);
	std::uniform_int_distribution<> keyDistr(0,NUM_INSERTIONS-1);
	char letters[] = "abcdefghijklmnopqrstuvwxyz";

	int key = keyDistr(eng);
	char val = letters[charDistr(eng)];
	BST<int,char> tree(key, val);
	for (int i = 1; i < NUM_INSERTIONS; ++i) {
		key = keyDistr(eng);
		val = letters[charDistr(eng)];
		tree.insert(key, val);
	}

	cout << "Printing tree (preorder): \n\n";
	tree.printPreorder();
	string out = tree.encode();
	cout << "\nEncoded tree: \n" << out << "\n";

	BST<int,char> decodedTree;
	decodedTree.decode(out);

	cout << "\nPrinting decoded tree in preorder: \n";
	decodedTree.printPreorder();

}
