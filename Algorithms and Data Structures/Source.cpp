#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "QuickSort.h"
#include "Stack.h"
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;
//Util functions

/// <summary>
/// String input validation. Checks that each char is an integer.
/// </summary>
/// <param name="s">Takes a pointer to a string.</param>
void check_int(string* str) {

	if (*str == "") {
		cout << "Illegal empty string detected in stream. Only integers allowed.";
		exit(3);
	}

	for (auto ch : *str) {
		if (isdigit(ch) == 0) {
			cout << "Illegal charchter \"" << ch << "\" detected in stream. Only integers allowed.";
			exit(3);
		}
	}
}


//Abstract data types
/// <summary>
/// A box to be placed in the container. Has three dimentions: width, length, and name.
/// </summary>
typedef struct box {


	unsigned char width;
	unsigned char length;
	unsigned char name;

	int boxArea() {
		return ((int)width * (int)length);
	}

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
};

typedef struct Coordinates {
	int x;
	int y;
	box* boxPlaced;
};

/// <summary>
/// The container that holds the number of boxes to be placed, its width, length, a pointer to boxes.
/// Two functions to dynamically allocate boxes and to print all of the configurations.
/// </summary>
typedef struct PackerProblem {
	unsigned short number_boxes = 0;
	unsigned short width = 0;
	unsigned short length = 0;
	unsigned char** container;

	box* boxes;

	/// <summary>
	/// Constructor for dynamic boxes.
	/// </summary>
	/// <param name="num_boxes">Number of boxes that needs to be allocated</param>
	void allocate_boxes(int num_boxes) {
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
				cout << container[j][i];
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
	for (int i = 0; i < problem->width; i++) {
		problem->container[i] = new unsigned char[problem->length];
	}
	for (int i = 0; i < problem->width; i++) {
		for (int j = 0; j < problem->length; j++) {
			problem->container[i][j] = '0';
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
		problem->boxes[counter].width = stoi(newLine);

		getline(file, newLine, ' ');
		check_int(&newLine);
		problem->boxes[counter].length = stoi(newLine);

		//No input checks for the name. We expect any char.
		getline(file, newLine, '\n');
		problem->boxes[counter].name = newLine[0];

		sumAreaBoxes = sumAreaBoxes + problem->boxes[counter].boxArea();
		counter++;
	}

	if (sumAreaBoxes > areaContainer) {
		cout << "The problem is unsolvable!" << endl;
		cout << "The area of boxes exceed the area of the container->" << endl;
		cout << "Container area: " << areaContainer << " " << " Sum of the boxes' area: " << sumAreaBoxes << endl;
		exit(3);
	}

	if (counter != problem->number_boxes) {
		cout << "The file is currupted!" << endl;
		cout << "Incorrect number of boxes provided/specified." << endl;
		exit(3);
	}

	//TODO After solving, compare to unused space and calulate efficiency
	//cout << "The optimal solution would use " << sumAreaBoxes << " square sm of space, leaving empty " << areaContainer - sumAreaBoxes << " sq.sm. of space." << endl << endl;
	

	return problem;
}

bool placeBox(PackerProblem* problem, box* box, int x, int y) {
	bool canBePlaced = true;
	int xBounds = x + box->width;
	int yBounds = y + box->length;
	int down = box->length;
	char name = box->name;
	//Check that box is in contqiner bounds
	/*if (xBounds > problem->width || yBounds > problem->length) {
		return !canBePlaced;
	}*/
	
	//Check that the space for the box is empty
	//for (int i = x; i < xBounds; i++) {
	//	for (int j = y; j < yBounds; j++) {
	//		//TODO change value to "0"
	//		if (problem->container[i][j] < 1) {
	//			return !canBePlaced;
	//		}
	//	}
	//}
	
	//Placing the box in the container
	for (int i = x; i < xBounds; i++) {
		memset(&problem->container[i][y], name, down);
	}


	return canBePlaced;
}

void removeBox(PackerProblem* problem, box* box, int x, int y) {
	int right = x + box->width;
	int down = box->length;


	for (size_t i = x; i < right; i++) {
		memset(&problem->container[i][y], '0', down);
	}
}

bool boxCollided(PackerProblem* problem, Stack<Coordinates>* pStack, box* boxToCheck, int x, int y) {
	int width = boxToCheck->width;
	int length = boxToCheck->length;

	if (x + width > problem->width || y + length > problem->length) {
		return true;
	}

	for (size_t i = 0; i < pStack->_top; i++) {		
		if (x > pStack->_data[i].x - width && x < pStack->_data[i].x + pStack->_data[i].boxPlaced->width) {
			if (y > pStack->_data[i].y - length && y < pStack->_data[i].y + pStack->_data[i].boxPlaced->length){
				return true;
			}			
		}	
	}
	return false;
}

void solveProblen(PackerProblem* problem) {
	Stack<Coordinates> stack;

	int boxIndex = problem->number_boxes-1;
	bool boxPlaced = true;
	int x = 0;
	int y = 0;
	while (boxIndex > -1){

		for (int i = y; i < problem->length && !boxPlaced; i++) {
			for (int j = x; j < problem->width && !boxPlaced; j++) {
				if (!boxCollided(problem, &stack, &problem->boxes[boxIndex], j, i)) {						
					stack.push(Coordinates({ j, i, &problem->boxes[boxIndex] }));					
					boxPlaced = true;
					boxIndex--;						
				}					
			}
			x = 0;
		}
		x = 0;
		y = 0;

		if (!boxPlaced) {
						
			x = stack.top().x + 1;
			y = stack.top().y;		
			stack.pop();
			boxIndex++;
			
		}
		boxPlaced = false;

	}

	for (size_t i = 0; i < stack._top; i++) {
		Coordinates tmp = stack._data[i];
		placeBox(problem, tmp.boxPlaced, tmp.x, tmp.y);

	}
}


int main() {

	PackerProblem* pC = loadPackerProblem("input.txt");
	
	QuickSort<box>(pC->boxes, 0, pC->number_boxes);

	pC->printContainer();

	high_resolution_clock::time_point start, finish;
	duration<double> duration;
	start = high_resolution_clock::now();

	solveProblen(pC);
	
	pC->printContainer();

	finish = high_resolution_clock::now();
	duration = finish - start;
	cout << duration.count() << "\n";

	/*Stack<Coordinates> stack;
	for (int i = 0; i < 25; i++) {
		stack.push(Coordinates{ 0, 0, &pC->boxes[24] });
	}

	high_resolution_clock::time_point start, finish;
	duration<double> duration;
	

	for (int j = 0; j < 15; j++)
	{
		
		start = high_resolution_clock::now();
		for (int i = 0; i < 20000000; i++) {
			boxCollided(pC, &stack, &pC->boxes[24], 8, 8);
		}
		finish = high_resolution_clock::now();
		duration = finish - start;
		cout << duration.count() << "\n";
		
	}*/
	
}
