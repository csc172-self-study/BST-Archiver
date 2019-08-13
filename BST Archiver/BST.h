/*
 * BST.h
 *
 *  Created on: Jul 10, 2019
 *      Author: alex
 */

#ifndef BST_H_
#define BST_H_

#include "BSTNode.h"
#include <iostream>
#include <string>
#include <tuple>
using std::pair; using std::cout; using std::string;

template<typename Key, typename E>
class BST : protected BSTNode<Key,E> {
private:
	BSTNode<Key, E>* root;
	int numNodes;

	void clearHelp(BSTNode<Key,E>* curNode) {
		if (curNode == nullptr) return;
		if (curNode->leftIsChild) {
			clearHelp(curNode->leftChild);
		}
		if (curNode->rightIsChild) {
			clearHelp(curNode->rightChild);
		}
		delete curNode;
	}

	void removeHelp(BSTNode<Key,E>* curNode, BSTNode<Key,E>* parent) {
		if (curNode == nullptr) {
			throw "Key not found.";
		} else if (curNode->leftIsChild && curNode->rightIsChild) {
			root = delTwoChildren(curNode, parent);
		} else if (curNode->leftIsChild || curNode->rightIsChild) {
			root = delOneChild(curNode, parent);
		} else {
			root = delNoChildren(curNode, parent);
		}
	}

	pair<BSTNode<Key,E>*, BSTNode<Key,E>*> findHelp(Key key) {
		BSTNode<Key,E>* parent = nullptr;
		BSTNode<Key,E>* curNode = root;

		while (curNode != nullptr) {
			if (key == curNode->getKey()) return std::make_pair(curNode, parent);

			parent = curNode;

			if (key < curNode->getKey()) {
				if (curNode->leftIsChild) {
					curNode = curNode->leftChild;
				} else {
					break;
				}
			} else {
				if (curNode->rightIsChild) {
					curNode = curNode->rightChild;
				} else {
					break;
				}
			}
		}

		return std::make_pair(nullptr, nullptr);
	}

	// If node has no children, remove it and move its predecessor or successor to parent
	BSTNode<Key,E>* delNoChildren(BSTNode<Key,E>* curNode, BSTNode<Key,E>* parent) {
		// If we are root with no child, "delete" the tree
		if (parent == nullptr) {
			root = nullptr;
			// If we're our parent's left child, make our left child our parent's
		} else if (curNode == parent->leftChild) {
			parent->leftIsChild = false;
			parent->leftChild = curNode->leftChild;
			// If we're our parent's right child, make our right child our parent's
		} else {
			parent->rightIsChild = false;
			parent->rightChild = curNode->rightChild;
		}

		return root;
	}

	BSTNode<Key,E>* delOneChild(BSTNode<Key,E>* curNode, BSTNode<Key,E>* parent) {
		BSTNode<Key,E>* child;
		if (curNode->leftIsChild) {
			child = curNode->leftChild;
		} else {
			child = curNode->rightChild;
		}

		// Delete root, or jump over node to be deleted (if parent's child is current node, replace with current node's child)
		if (parent == nullptr) {
			root = child;
		} else if (curNode == parent->leftChild) {
			parent->leftChild = child;
		} else {
			parent->rightChild = child;
		}

		// Update parent's successor or predecessor, if necessary (if parent has only one child)
		if (parent->leftIsChild && !parent->rightIsChild) {
			parent->rightChild = inOrderSuccessor(parent);
		} else if (parent->rightIsChild && !parent->leftIsChild) {
			parent->leftChild = inOrderPredecessor(parent);
		}

		return root;
	}

	BSTNode<Key,E>* delTwoChildren(BSTNode<Key,E>* curNode, BSTNode<Key,E>* parent) {
		BSTNode<Key,E>* successorParent = curNode;
		BSTNode<Key,E>* successor = curNode->rightChild;

		// Leftmost child of current successor (minimum value of subtree) becomes current node
		std::tie(successor, successorParent) = leftmost(successor);
		if (successorParent == nullptr) successorParent = curNode;	// If successorParent returns null, the right child of the current node has no smaller children

		curNode->key = successor->key;
		curNode->value = successor->value;

		// Minimum value either has right child or no children
		if (!successor->leftIsChild && !successor->rightIsChild) {
			root = delNoChildren(successor, successorParent);
		} else {
			root = delOneChild(successor, successorParent);
		}

		return root;
	}

	BSTNode<Key,E>* inOrderSuccessor(BSTNode<Key,E>* curNode) {

		if (!curNode->rightChild->leftIsChild) {
			return curNode->rightChild;
		}
		curNode = curNode->rightChild;
		while (curNode->leftIsChild) {
			curNode = curNode->leftChild;
		}
		return curNode;
	}

	BSTNode<Key,E>* inOrderPredecessor(BSTNode<Key,E>* curNode) {

		if (curNode->leftChild != nullptr && !curNode->leftChild->rightIsChild) {
			return curNode->leftChild;
		}
		curNode = curNode->leftChild;
		while (curNode != nullptr && curNode->rightIsChild) {
			curNode = curNode->rightChild;
		}
		return curNode;
	}

	pair<BSTNode<Key,E>*, BSTNode<Key,E>*> leftmost(BSTNode<Key,E>* curNode) {
		BSTNode<Key,E>* parent = nullptr;
		// Stop if we hit the most leftmost or the leftmost of current subtree
		while (curNode->leftChild != nullptr && curNode->leftIsChild) {
			parent = curNode;
			curNode = curNode->leftChild;
		}

		return std::make_pair(curNode, parent);
	}

public:
	// Constructor. Creates a new tree.
	BST() {
		root = nullptr;
		numNodes = 0;
	}

	BST(const Key& k, const E& val) {
		root = new BSTNode<Key,E>(k, val, nullptr, nullptr);
		numNodes = 1;
	}

	BST(BSTNode<Key,E>* newRoot) {
		root = newRoot;
		numNodes = countInorder();
	}

	~BST() {
		clearHelp(root);
	}

	void insert(const Key& k, const E& val) {
		BSTNode<Key,E>* parent = nullptr;
		BSTNode<Key,E>* curNode = root;
		while (curNode != nullptr) {
			parent = curNode;

			if (k < curNode->key) {
				if (curNode->leftIsChild) {
					curNode = curNode->leftChild;
				} else {
					break;
				}
			} else {
				if (curNode->rightIsChild) {
					curNode = curNode->rightChild;
				} else {
					break;
				}
			}
		}

		BSTNode<Key,E>* temp = new BSTNode<Key,E>;
		temp->key = k;
		temp->value = val;
		if (parent == nullptr) {
			root = temp;
			root->leftChild = nullptr;
			root->rightChild = nullptr;
		} else if (k < parent->key) {
			parent->setLeft(temp);
		} else {
			parent->setRight(temp);
		}

		numNodes++;
	}

	BSTNode<Key,E>* find(const Key& key) {
		BSTNode<Key,E>* node;
		std::tie(node, std::ignore) = findHelp(key);
		return node;
	}

	void remove(const Key& key) {
		BSTNode<Key,E>* curNode;
		BSTNode<Key,E>* parent;
		std::tie(curNode, parent) = findHelp(key);
		removeHelp(curNode,parent);
	}

	// Preorder-traverse tree from given root.
	void traversePreorder(bool (*visit)(BSTNode<Key,E>*, int[2], double), int array[2], double num) {
		// Start at root
		BSTNode<Key,E>* curNode = root;

		// Check for end of tree
		while (curNode != nullptr) {
			// Visit node
			(*visit)(curNode, array, num);

			// If current node has left child, go there (and traverse left subtree)
			if (curNode->leftIsChild) {
				curNode = curNode->leftChild;
				// If current node doesn't have left child but does have right child, go there
			} else if (curNode->rightIsChild) {
				curNode = curNode->rightChild;
				// If current node has neither child, move back up to nearest un-traversed right subtree
			} else {
				while (curNode != nullptr && !curNode->rightIsChild) {
					curNode = curNode->rightChild;
				}
				if (curNode != nullptr) {
					curNode = curNode->rightChild;
				}
			}
		}
	}

	BSTNode<Key,E>* traverseInorder(BSTNode<Key,E>* (*visit)(BSTNode<Key,E>*, int[2]), int array[2]) {
		// Start at leftmost node
		BSTNode<Key,E>* curNode;
		std::tie(curNode, std::ignore) = leftmost(root);

		// Look for end of tree
		while (curNode != nullptr) {
			// Visit node
			BSTNode<Key,E>* result = (*visit)(curNode, array);
			if (result != nullptr) return result;

			// If current node has no right subtree, move back up (and visit)
			if (!curNode->rightIsChild) {
				curNode = curNode->rightChild;
				// Otherwise go to leftmost node on right subtree
			} else {
				std::tie(curNode, std::ignore) = leftmost(curNode->rightChild);
			}
		}

		return nullptr;

	}

	int countInorder() {
		int count = 0;
		// Start at leftmost node
		BSTNode<Key,E>* curNode;
		std::tie(curNode, std::ignore) = leftmost(root);

		// Look for end of tree
		while (curNode != nullptr) {
			// Visit node
			count++;

			// If current node has no right subtree, move back up (and visit)
			if (!curNode->rightIsChild) {
				curNode = curNode->rightChild;
				// Otherwise go to leftmost node on right subtree
			} else {
				std::tie(curNode, std::ignore) = leftmost(curNode->rightChild);
			}
		}

		return count;

	}

	// Preorder-traverse and print tree from given root.
	void printPreorder() {
		// Start at root
		BSTNode<Key,E>* curNode = root;
		int level = 0;

		// Check for end of tree
		while (curNode != nullptr) {
			// Visit node
			cout << curNode->getKey() << "/" << curNode->getVal() << "\n";

			// If current node has left child, go there (and traverse left subtree)
			if (curNode->leftIsChild) {
				curNode = curNode->leftChild;
				cout << "Left child: ";
				level++;
				// If current node doesn't have left child but does have right child, go there
			} else if (curNode->rightIsChild) {
				curNode = curNode->rightChild;
				cout << "Right child: ";
				level++;
				// If current node has neither child, move back up to nearest un-traversed right subtree
			} else {
				while (curNode != nullptr && !curNode->rightIsChild) {
					curNode = curNode->rightChild;
					level--;
				}
				if (curNode != nullptr) {
					curNode = curNode->rightChild;
					level++;
					cout << "Moved up to level " << level << ". Right child: ";
				}
			}
		}
	}

	// No easy way to do a postorder traversal using threads.
	// Requires always visiting the first child processed, so have to remember which that is for every node.
	// Could still do postorder traversal recursively, though.
	//void traversePostorder() {}

	int size() {
		return numNodes;
	}

	string encode() {
		string outputString = "";
		BSTNode<Key, E>* curNode = root;

		while (curNode != nullptr) {

			outputString += std::to_string(curNode->key) + curNode->value;

			if (curNode->leftIsChild) {
				curNode = curNode->leftChild;
				continue;
			} else {
				outputString += "/";
			}

			if (curNode->rightIsChild) {
				curNode = curNode->rightChild;
				continue;
			}

			if (curNode->isLeaf()) {
				while (curNode != nullptr && !curNode->rightIsChild) {
					curNode = curNode->rightChild;
					outputString += "/";
				}
				if (curNode != nullptr) {
					curNode = curNode->rightChild;
				}
			}
		}
		return outputString;
	}

	BSTNode<Key, E>* decodeHelp(string inputString, int& curr) {
		if (inputString.at(curr) == '/') {
			curr++;
			return nullptr;
		}
		Key k = inputString.at(curr++) - '0';
		E val = inputString.at(curr++);

		BSTNode<Key,E>* tempRoot = new BSTNode<Key,E>(k, val, decodeHelp(inputString, curr), decodeHelp(inputString, curr));
		// Predecessor pointers are set in setLeft or setRight, but successors don't yet exist, since construction proceeds postorder
		// Therefore, set them (if necessary) once their successor exists
		BSTNode<Key,E>* predecessor = inOrderPredecessor(tempRoot);
		if (predecessor != nullptr && !predecessor->rightIsChild) predecessor->rightChild = tempRoot;
		return tempRoot;
	}

	void decode(string inputString) {
		int curr = 0;
		root = decodeHelp(inputString, curr);
		numNodes = countInorder();
	}

};

#endif /* CITYBST_H_ */
