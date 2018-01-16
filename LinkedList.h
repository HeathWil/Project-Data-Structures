#pragma once
#include <string>
#include <iostream>
#include<iomanip>
#include "Header.h"


// may move this whole think to my header file. tbd


using namespace std;

class LinkedListException : public exception{};
class LinkedListMemoryError : public LinkedListException {};

template <class T>
class Node {
public:
	T data;
	Node* next;
	Node(T thing) {
		data = thing;
		next = NULL;
	}

	//copy constructor
	Node(const Node& that) {
		if (that.next != NULL) {
			next = new Node(that.next);
		}
		else {
			next = NULL;
		}
		data = that.data;
	}
	//destructor
	~Node() {

		if (next != NULL) {
			delete next;
			next = NULL;
		}
	}
	//override =
	void operator=(const Node& that) {
		if (this != &that) {
			data = that.data;
		}
	}


};

template <class T>
class LinkedList {

protected:

	

	Node<T>* head;
	Node <T>* current;
	Node<T>* temp;
	int size=0;
public:

	//constructor
	LinkedList() {
		head = NULL;
		current = NULL;
		temp = NULL;
	}




	//copy constructor
	LinkedList(const LinkedList& that) {
		if (that.head != NULL) {
			head = new Node(that.head);
			if (head == NULL)
			{
				throw LinkedListMemoryError();
			}
		}
		else {
			head = NULL;
		}

	}


	//copy assignment operator
	void operator=(const LinkedList& that) {
		if (this != &that) {
			if (head != NULL) {
				delete head;
				
			}

			head = new Node(that.head);
			if (head == NULL) {
				throw LinkedListMemoryError();
			}
		}
	}



	~LinkedList() {
		if (head != NULL) {
			delete head;
			head = NULL;
		}
		current = NULL;
		temp = NULL;
		
	}

	//add node method for linked list
	void addNode(const T& newData) {

		Node<T>* nde = new Node<T>(newData);
		if (nde == NULL) {
			throw LinkedListMemoryError();
		}
		nde->next = NULL;
		

		if (head != NULL) {
			//adding Node to nonempty list 
			current = head;
			while (current->next != NULL) {
				current = current->next;
			}
			current->next = nde;
		}//end of if
		else {
			//adding Node to empty list
			head = nde;
		}

		size++;
	}

	//delete node method for linked list
	bool deleteNode(const T& remove) {
		Node<T>* before;
		bool deleted = false;
		temp = head->next->next;
		before = head->next;

		while (temp->next != NULL && temp->data<remove) {
			before = temp;
			temp = temp->next;
		}
		if (temp->data == remove) {
			before->next = temp->next;
			temp->next = NULL;
			delete temp;
			temp = NULL;
			size--;
			deleted = true;

		}

		return deleted;
	}

	//prints out the linked list
	void print() {
		
		current = head;
		while (current != NULL) {
			cout << current->data << endl;
			current = current->next;
		}
	}

	

	//converts linked list into array for sorting/searching
	Resize<T>* returnArray() {
		Resize<T>* temp = new Resize<T>();
		current = head;
		while (current != NULL) {
			temp->add(current->data);
			current = current->next;
		}

		return temp;
	}

	//merges files into database, making sure there are no duplicates
	void merge(LinkedList<T> &thing) {
		Node<T>* before;
		current = thing.head->next->next;
		while (current->next != NULL) {
			temp = head->next->next;
			before = head->next;
			while (temp->next != NULL && temp->data<current->data) {
				before = temp;
				temp = temp->next;
			}
			if (temp->data == current->data) {
				temp->data = current->data;
				//TODO:number stuff
			}
			else {
				before->next = new Node<T>(current->data);
				before->next->next = temp;
				size++;
			}
			current = current->next;
		}

	}

	//purges file from database, erasing duplicates
	void purge(LinkedList<T> &thing) {
		Node<T>* before;
		Node<T>* beforeThing = thing.head->next;
		current = thing.head->next->next;
		while (current->next != NULL) {
			temp = head->next->next;
			before = head->next;
			
			while (temp->next != NULL && temp->data<current->data) {
				before = temp;
				temp = temp->next;
			}
			if (temp->data == current->data) {
				before->next = temp->next;
				temp->next = NULL;
				delete temp;
				size--;

				beforeThing->next = current->next;
				current->next = NULL;
				delete current;
				current = beforeThing;
				
				//TODO:number stuff
			}
			

			beforeThing = current;
			current = current->next;
		}
	}
	
	//removes the last line of the file (grand totals)
	void removeLast() {
		current = head;
		while (current->next != NULL) {
			temp = current;
			current = current->next;
		}
		delete current;
		size--;
		temp->next = NULL;
	}


	Node<T>* getHead() {
		return head;
	}

	Node<T>* getCurrent() {
		return current;
	}

	int getSize() {
		return size;
	}


};


//Class used to calculate grand totals of database
class Totals : public LinkedList<ShipData> {
public:
	int runningTotals[24] = { 0 };

	Totals() {
	}

	//if it contains the item, return a pointer
	Node<ShipData>* contains(const ShipData& info) {
		if (head != NULL && head->next != NULL && head->next->next != NULL) {


			Node<ShipData>* counter = head->next->next;
			while (counter != NULL && counter->data != info) {
				counter = counter->next;
			}


			return counter;
		}
		else {
			return NULL;
		}
	}


	//the toString
	ShipData calculateTotals() {
		string totals = "";
		totals += "Grand Total,";
		string num = "";
		for (int i = 0; i < 24; i++) {
			num = to_string(runningTotals[i]);
			totals += num + ",";
		}

		return ShipData(totals);

	}

	//overrides the above addNode method
	void addNode(ShipData& info) {
		if (head != NULL && head->next != NULL) {
			Node<ShipData>* temp = contains(info);
			if (temp != NULL) {
				for (int i = 0; i < 24; i++) {
					string num = temp->data.getData()[i + 1];
					if (num == "") {
						num = "0";
					}

					runningTotals[i] -= stoi(num);
				}

			}

			for (int i = 0; i < 24; i++) {

				string num2 = info.getData()[i + 1];

				if (num2 == "") {
					num2 = "0";
				}
				runningTotals[i] += stoi(num2);



			}
		}
			LinkedList<ShipData> ::addNode(info);

	}

	//kindof overrides the above deleteNode method
	bool deleteNode(ShipData& info) {
		if (head != NULL && head->next != NULL) {
			Node<ShipData>* temp = contains(info);
			if (temp != NULL) {
				for (int i = 0; i < 24; i++) {
					string num = temp->data.getData()[i + 1];
					if (num == "") {
						num = "0";
					}

					runningTotals[i] -= stoi(num);
				}
			}

		}

		return LinkedList<ShipData> ::deleteNode(info);
	}
	//overrides the merge method above
	void merge(const Totals& info) {
	
		Node<ShipData>* before;
		current = info.head->next->next;
		while (current->next != NULL) {
			temp = head->next->next;
			before = head->next;
			while (temp->next != NULL && temp->data<current->data) {
				before = temp;
				temp = temp->next;
			}
			if (temp->data == current->data) {
				for (int i = 0; i < 24; i++) {
					string num = temp->data.getData()[i + 1];
					if (num == "") {
						num = "0";
					}

					runningTotals[i] -= stoi(num);
				}
				temp->data = current->data;
				
			}
			else {
				before->next = new Node<ShipData>(current->data);
				before->next->next = temp;
				size++;
			}
			for (int i = 0; i < 24; i++) {

				string num2 = current->data.getData()[i + 1];

				if (num2 == "") {
					num2 = "0";
				}
				runningTotals[i] += stoi(num2);



			}
			current = current->next;
		}


	

	}

	//overrides the purge method above
	void purge(const Totals& info) {
		Node<ShipData>* temp = info.head->next->next;
		Node<ShipData>* runner = info.head->next;
		while (temp !=NULL) {
			
			if (deleteNode(temp->data) == true) {
				runner->next = temp->next;
				temp->next = NULL;
				delete temp;
				temp = runner;
			}
			runner = temp;
			temp = temp->next;

		}
		
	}

	

};

