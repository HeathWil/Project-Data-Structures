#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include "Header.h"
#include "LinkedList.h"



template <class T>
class HashData {
protected:

	T data;


	HashData* prev;
	HashData* next;
	HashData* prevG;
	HashData* nextG;
	int localSize = 0;
	int keyVal = 0;


public:


	HashData(const T& shipData, int midHash) {
		prev = NULL;
		next = NULL;
		prevG = NULL;
		nextG = NULL;
		keyVal = midHash;
		data = shipData;
	}


	HashData(const HashData& that) {
		data = that.data;
		prev = that.prev;
		next = that.next;
		prevG = that.prevG;
		nextG = that.nextG;
		keyVal = that.keyVal;

	}

	void operator=(const HashData& that) {
		if (this != that) {
			data = that.data;
			prev = that.prev;
			next = that.next;
			prevG = that.prevG;
			nextG = that.nextG;
			keyVal = that.keyVal;

		}
	}

	~HashData() {}


	bool isFull() {
		return localSize >= BUCKETSIZE;
	}

	void setPrev(HashData* previous) {
		prev = previous;
	}
	void setPrevG(HashData* previousG) {
		prevG = previousG;

	}
	void setNext(HashData* nextObject) {
		next = nextObject;
	}
	void setNextG(HashData* nextGlobal) {
		nextG = nextGlobal;
	}
	void setData(T dataReset) {
		data = dataReset;
	}
	HashData* getPrev() {
		return prev;
	}
	HashData* getNext() {
		return next;
	}
	HashData* getPrevG() {
		return prevG;
	}
	HashData* getNextG() {
		return nextG;
	}
	T getData() {
		return data;
	}
	int getLocalSize() {
		return localSize;
	}
	int getKey() {
		return keyVal;
	}

};




template <class T>
class Hash {
protected:

	static const int BUCKETSIZE = 5;


	int bitCounter = 0;

	HashData<T>** data;

	int size = 1;
	int elemInL = 0;
	const double LOADFACTOR = 0.75;


	//resizing at 75% capacity
	void resize() {
		elemInL = 0;
		int tempSize = size * 2;
		HashData<T>** tempData = data;
		data = new HashData<T>*[tempSize];
		for (int i = 0; i < tempSize; i++) {
			data[i] = NULL;
		}
		HashData<T> *tempHead = head, *runner;
		head = NULL;
		tail = NULL;
		overflow = NULL;
		for (int i = 0; i < size; i++) {
			while (tempHead != NULL) {
				T obj = tempHead->getData();
				runner = tempHead;
				tempHead = tempHead->getNext();
				put(obj);
				delete runner;

			}

		}


		size = tempSize;
		//deleting useless pointers
		delete[] tempData;

	}

	HashData<T>* head;
	HashData<T>* tail;
	HashData<T>* overflow;



public:


	Hash(int fileSize) {
		if (fileSize < 2) {
			fileSize = 2;
		}
		//making it so it has to resize
		fileSize /= BUCKETSIZE*LOADFACTOR;
		while (size < fileSize) {

			size *= 2;
			bitCounter++;

		}
		data = new HashData<T>*[size];
		for (int i = 0; i < size; i++) {
			data[i] = NULL;
		}

		head = NULL;
		tail = NULL;
		overflow = NULL;
	}

	Hash(const Hash& object) {
		size = that.size;
		data = new HashData<T>[size];

		for (int i = 0; i < size; i++) {
			data[i] = new HashData<T>(object.data[i]);
		}
	}

	void operator=(const Hash& that) {

		if (this != &that) {
			if (data != NULL) {
				//TODO make this a for
				//delete[] data;

			}
			size = that.size;
			data = new HashData<T>[size];

			for (int i = 0; i < size; i++) {
				data[i] = new HashData<T>(that.data[i]);
			}
		}
	}

	~Hash() {

		for (int i = 0; i < size; i++) {
			if (data[i] != NULL) {
				delete data[i];
			}
		}

	}


	//mid square hashing method
	int midHash(T object) {
		return -1;
	}

	//	using quadratric probing
	void put(const T object) {
		int tempSize = 0;


		//setting the size for a boolean function
		//int temp = (int)(size * LOADFACTOR);


		int hVal = midHash(object);

		//modulo used so we don't go out of bounds
		HashData<T>* temp = (HashData<T>*)(data[hVal]);
		HashData<T>* runner = NULL;

		if (temp != NULL) {
			while (temp != NULL && temp->getData() != object) {
				runner = temp;
				temp = temp->getNext();
				tempSize++;
			}
			if (temp == NULL) {
				if (tempSize >= BUCKETSIZE) {
					//go to overflow here
					runner = NULL;
					HashData<T>* overflowH = overflow;

					while (overflowH != NULL && overflowH->getData() != object) {

						runner = overflowH;
						overflowH = overflowH->getNext();

					}
					if (overflowH == NULL) {
						overflowH = new HashData<T>(object, hVal);
						overflowH->setPrev(runner);
						tail->setNextG(overflowH);
						overflowH->setPrevG(tail);
						tail = overflowH;
						elemInL++;
						if (overflow == NULL) {
							overflow = overflowH;
						}
						else {
							runner->setNext(overflowH);
						}

					}
					else {
						overflowH->setData(object);
					}
				}
				else {
					//if tempsize is less than bucketsize
					temp = new HashData<T>(object, hVal);
					temp->setPrev(runner);
					tail->setNextG(temp);
					temp->setPrevG(tail);
					tail = temp;
					runner->setNext(temp);
					elemInL++;

				}
			}
			//encounter end of linked list(can still grow)
			else {
				//found a match
				temp->setData(object);

			}
		}
		else {
			//no data here yet
			data[hVal] = new HashData<T>(object, hVal);

			if (tail != NULL) {
				tail->setNextG(data[hVal]);
				data[hVal]->setPrevG(tail);
			}
			elemInL++;

			tail = data[hVal];
			if (head == NULL) {
				head = data[hVal];

			}
			else {
				tail->getPrevG()->setNextG(tail);
			}

		}

		if (elemInL >= (int)(size * LOADFACTOR*BUCKETSIZE)) {
			resize();

		}

	}

	void remove(T object) {

		int tempHash = midHash(object);
		HashData<T> *no = data[tempHash], *runner;
		int ts = 0;
		while (no != NULL && no->getData() != object) {

			runner = no;
			no = no->getNext();
			ts++;
		}

		if (no!= NULL) {
			//woo we found it
			//kill it

			if (no->getPrevG() != NULL) {
				no->getPrevG()->setNextG(no->getNextG());


			}
			else {
				//object is the head
				runner = head;
				head = no->getNextG();


			}
			HashData<T>* mini = no;

			if (no->getNextG() != NULL) {
				no->getNextG()->setPrevG(no->getPrevG());
			}
			else {
				//object is the tail
				runner = tail;
				tail = no->getPrevG();
			}
			if (no->getNext() != NULL) {
				no->getNext()->setPrev(no->getPrev());
				ts++;
				while (mini->getNext() != NULL) {
					mini = mini->getNext();
					ts++;
				}
			}
			else {
				runner = tail;
				tail = no->getPrev();
			}
			if (no->getPrev() != NULL) {
				no->getPrev()->setNext(no->getNext());
			}
			else {
				runner = data[tempHash];
				data[tempHash] = runner->getNext();
			}

			
			if (ts >= BUCKETSIZE) {
				HashData<T>* ugh = overflow;
				while (ugh != NULL && midHash(ugh->getData()) != tempHash) {

					ugh = ugh->getNext();
				}
				if (ugh != NULL) {
					//found in overflow
					mini->setNext(ugh);
					if (ugh->getPrev() != NULL) {
						ugh->getPrev()->setNext(ugh->getNext());

					}
					else {
						overflow = ugh->getNext();
					}
					if (ugh->getNext() != NULL) {
						ugh->getNext()->setPrev(ugh->getPrev());

					}
					ugh->setNext(NULL);
				}
			}
			delete no;
		}
		else if (ts >= BUCKETSIZE) {
			//check overflow
			no = overflow;
			runner = NULL;
			while (no != NULL&& no->getData() != object) {

				runner = no;
				no = no->getNext();
			}
			if (no != NULL) {
				//delete this 

					//woo we found it
					//kill it

				if (no->getPrevG() != NULL) {
					no->getPrevG()->setNextG(no->getNextG());


				}
				else {
					//object is the head
					runner = head;
					head = no->getNextG();


				}

				if (no->getNextG() != NULL) {
					no->getNextG()->setPrevG(no->getPrevG());
				}
				else {
					//object is the tail
					runner = tail;
					tail = no->getPrevG();
				}
				if (no->getNext() != NULL) {
					no->getNext()->setPrev(no->getPrev());

				}
				else {
					runner = tail;
					tail = no->getPrev();
				}
				if (no->getPrev() != NULL) {
					no->getPrev()->setNext(no->getNext());
				}
				else {
					runner = overflow;
					overflow = runner->getNext();
				}


				delete no;
			}
			

		}


		elemInL--;

	}

	void merge(T object) {

		int tm = midHash(object);


		HashData<T> *tr = head, *runner;

		runner = NULL;
		while (tr != NULL && tr->getData() < object) {
			runner = tr;
			tr = tr->getNextG();
		}
		if (tr != NULL && tr->getData() == object) {
			//replace w new
			tr->setData(object);
			cout << "FOUND MATCH: " << object << endl;
		}
		else {
			// everything after is less
			HashData<T>* bn = new HashData<T>(object, tm);

			bn->setNextG(tr);
			if (tr == NULL) {
				cout << "REPLACED TAIL: " << object << endl;
				tail = bn;
			}
			else tr->setPrevG(bn);
			if (runner == NULL) {
				cout << "REPLACED HEAD: " << object << endl;
				head = bn;
			}
			else {
				runner->setNextG(bn);
			}
			bn->setPrevG(runner);

			tr = data[tm];
			runner = NULL;
			int ts = 0;

			while (tr != NULL && tr->getData() != object) {
				runner = tr;
				tr = tr->getNext();
				ts++;
			}
			if (tr == NULL) {
				if (ts >= BUCKETSIZE) {
					cout << "GOING INTO OVERFLOW: " << object << endl;
					//go to overflow here
					runner = NULL;
					HashData<T>* overflowH = overflow;

					while (overflowH != NULL) {

						runner = overflowH;
						overflowH = overflowH->getNext();

					}
					if (runner == NULL) {
						overflow = bn;
					}
					else {
						runner->setNext(bn);
					}
					if (overflowH != NULL) {
						overflowH->setPrev(bn);
					}

					bn->setNext(overflowH);
				}
				else {

					//if tempsize is less than bucketsize
					bn->setPrev(runner);
					if (runner != NULL) {
						runner->setNext(bn);
					}
					else {
						data[tm] = bn;
					}
					bn->setNext(NULL);

				}

			}
		}







	}

	T get(const T& object) {
		//find object

		int key = midHash(object);
		HashData<T>* search = data[key];
		int bucketTracker = 0;
		while (search != NULL && search->getData() != object) {
			search = search->getNext();
			bucketTracker++;
		}
		if (search == NULL) {
			if (bucketTracker == BUCKETSIZE) {


				search = overflow;
				while (search != NULL && search->getData() != object) {
					search = search->getNext();
				}
				if (search == NULL) {
					return T();
				}
				else {
					//found it in overflow
					return search->getData();
				}
			}
			else {
				return T();

			}

		}
		else {
			//found it in bucket
			return search->getData();
		}

	}

	void globalOutput() {
		HashData<T>* tempHead = head;
		while (tempHead != NULL) {
			cout << tempHead->getData() << endl;
			tempHead = tempHead->getNextG();
		}
	}

	void bucketOutput(int x) {
		if (x == 25) {
			cout << "";
		}
		HashData<T>* tempHead = data[x];
		int sizeCounter = 0;
		//TODO: be sure this is printing the bucket number (hash code) of where it is stored. 
		cout << endl;
		cout << " Bucket: " << toBinary(x) << " : ";
		if (tempHead == NULL) {
			cout << "NULL" << endl;
		}
		while (tempHead != NULL) {

			cout << tempHead->getData() << endl;
			sizeCounter++;
			tempHead = tempHead->getNext();


		}
		if (sizeCounter >= BUCKETSIZE) {
			HashData<T>* no = overflow;
			while (no != NULL) {
				if (midHash(no->getData()) == x) {
					//found a match

					cout << "OVERFLOW: " << no->getData() << endl;
				}
				no = no->getNext();

			}

		}

	}

	void printBuckets() {

		for (int i = 0; i < size; i++) {
			bucketOutput(i);
		}
	}

	string toBinary(int y) {
		string ret = "";

		while (y > 0) {
			ret.push_back((char)(y % 2 + '0'));
			y = y / 2;
		}
		while (ret.length() < bitCounter) {
			ret.push_back('0');

		}

		char ch = 0;
		for (int i = 0; i < ret.length() / 2; i++) {
			ch = ret.at(i);
			ret.at(i) = ret.at((ret.length() - 1) - i);
			ret.at((ret.length() - 1) - i) = ch;
		}
		return ret;
	}



};





inline int Hash<ShipData>::midHash(ShipData object)
{

	string title = string(object.getData()[0]);
	char temp = 0;
	unsigned val = 0;

	for (int i = 0; i < title.length(); i++) {
		temp = title.at(i);
		if (temp >= 'A' && temp <= 'Z') {
			val = val * 10 + (temp - 'A' + 1);

		}
		else if (temp >= 'a' && temp <= 'z') {
			val = val * 10 + (temp - 'a' + 1);
		}

	}


	val = val * val;
	unsigned x = val << 16;
	unsigned y = x >> (32 - bitCounter);

	return y;

}
