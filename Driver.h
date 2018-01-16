#pragma once
using namespace std;
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "LinkedList.h"
//#include <cassert>
#include "ShipData.h"
#include "Hash.h"
#include "Tree.h"




Resize<ShipData>* reverse(Resize<ShipData>* file, int size, int larger, int smaller);
bool mathCheck2(LinkedList<ShipData>* file);
Totals* parsefile(string filein);
int * finalCalcCols(Resize<ShipData> * totalfile);
Totals* returnList(Resize<ShipData>* file);
Tree<ShipData>* returnTree(Resize<ShipData>* file);
void printInOrder();
void _printIO(LeafNode<ShipData> *node);
