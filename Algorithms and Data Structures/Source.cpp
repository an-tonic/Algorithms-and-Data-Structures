#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "QuickSort.h"


using namespace std;

//Util functions

/// <summary>
/// String input validation. Checks that each char is an integer.
/// </summary>
/// <param name="s">Takes a pointer to a string.</param>
void check_int(string* str) {
	for (auto ch : *str) {
		if (isdigit(ch) == 0) {
			cout << "Illegal charchter \"" << ch << "\" detected in stream. Only integers allowed.";
			exit(3);
		}
	}
}


//Absract data types
/// <summary>
/// A box to be placed in the container. Has three dimentions: width, length, and name.
/// </summary>
typedef struct box {
	unsigned char width;
	unsigned char length;
	unsigned char name;

	// Operator Overloadings on area of the boxes

	bool operator >(const box& bx) {
		return width*length > bx.width * bx.length;
	}

	bool operator <(const box& bx) {
		return width * length < bx.width * bx.length;
	}

	bool operator ==(const box& bx) {
		return width * length == bx.width * bx.length;
	}

	bool operator <=(const box& bx) {
		return width * length <= bx.width * bx.length;
	}

	bool operator >=(const box& bx) {
		return width * length >= bx.width * bx.length;
	}

	int boxArea() {
		return ((int)width * (int)length);
	}
};

/// <summary>
/// The container that holds the number of boxes to be placed, its width, length, a pointer to boxes.
/// Two functions to dynamically allocate boxes and to print all of the configurations.
/// </summary>
typedef struct PackerProblem {
	unsigned short number_boxes = 0;
	unsigned short int width = 0;
	unsigned short int length = 0;
	unsigned char** container;

	box* boxes;

	/// <summary>
	/// Constructor for dynamic boxes.
	/// </summary>
	/// <param name="num_boxes">Number of boxes that needs to be allocated</param>
	void allocate_boxes(short num_boxes) {
		boxes = new box[num_boxes];
	}

	/// <summary>
	/// Prints the container2 and some additional information.
	/// </summary>
	void printContainer() {
		//print container
		cout << "Container Configuration (" << length << "x" << width << ")" << endl;

		//top
		cout << '+' << string(width, '-') << '+' << endl;

		//body
		for (int i = 0; i < length; i++) {
			cout << '|';
			for (int j = 0; j < width; j++) {
				cout << (int)container[i][j];
			}

			cout << '|' << endl;
		}
		//bottom
		cout << '+' << string(width, '-') << '+' << endl;
	}

	/// <summary>
	/// Prints the boxes and some additional information.
	/// </summary>
	void printBoxes() {

		//print boxes
		cout << number_boxes << " Boxes to be placed" << endl;
		for (int i = 0; i < number_boxes; i++) {
			cout << boxes[i].name << " (" << (int)boxes[i].width << "x" << (int)boxes[i].length << ")" << endl;
			for (int j = 0; j < boxes[i].length; j++) {
				cout << string(boxes[i].width, boxes[i].name) << endl;
			}
			cout << endl;
		}

	};
};


PackerProblem* loadPackerProblem(string filename) {
	PackerProblem* problem = new PackerProblem();
	ifstream file(filename);
	string newLine;
	unsigned short counter = 0;
	unsigned int areaContainer = 0;
	unsigned int sumAreaBoxes = 0;

	//Reading and proof-checking the first line of the file
	getline(file, newLine);

	cout << "Loading file: " << newLine << endl;

	//The first line of the file should be of a specific length and start with a char
	if (newLine.length() < 5 || isdigit(newLine[0]) > 0) {
		cout << "The file is currupted." << endl;
		cout << "The first line of the file is incorrect." << endl;
		exit(3);
	}

	//Reading and initiating main object with number of boxes and size of the container
	
	getline(file, newLine, ' ');
	check_int(&newLine);
	problem->width = stoi(newLine);
		
	getline(file, newLine, '\n');
	check_int(&newLine);
	problem->length = stoi(newLine);
	
	//Initialyzing the container with empty space
	problem->container = new unsigned char* [problem->width];
	for (int i = 0; i < problem->length; i++) {
		problem->container[i] = new unsigned char[problem->length];
	}
	for (int i = 0; i < problem->length; i++) {
		for (int j = 0; j < problem->width; j++) {
			problem->container[i][j] = 0;
		}
	}
	

	getline(file, newLine);
	check_int(&newLine);
	problem->number_boxes = stoi(newLine);
	problem->allocate_boxes(problem->number_boxes);

	areaContainer = problem->width * problem->length;
	
	
	
	//Reading and initiating box sizes and their names
	while (file.good()) {
		getline(file, newLine, ' ');
		check_int(&newLine);
		problem->boxes[counter].length = stoi(newLine);

		getline(file, newLine, ' ');
		check_int(&newLine);
		problem->boxes[counter].width = stoi(newLine);

		//No input checks for the name. We expect any char.
		getline(file, newLine, '\n');
		problem->boxes[counter].name = newLine[0];

		sumAreaBoxes = sumAreaBoxes + problem->boxes[counter].boxArea();
		counter++;
	}

	if (sumAreaBoxes > areaContainer) {
		cout << "The problem is unsolvable!" << endl;
		cout << "The area of boxes exceed the area of the container." << endl;
		cout << "Container area: " << areaContainer << " " << " Sum of the boxes' area: " << sumAreaBoxes << endl;
		exit(3);
	}

	if (counter != problem->number_boxes) {
		cout << "The file is currupted!" << endl;
		cout << "Incorrect number of boxes provided/specified." << endl;
		exit(3);
	}

	//TODO After solving, compare to unused space and calulate efficiency
	cout << "The optimal solution would use " << sumAreaBoxes << " square sm of space, leaving empty " << areaContainer - sumAreaBoxes << " sq.sm. of space." << endl << endl;
	

	return problem;
}


int main() {

	PackerProblem* pC = loadPackerProblem("input.txt");
	
	pC->printContainer();
	/*pC->printBoxes();
	cout << "1";
	cout << "2";

	QuickSort<box>(pC->boxes, 0, pC->number_boxes);

	pC->printBoxes();*/
	



};
