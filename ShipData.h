#pragma once
using namespace std;
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
//#include <cassert>
#include "Header.h"

const int SPACE_SIZE_HEADER = 30;
const int SPACE_SIZE_REST = 20;


class ShipData {
private:

	string * spaced = new string{ 25 };
	string * data;
	int currCol=0;

public:

	ShipData() {
		data = NULL;
	}

	 ShipData(string line) {


		 data = parser(line);

	}


	 friend ostream& operator<<(ostream& o, const ShipData&);

	 //copy constructor
	 ShipData(const ShipData& that)
	 {
		 if (that.data != NULL) {
			 data = new string[25];

			 for (int i = 0; i < 25; i++) {
				 data[i] = that.data[i];
			 }
		 }
		 else {
			 data = that.data;
		 }
		 currCol = that.currCol;
	 }



	 int operator!=(const ShipData& thing) {
		 return data[currCol] != thing.data[currCol];
	 }


	 //copy assignment operator
	 void operator=(const ShipData& that) {
		 if ( that.data != NULL) {
			 if (this != &that)
			 {
				 if (data != NULL) {
					 delete[] data;
				 }
				 data = new string[25];

				 for (int i = 0; i < 25; i++) {
					 data[i] = that.data[i];
				 }


			 }

		 }
		 else
		 {
			 if (data != NULL) {
				 delete[] data;
				 data = NULL;
			}
		 }
		 currCol = that.currCol;
	 }


	 ~ShipData() {
		 delete[] data;
		 data = NULL;
	 }

	 int operator<(const ShipData& thing)
	 {
		 int sortBy = currCol;
		 if (sortBy == 0) {
			 return data[sortBy] < thing.data[sortBy];

		 }
		 else {
			 int a = 0, b = 0;
			 if (data[sortBy] != "") {
				  a = stoi(data[sortBy]);
				
			 }
			 if (thing.data[sortBy] != "") {
					  b = stoi(thing.data[sortBy]);
			 }
			 return a < b;
		 }

		// return data[sortBy] < thing.data[sortBy];

	 }

	 int operator==(const ShipData& thing)
	 {
		 
		 int sortBy = currCol;
		 if (sortBy == 0) {
			 return data[sortBy] == thing.data[sortBy];

		 }
		 else {
			 int a = 0, b = 0;
			 if (data[sortBy] != "") {
				 a = stoi(data[sortBy]);

			 }
			 if (thing.data[sortBy] != "") {
				 b = stoi(thing.data[sortBy]);
			 }
			 return a == b;
		 }

		 

	 }
	 int operator>(const ShipData& thing) {
		 int sortBy = currCol;
		 if (sortBy == 0) {
			 return data[sortBy] > thing.data[sortBy];

		 }
		 else {
			 int a = 0, b = 0;
			 if (data[sortBy] != "") {
				 a = stoi(data[sortBy]);

			 }
			 if (thing.data[sortBy] != "") {
				 b = stoi(thing.data[sortBy]);
			 }
			 return a > b;
		 }
	 }

 string * parser(const string &eachline) {

		 string * columns = new string[25];
		 string currword = "";

		 for (int j = 0; j < 25; j++) {

			 for (int i = 0; i < eachline.length(); i++) {

				 //if the character is not a comma
				 if (eachline.at(i) != ',') {
					 currword.push_back(eachline.at(i));

					 if (i == eachline.length() - 1) {
						 columns[j] = currword;
						 ++j;
					 }
				 }

				 //if the character is a comma
				 else {
					 columns[j] = currword;
					 currword = "";
					 ++j;
				 }
			 }

			 if (columns[1].find_first_of("0123456789") != string::npos) {
				// mathCheck(columns);
			 }
			 return columns;
			 //may be an error here.
		 }

	 }




	 string * getData() {
		 return data;
	 }

	 void setColumn(int thing) {
		 currCol = thing;
	 }

	 bool isNull() {
		 return data == NULL;
	 }

};


string spacing(string* line) {
	string * columns = line;



	string currword = "";
	string finalline = "";
	string spaces = "";

	//temp array should now be full

	int tempsize = 0;
	for (int j = 0; j < 25; j++) {
		if (j == 0) {
			currword = columns[j];
			if (currword != "") {
				tempsize = SPACE_SIZE_HEADER - currword.size();
				for (int j = 0; j < tempsize; j++) {
					spaces += " ";
				}
				finalline = currword + spaces;
				spaces = "";
				currword = "";
			}
		}
		else {

			currword = columns[j];
			tempsize = SPACE_SIZE_REST - currword.size();
			for (int j = 0; j < tempsize; j++) {
				spaces += " ";
			}
			finalline += spaces + currword;
			spaces = "";
			currword = "";
		}
	}//end of spacing


	return finalline;

}

 ostream& operator<<(ostream& o, const ShipData& shipdata) {


	 string * columns = shipdata.data;



	string currword = "";
	string finalline = "";
	string spaces = "";

	//temp array should now be full

	int tempsize = 0;
	for (int j = 0; j < 25; j++) {
		if (j == 0) {
			currword = columns[j];
			if (currword != "") {
				tempsize = SPACE_SIZE_HEADER - currword.size();
				for (int j = 0; j < tempsize; j++) {
					spaces += " ";
				}
				finalline = currword + spaces;
				spaces = "";
				currword = "";
			}
		}
		else {

			currword = columns[j];
			tempsize = SPACE_SIZE_REST - currword.size();
			for (int j = 0; j < tempsize; j++) {
				spaces += " ";
			}
			finalline += spaces + currword;
			spaces = "";
			currword = "";
		}
	}//end of spacing



	o << finalline;
	return o;
}
