#include "Driver.h"
#include "Header.h"
#include "LinkedList.h"
#include "Tree.h"

int sum = 0;

using namespace std;



int first = 10;
int current = 0;
string finalline;
Totals* file = NULL;
Totals* database;
Tree<ShipData> *tree = new Tree<ShipData>();
string filein;
string userInput;
int sortedCol = -1;
ofstream out;


void main() {

	//string that holds the line being parsed.

	

	cout << "What file?" << endl;

	getline(cin, filein);
	//validating the file
	while (file == NULL) {
		//reading in a file
		file = parsefile(filein);
		if (file == NULL) {
			cout << "Invalid file. What file?" << endl;
			getline(cin, filein);
		}
	}//end of while loop

		//making linked list into array
	Resize <ShipData>* sortingTemp = file->returnArray();
	delete file;
	file = NULL;
	//sorting array
	for (int i = 0; i < sortingTemp->getSize(); i++) {
		sortingTemp[0][i].setColumn(0);
	}
	sortingTemp->sort();
	//returning array (sorted by country) to linkedlist
	database = returnList(sortingTemp);
	Totals* sortedList = NULL;

	//TREE STUFF
	tree = returnTree(sortingTemp);

	


	//call Hash Stuff
	Hash<ShipData>* hashTable = new Hash<ShipData>(database->getSize());
	Node<ShipData>* hashTester = database->getHead()->next->next;
	while (hashTester != NULL) {
		hashTable->put(hashTester->data);
		hashTester = hashTester->next;
	}



	//DS_16_WorldRegistries.csv











	while (userInput != "Q" && userInput != "q") {
		cout << "Do you want to 'M'erge, 'P'urge, 'A'lphabetize, 'O'utput, 'S'ort, 'H'ash Display, 'L'inked Display, 'F'ind, or 'Q'uit?" << endl;
		getline(cin, userInput);

		//Alphabetize
		if (userInput == "A" || userInput == "a") {

			

			cout << "What file would you like to write to? " << endl;

			getline(cin, userInput);

			//if user just hits enter
			if (userInput == "") {
				database->print();

				cout << finalline << endl;
			}
			//check that file does not already exist. If it does, do stuff
			if (ifstream(userInput)) {
				cout << "File already exists. Do you want to overwrite? Enter 'Y'es or 'N'o. " << endl;

				getline(cin, userInput);
				if (userInput == "Y" || userInput == "y") {
					//overwrite file
					cout << "Please input the file name again." << endl;
					getline(cin, userInput);

					ofstream out(userInput);
					Node<ShipData>* temp = database->getHead();
					while (temp->next != NULL) {
						out << temp->data << endl;
						temp = temp->next;

					}
					out << database->calculateTotals() << endl;
					//continue?

				}//end of overwriting file
				if (userInput == "N" || userInput == "n") {
					continue;
				}

			}

			else {

				ofstream out(userInput);
				Node<ShipData>* temp = database->getHead();
				while (temp->next != NULL) {
					out << temp->data << endl;
					temp = temp->next;

				}
				out << database->calculateTotals() << endl;

			}




		}
		//Output
		if (userInput == "O" || userInput == "o") {

			cout << "What file would you like to write to? Please remember to end filename with .txt " << endl;

			getline(cin, userInput);

			//if user just hits enter
			if (userInput == "") {
				//sortedList->print();
				tree->printInOrder();

				cout << finalline << endl;
			}
			//check that file does not already exist. If it does, do stuff
			if (ifstream(userInput)) {
				cout << "File already exists. Do you want to overwrite? Enter 'Y'es or 'N'o. " << endl;

				getline(cin, userInput);
				if (userInput == "Y" || userInput == "y") {
					//overwrite file
					cout << "Please input the file name again." << endl;
					getline(cin, userInput);

					out= ofstream(userInput);
					out << database->getHead()->data << endl;
					out << database->getHead()->next->data << endl;
					printInOrder();


					//Node<ShipData>* temp = sortedList->getHead();
					////while (temp->next != NULL) {
					//	out << temp->data << endl;
					//	temp = temp->next;

					////}
					
					out << database->calculateTotals() << endl;
					//continue?

				}//end of overwriting file
				if (userInput == "N" || userInput == "n") {
					continue;
				}

			}

			else {

				ofstream out(userInput);
				Node<ShipData>* temp = database->getHead();
				while (temp->next != NULL) {
					out << temp->data << endl;
					temp = temp->next;

				}
				out << database->calculateTotals() << endl;

			}


		}
		//Merge
		if (userInput == "m" || userInput == "M") {
			cout << "What file would you like to merge? " << endl;
			getline(cin, filein);
			while (file == NULL) {
				//reading in a file
				file = parsefile(filein);
				if (file == NULL) {
					cout << "Invalid file. What file?" << endl;
					getline(cin, filein);
				}
			}//end of while loop

			Node<ShipData>* merger = file->getHead()->next->next;
			 
			while (merger != NULL) {
				hashTable->merge(merger->data);
				tree->add(merger->data);
				merger = merger->next;

			}
	
			
			database->merge(*file);
			Node<ShipData>* current = file->getHead();
			sortedCol = -1;


			//adding/resizing the hash table

			while (current != NULL) {
				//	hashTable->put(current->data);
				current = current->next;
			}




			cout << "Merged: " << filein << endl;
			delete file;
			file = NULL;
			cout << database->calculateTotals() << endl;

			//testing purposes
			//tree->printInOrder();
			//cout << endl << endl << endl;

		}
		//Purge
		if (userInput == "p" || userInput == "P") {
			cout << "What file would you like to purge? " << endl;
			getline(cin, filein);
			while (file == NULL) {
				//reading in a file
				file = parsefile(filein);
				if (file == NULL) {
					cout << "Invalid file. What file?" << endl;
					getline(cin, filein);
				}
			}//end of while loop

			Node<ShipData>* purger = file->getHead();
			while (purger != NULL) {
				hashTable->remove(purger->data);
				tree->remove(purger->data);
				purger = purger->next;
			}

			database->purge(*file);
			Node<ShipData>* removedFile = file->getHead()->next->next;
			while (removedFile != NULL) {
				cout << removedFile->data.getData()[0] << " was not found" << endl;
				removedFile = removedFile->next;
			}
			sortedCol = -1;
			cout << "Purged: " << filein << endl;
			delete file;
			file = NULL;
			cout << database->calculateTotals() << endl;

			//testing purposes
			//tree->printInOrder();

		}


		//Sort
		if (userInput == "S" || userInput == "s") {
			while (userInput != "") {
				cout << "What column would you like to sort by? 2-25" << endl;
				getline(cin, userInput);
				int num = 0;
				if (userInput.find_first_of("1234567890") != string::npos) {
					num = stoi(userInput);

					if (num > 1 && num < 26) {
						sortedCol = num;
						sortingTemp = database->returnArray();

						//sorting array
						for (int i = 0; i < sortingTemp->getSize(); i++) {
							sortingTemp[0][i].setColumn(num - 1);
						}
						sortingTemp->sort();
						//returning array (sorted by country) to linkedlist
						//DO I WANT TO DO THIS?
						sortedList = returnList(sortingTemp);
						break;



					}
					else {
						cout << "Invalid column. What column would you like to sort by? 2-25" << endl;
						getline(cin, userInput);
					}
				}
			}





		}

		//Search
		if (userInput == "F" || userInput == "f") {


			cout << "What column would you like to search from? 1-25, or hit 'enter' to exit." << endl;
			getline(cin, userInput);
			int num = 0;
			bool found = false;
			if (userInput.find_first_of("1234567890") != string::npos) {
				num = stoi(userInput);
				if (num >= 1 && num < 26) {

					if (num == 1) {
						cout << "What country are you looking for?" << endl;
						const string commas = ",,,,,,,,,,,,,,,,,,,,,,,,";
						getline(cin, userInput);
						//hashmap



						ShipData s = hashTable->get(ShipData(userInput + commas));
						if (s.isNull()) {
							//output not found
							cout << "Country not found " << endl;
						}
						else {
							cout << s << endl;
						}

					}
					else {
						int i = num - 1;
						cout << "Watchu looking for?" << endl;

						getline(cin, userInput);

						if (sortedCol == num) {
							//binary search
							string searchline = "";
							for (int j = 0; j < 25; j++) {
								if (j == i) {
									searchline += userInput;
								}
								else {
									searchline += ",";
								}

							}
							//need to handle duplicates
							ShipData sd(searchline);
							int value=sortingTemp->binarySearch(sd);
							if (value < 0) {
								found = false;
							}
							else {
								//it was found
								while (value-1 >-1 && sortingTemp[0][value-1] == sd) {
									value--;
								}
								while (value < sortingTemp->getSize() && sortingTemp[0][value] == sd) {
									cout << sortingTemp[0][value++] << endl;
								}
							}
							searchline = "";

						}
						else {

						
							//linear search


							Node<ShipData>* searchingTemp = database->getHead()->next->next;
							while (searchingTemp != NULL) {
								if (searchingTemp->data.getData()[i] == userInput) {
									cout << searchingTemp->data << endl;
									found = true;
								}
								searchingTemp = searchingTemp->next;

							}
						}

						//finding country name

						if (!found) {
							cout << "Not found in column" << endl;
						}
					}


				}



			}

		}


		if (userInput == "H" || userInput == "h") {

			hashTable->printBuckets();
		}

		if (userInput == "L" || userInput == "l") {

			hashTable->globalOutput();

		}

		//Quit
		if (userInput == "Q" || userInput == "q") {

			cout << "Bye" << endl;

			return;

		}



		//DS_16_WorldRegistries.csv



		//	return;

	}


}






Totals* returnList(Resize<ShipData>* file) {
	Totals* temp = new Totals();
	int i = 0;
	while (i < file->getSize()) {
		temp->addNode(file[0][i]);
		i++;
	}
	return temp;
}

Tree<ShipData>* returnTree(Resize<ShipData>* file) {
	Tree<ShipData> *temp = new Tree<ShipData>();
	int i = 2;
	while (i < file->getSize()) {
		temp->add(file[0][i]);
		i++;
	}
	return temp;
}
Totals* parsefile(string filein) {



	ifstream in(filein);

	string eachline = "";

	cout << "The file you entered is: " << filein << endl;



	if (in.is_open()) {
		Totals * file = new Totals();

		while (getline(in, eachline)) {

			file->addNode(ShipData(eachline));
			++sum;

		}


		cout << (mathCheck2(file) ? "Math check 2 was correct" : "Math check 2 was incorrect") << endl;
		file->removeLast();


		cout << "The file was successfully read" << endl;
		return file;
	}//end of reading in a file





	else {

		cout << "File not found. Please input a file name.";
		return NULL;


	}//if its not a valid file


}





bool mathCheck2(LinkedList<ShipData>* file) {


	int totals[24] = {};
	string currword = "";
	string * currCols;
	bool result = true;
	Node<ShipData>* tempCols = file->getHead()->next->next;

	//making sure to start on the second row with all the data, and not to hit the totals line
	while (tempCols->next != NULL) {


		currCols = tempCols->data.getData();
		for (int j = 0; j < 24; j++) {
			int num = 0;

			if (currCols[j + 1].find_first_of("0123456789") != string::npos) {
				num = stoi(currCols[j + 1]);
				totals[j] += num;
				//	cout << totals[j] << " ";
			}
			else
			{
				totals[j] += 0;

			}
		}
		//cout << endl;
		tempCols = tempCols->next;
	}


	//check that everything works out
	//size - 1?

	currCols = tempCols->data.getData();

	for (int i = 0; i < 24; i++) {

		if (stoi(currCols[i + 1]) != totals[i]) {
			//CHECK THAT THESE GET THE RIGHT TOTALS
			cout << "The totals for column " << i << " is incorrect. Driver 362 " << endl;
			result = false;
		}
	}


	return result;
}

void printInOrder() {

	_printIO(tree->getRoot());
}

void _printIO(LeafNode<ShipData> *node) {

	if (!node) return;

	_printIO(node->left);
     out << node->data << endl;
	_printIO(node->right);


}


