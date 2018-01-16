#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include "ShipData.h"
#include "LinkedList.h"


using namespace std;

//int current;
//int size;

bool mathCheck(string columns[]);
class ResizeException : public exception {};
class ResizeMemoryError : public ResizeException {};



template <class T>
class Resize {


private:
	T * file;
	int size = 10;
	int current = 0;

	//quicksort method
	void qSort(int low, int high) {
		if (high>low) {
			T pivot = file[(low + high) / 2];
			int a = low;
			int b = high;
			while (a <= b) {
				while (file[a] < pivot) {
					a++;
				}
				while (file[b] > pivot) {
					b--;
				}
				/*if (file[a] == file[b] && a<high) {
					a++;
					continue;
				}*/

				if (a <= b) {
					T temp = file[a];
					file[a] = file[b];
					file[b] = temp;
					a++;
					b--;
				}

			}

			qSort(low, b);
			qSort(a, high);
		}// end of if 
	}//end of qSort

public:

	Resize() {
		file = new T[size];
		if (file == NULL) {
			throw ResizeMemoryError();
		}
	}


	//copy constructor
	Resize(const Resize& that)
	{
		file = new T[size];
		if (file == NULL) {
			throw ResizeMemoryError();
		}
		//strcpy(file, that.file);
		current = that.current;
		size = that.size;
		for (int i = 0; i < current; i++) {
			file[i] = that.file[i];
		}
	}


	//copy assignment operator
	void operator=(const Resize& that) {
		if (this != &that)
		{

			size = that.size;
			current = that.current;

			if (file != NULL) {
				delete[] file;
			}
			file = new T[size];
			if (file == NULL) {
				throw ResizeMemoryError();
			}
			for (int i = 0; i < current; i++) {
				file[i] = that.file[i];
			}

		}

	}


	//destructor
	~Resize()
	{
		delete[] file;
		file = NULL;
		
	}


	T& operator[](int i) {
		return file[i];
	}


	void merge(Resize<T> &thing) {
		for (int i = 0; i < thing.current; i++) {
			add(thing.file[i]);
		}

	}

	void add(const T &thing) {
		//assert(thing != NULL);
		
		for (int i = 0; i < current; i++) {
			if (file[i] == thing) {
				file[i] = thing;
				return;
			}
		}
		//check that we're not copying over duplicates. Searching here?
		if (current < size) {
			file[current] = thing;
			++current;
			
		}
		else
		{

			resize();
			file[current] = thing;
			++current;

		}
		//cout << current << endl;
	}


	void resize() {


		T* temp = new T[size];
		if (temp == NULL) {
			throw ResizeMemoryError();
		}

		for (int i = 0; i < size; i++) {
			temp[i] = file[i];
		}

		delete[]file;
		file = new T[size * 2];
		if (file == NULL) {
			throw ResizeMemoryError();
		}
		for (int i = 0; i < size; i++) {
			file[i] = temp[i];
		}
	
		size = size * 2;
		delete[] temp;
		temp = NULL;


	}

	void remove(int place) {
		// removes thing at point in array
		if (current - 1 <= size / 2)
		{
			if (size < 2) {
				size = 2;
			}
			T* newfile = new T[size / 2];
			if (newfile == NULL) {
				throw ResizeMemoryError();
			}

			size = size / 2;
			for (int i = 0; i < place; i++) {
				newfile[i] = file[i];
			}
			for (int j = current-1; j > place; j--) {
				newfile[j - 1] = file[j];
			}
			
			delete[] file;
			file = newfile;		
		}
	

		else {
			file[place] = ShipData();
			for (int j = current-1; j > place; j--) {
				file[j - 1] = file[j];
			}
		}
	
		file[current-1] = ShipData();
		current--;

		
	}



	void replace(T thing, int place) {
		//replaces object at certain point w/ new object
		file[place] = thing;
	}

	T * getFile() {
		return file;
	}

	T getLast() {

		return file[current-1];
	}

	int getSize() {
		return current;
	}


	T& getAt(int place) {

		return file[place];
	}

	void addAt(int place, T thing) {

		if(place > current){
			throw ResizeException();
		}
		T temporary = file[place];
		
		current++;
		int number = place;
		for (; number < current; number++) {
			file[number+1] = file[number];
		}
		file[place] = thing;
		if (current >= size) {

			resize();
				
		}
	


	}
	//method that calls the quicksort method
	void sort() {
		qSort(2, current-1);
	}

	
	int binarySearch(const T& thing) {

		int temp = current / 2;
		int low = 0;
		int high = current;
		while (low<high) {
			temp = (low + high) / 2;

			if (file[temp] < thing) {
				low = temp;
				
			}
			else if (file[temp] > thing) {
				high = temp;
			}
			else {
			//found it
				return temp;

			}


		}
		if (file[low] == thing) {
			return low;
		}
		else {
			//we did not find it, return -1;
			return -1;
		}

	}


};
//end of resize








//checking each row for the correct numbers
bool mathCheck(string columns[]) {

	int numbers[24] = {};
	string temp;

	for (int i = 0; i < 24; i++) {


		if (columns[i + 1] == "") {
			numbers[i] = 0;
		}
		else {
			numbers[i] = stoi(columns[i + 1]);
		}
	}

	int total = 0;
	for (int i = 3; i < 24; i += 3) {
		total += numbers[i];
	}

	if (numbers[0] != total) {
		cout << "First column total is false. Header line 272. " << endl;
		return false;
		
	}

	total = 0;
	for (int i = 4; i < 24; i += 3) {
		total += numbers[i];
	}

	if (numbers[1] != total) {
		cout << "Second column total is false. Header line 283. " << endl;
		return false;
	}

	total = 0;
	for (int i = 5; i < 24; i += 3) {
		total += numbers[i];
	}

	if (numbers[2] != total) {
		cout << "Third column total is false. Header line 293. " << endl;
		return false;
	}

	return true;
};




