#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include "ShipData.h"
#include "Header.h"
#include "LinkedList.h"

using namespace std;

class TreeException : public exception {};
class TreeMemoryError : public TreeException {};



template <class T>
class LeafNode {
public:
	int balance;
	LeafNode *left, *right, *proot;
	T data;



	LeafNode(T data1, LeafNode<T> *thing) {
		data = data1;
		left = NULL;
		right = NULL;
		proot = thing;
		balance = 0;
	}

	//copy constructor
	LeafNode(const LeafNode& that) {
		if (that.left != NULL) {
			left = new LeafNode(that.left);
		}
		else {
			left = NULL;
		}
		if (that.right != NULL) {
			right = new LeafNode(that.right);

		}
		else {
			right = NULL;
		}

		data = that.data;
		balance = that.balance;
		proot = that.proot;
	}

	~LeafNode() {

		if (left != NULL) {
			delete left;
			left = NULL;
		}
		if (right != NULL) {
			delete right;
			right = NULL;
		}

	}

	void operator=(const LeafNode& that) {
		if (this != &that) {
			if (that.left != NULL) {
				left = new LeafNode(that.left);
				if (left == NULL) {
					throw TreeMemoryError();
				}
			}
			else {
				left = NULL;
			}
			if (that.right != NULL) {
				right = new LeafNode(that.right);
				if (right == NULL) {
					throw TreeMemoryError();
				}

			}
			else {
				right = NULL;
			}
			if (that.proot != NULL) {
				proot = new LeafNode(that.proot);
				if (proot == NULL) {
					throw TreeMemoryError();
				}

			}
			else {
				proot = NULL;
			}

			data = that.data;
			balance = that.balance;

		}
	}

	void setBalanced(int val) {
		balance = val;
	}
};





template <class T>
class Tree {

protected:

	LeafNode<T> *root;
public:

	Tree() {
		root = NULL;

	}
	//copy assignment operator
	Tree(const Tree& that) {
		if (that.root == NULL) {
			root = NULL;
		}
		else {
			root = new LeafNode<T>(that.root);
			if (root == NULL) {
				throw TreeMemoryError();
			}
		}
	}

	//operator =
	void operator=(const Tree& that) {
		if (this != that) {
			if (that.root == NULL) {
				root = NULL;
			}
			else {
				root = new LeafNode<T>(that.root);
				if (root == NULL) {
					throw TreeMemoryError();
				}
			}
		}
	}


	//destructor
	~Tree() {
		delete root;
	}





	void rebalance(LeafNode<T> *leaf) {
		if (!leaf) return;

		setBalance(leaf);
		if (leaf->balance == -2) {
			//unbalanced to the left
			if (leaf->left->balance < 0) {
				//left heavy left 
				rotateRight(leaf->left);

			}
			else {
				//left right
				rotateLeft(leaf->left->right);
				rotateRight(leaf->left);

			}


		}
		else if (leaf->balance == 2) {
			//unbalanced to the right
			if (leaf->right->balance > 0) {
				//right heavy 
				rotateLeft(leaf->right);

			}
			else {
				//right left
				rotateRight(leaf->right->left);
				rotateLeft(leaf->right);
			}

		}
		if (leaf->proot == NULL) {
			root = leaf;
		}

	}

	void rotateLeft(LeafNode<T> *problem) {
		LeafNode<T> *parent = problem->proot;
		problem->proot = parent->proot;
		if (problem->proot != NULL) {
			if (parent == problem->proot->left) {
				problem->proot->left = problem;
			}
			else {
				problem->proot->right = problem;
			}
		}
		parent->proot = problem;
		parent->right = problem->left;
		problem->left = parent;

		if (parent->right != NULL) {
			parent->right->proot = parent;
		}

		setBalance(problem);
		setBalance(parent);
		//cout << "rotated left " << problem->data << endl;
		
	}

	void rotateRight(LeafNode<T> *problem) {
		LeafNode<T> *parent = problem->proot;
		problem->proot = parent->proot;
		if (problem->proot != NULL) {
			if (parent == problem->proot->left) {
				problem->proot->left = problem;
			}
			else {
				problem->proot->right = problem;
			}
		}
		parent->proot = problem;
		parent->left = problem->right;
		problem->right = parent;


		if (parent->left != NULL) {
			parent->left->proot = parent;

		}


		setBalance(problem);
		setBalance(parent);
		//cout << "rotated right " << problem->data << endl;

	}

	void rotateL2R(LeafNode<T> *leaf) {
		rotateLeft(leaf);
		rotateRight(leaf);

	}

	void rotateR2L(LeafNode<T> *leaf) {
		rotateRight(leaf);
		rotateLeft(leaf);

	}

	int height(LeafNode<T> *leaf) {
		if (leaf == NULL) {
			return -1;
		}
		int rhett = 1 + max(height(leaf->left), height(leaf->right));

		return rhett;
	}

	void setBalance(LeafNode<T> *leaf) {
		//height of the right side minus the left

		leaf->balance = height(leaf->right) - height(leaf->left);
	}


	void add(T newData) {
		if (root == NULL) {
			root = new LeafNode<T>(newData, NULL);
			if (root == NULL) {
				throw TreeMemoryError();
			}
		}
		else {
			LeafNode<T> *temproot = root;
			LeafNode<T> *runner = root;
			while (temproot != NULL) {
				runner = temproot;
				if (temproot->data == newData) {
					temproot->data = newData;
					return;
				}
				
				if (newData < temproot->data) {
					//go left
					temproot = temproot->left;
				}
				else {
					//go right
					temproot = temproot->right;
				}

			}
			temproot = new LeafNode<T>(newData, runner);
			if (temproot == NULL) {
				throw TreeMemoryError();
			}
			if (newData < runner->data) {
				//go left
				runner->left = temproot;
					
			}
			else {
				//go right
				runner->right = temproot;
				
			}

		
			while (temproot != NULL) {
				rebalance(temproot);
				temproot = temproot->proot;
			}
		}

		//printInOrder();

	}

	void remove(T del) {

		LeafNode<T> *temp = NULL;
		if (root == NULL) {
			//break
			return;
		}
		else {
			LeafNode<T> *thing2del = search(del);
			LeafNode<T> *compare = NULL;
			if (thing2del == NULL) {
				//thing not found
				//error message
				cout << del << " was not found." << endl;
				return;
			}
			else {
				//found it. kill it.

				if (thing2del->left != NULL) {
					//we have a left child


					if (thing2del->right != NULL) {
						//we have 2 children
						//recursive case

						compare = thing2del->right;
						while (compare->left != NULL) {
							compare = compare->left;
						}
						//compare is the next sequential value in the tree

						T retainData = compare->data;
						remove(retainData);
						thing2del->data = retainData;

					}




					else {
						//shift left pointer up, delete node
						thing2del->left->proot = thing2del->proot;
						if (thing2del == thing2del->proot->right) {
							//right subchild of the parent
							thing2del->proot->right = thing2del->left;

						}
						else {
							thing2del->proot->left = thing2del->left;
						}
						thing2del->left = NULL;
						thing2del->right = NULL;
						temp = thing2del->left;
						delete thing2del;
					}
					

				}
				
				else if (thing2del->right != NULL) {

					if (thing2del == thing2del->proot->right) {

						thing2del->proot->right = thing2del->right;
					}
					else {
						thing2del->proot->left=thing2del->right;
					}
						thing2del->right->proot = thing2del->proot;
						thing2del->left = NULL;
						thing2del->right = NULL;
						temp = thing2del->right;
						delete thing2del;
				}


				else {
					//end of tree
					if (thing2del == root) {
						delete thing2del;
						root = NULL;
					}
					else if (thing2del->proot->right == thing2del) {
						thing2del->proot->right = NULL;
						temp = thing2del->proot;
						delete thing2del;
					}
					else {
						thing2del->proot->left = NULL;
						temp = thing2del->proot;
						delete thing2del;
					}
				}

				
				if (temp != NULL) {
					setBalance(temp);


					do {

						rebalance(temp = temp->proot);

					} while (temp->proot != NULL);

				}

			}

		}


		//printInOrder();


	}


	int max(int a, int b) {

		return ((a < b) ? b : a);
	}

	LeafNode<T>* search(const T& data) {
		LeafNode<T> *leaf = root;
		if (root == NULL) {
			return NULL;
		}
		while (leaf->data != data && !(leaf->left == NULL && leaf->right == NULL)) {

			
			if (leaf->data > data) {
				//go left
				if (leaf->left == NULL) {
					break;
				}
				leaf = leaf->left;
				
			}
			else {
				//go right
				if (leaf->right == NULL) {
					break;
				}
				leaf = leaf->right;
			}
		}
		//found data or end of tree
		if (leaf->data == data) {
			return leaf;
		}

		else {
			return NULL;
		}
	}//end of search

	void printPreOrder() {
		_printPreO(root);
		
	}
	void printPostOrder() {
		
		_printPosO(root);
	}
	void printInOrder() {

		_printIO(root);
	}

	void _printIO(LeafNode<T> *node) {
	
		if (!node) return;

		_printIO(node->left);
		cout << node->data << endl;
		_printIO(node->right);


	}
	void _printPreO(LeafNode<T> *node) {
		if (!node) return;

		cout << node->data << endl;
		_printPreO(node->left);
		_printPreO(node->right);
	}

	void _printPosO(LeafNode<T> *node) {

		if (!node) return;

		_printPosO(node->left);
		_printPosO(node->right);
		cout << node->data << endl;

	}

	LeafNode<T>* getRoot() {
		return root;
	}
};






//class TreeTotals : public Tree<ShipData> {
//public:
//	int runningTotals[24] = { 0 };
//
//	TreeTotals() {
//
//	}
//
//	LeafNode<ShipData>* contains(const ShipData& info) {
//		if (root != NULL && (root->left != NULL || root->right != NULL) && ((root->left->left != NULL || root->left->right != NULL)
//			|| (root->right->left != NULL || root->right->right != NULL))) {
//			//AH
//			LeafNode<ShipData>* counter;
//			if (root->left->left != NULL) counter = root->left->left;
//			else if (root->left->right != NULL) counter = root->left->right;
//			else if (root->right->left != NULL) counter = root->right->left;
//			else if (root->right->right != NULL) counter = root->right->right;
//			else counter = NULL;
//
//
//			while (counter != NULL && counter->data != info) {
//				if (counter->left != NULL) {
//					counter = counter->left;
//				}
//				else  {
//					counter = counter->right;
//				}
//				
//			}
//
//			return counter;
//		
//
//		}
//		else {
//			return NULL;
//		}
//
//	}
//
//	ShipData calculateTotals() {
//		string totals = "";
//		totals += "Grand Total,";
//		string num = "";
//		for (int i = 0; i < 24; i++) {
//			num = to_string(runningTotals[i]);
//			totals += num + ",";
//		}
//		return ShipData(totals);
//	}
//
//
//	//overrides add method above
//	void add(ShipData& info) {
//		//what if both aren't null? Still true?
//		if (root != NULL && (root->left != NULL || root->right != NULL)) {
//			LeafNode<ShipData>* temp = contains(info);
//			if (temp != NULL) {
//				for (int i = 0; i < 24; i++) {
//					string num = temp->data.getData()[i + 1];
//					if (num == "") {
//						num = "0";
//					}
//					runningTotals[i] -= stoi(num);
//
//				}
//
//			}
//
//
//			for (int i = 0; i < 24; i++) {
//				string num2 = info.getData()[i + 1];
//
//				if (num2 == "") {
//					num2 = "0";
//
//				}
//
//				runningTotals[i] += stoi(num2);
//			}
//		}
//
//		Tree<ShipData> ::add(info);
//	}
//
//	//kind of overrides remove above
//	bool remove(ShipData& info) {
//
//
//
//		Tree<ShipData>::remove(info);
//	}
//};
//
