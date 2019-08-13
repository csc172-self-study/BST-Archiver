/*
 * BSTNode.h
 *
 *  Created on: Jul 12, 2019
 *      Author: alex
 */

#ifndef BSTNODE_H_
#define BSTNODE_H_

#include <iostream>

template <typename Key, typename E>
class BSTNode {
public:
	Key key;
	E value;
	bool leftIsChild = false;
	bool rightIsChild = false;
	BSTNode<Key, E>* leftChild;
	BSTNode<Key, E>* rightChild;

public:
	~BSTNode() {}

	// Default constructor
	BSTNode() {
		// Null constructor has no predecessor or successor
		leftChild = rightChild = nullptr;
	}

	// Constructor. Sets key, value, and left and right child
	BSTNode(Key k, E val, BSTNode<Key, E>* left_child, BSTNode<Key, E>* right_child) {
		key = k;
		value = val;
		setLeft(left_child);
		setRight(right_child);
	}

	// Return node value
	E& getVal() {
		return value;
	}

	// Set node value
	void setVal(const E& val) {
		value = val;
	}

	// Return node key
	Key& getKey() {
		return key;
	}

	// Set node key
	void setKey(const Key& k) {
		key = k;
	}

	// Return left child
	BSTNode* getLeft() {
		return leftChild;
	}

	// Return right child
	BSTNode* getRight() {
		return rightChild;
	}

	// Set left child (and appropriate threads). Use with care.
	void setLeft(BSTNode<Key,E>* left_child) {
		if (left_child != nullptr) {
			// Prevent clobbering of existing children
			if (leftChild != nullptr) {
				left_child->leftChild = this->leftChild;
				left_child->leftIsChild = this->leftIsChild;
			}
			// Prevent clobbering of existing children
			if (!left_child->rightIsChild) {
				left_child->rightChild = this;
				left_child->rightIsChild = false;
			}
			// Set new child
			leftChild = left_child;
			leftIsChild = true;
		} else {
			left_child = nullptr;
		}
	}

	// Set right child (and appropriate threads). Use with care.
	void setRight(BSTNode<Key,E>* right_child) {
		if (right_child != nullptr) {
			// Prevent clobbering of existing children
			if (!right_child->leftIsChild) {
				right_child->leftChild = this;
				right_child->leftIsChild = false;
			}
			// Prevent clobbering of existing children
			if (rightChild != nullptr) {
				right_child->rightChild = this->rightChild;
				right_child->rightIsChild = this->rightIsChild;
			}
			// Set new child
			rightChild = right_child;
			rightIsChild = true;
		} else {
			right_child = nullptr;
		}
	}

	// False if either left or right is an actual child (not a thread)
	bool isLeaf() {
		return !(leftIsChild || rightIsChild);
	}
};

#endif /* BSTNODE_H_ */
