#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "QuickSort.h"
#include "Stack.h"
#include <ctime>
#include <chrono>
#include <vector>

using namespace std;
using namespace std::chrono;
//Util functions

/// <summary>
/// String input validation. Checks that each char is an integer.
/// </summary>
/// <param name="s">Takes a pointer to a string.</param>


//Abstract data types
/// <summary>
/// A box to be placed in the container. Has three dimentions: width, length, and name.
/// </summary>

typedef struct Box {
	unsigned char width;
	unsigned char length;
	unsigned char name;

	// Operator Overloadings on area of the boxes for the Quicksort Algorithm
	bool operator <=(const Box& bx) {
		return width * length <= bx.width * bx.length;
	}

};

template <typename T>
struct Coordinates {
	T x;
	T y;
	Box* boxPlaced;
};

/// <summary>
/// The container that holds the number of boxes to be placed, its width, length, a pointer to boxes.
/// Two functions to dynamically allocate boxes and to print all of the configurations.
/// </summary>
typedef struct PackerProblem {
	unsigned int number_boxes = 0;
	unsigned short width = 0;
	unsigned short length = 0;
	unsigned char** container;

	Box* boxes;

	/// <summary>
	/// Constructor for dynamic boxes.
	/// </summary>
	/// <param name="num_boxes">Number of boxes that needs to be allocated</param>
	void allocate_boxes(int num_boxes) {
		boxes = new Box[num_boxes];
	}

	/// <summary>
	/// Prints the container and some additional information.
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


typedef struct PackerSolver {

	unsigned long long steps = 0;

	unsigned long long numberOfSolutions = 0;

	PackerProblem* pProblem;
	string* finalSolution;

};

/// <summary>
/// Calculates the area of the box. 
/// </summary>
/// <param name="box">Pointer to the Box structure.</param>
/// <returns>The area of the box</returns>
int boxArea(Box* box) {
	return (box->width * box->length);
}

/// <summary>
/// Accepts pointer to string. Check if the string is empty. Searches for chars in the strings.
/// </summary>
void checkInput(string* str) {

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

/// <summary>
/// Checks if any of the boxes could potentially fit into the container.
/// </summary>
/// <param name="problem">The structure holding container and boxes.</param>
/// <param name="counter">The box counter: to dislay which box is the problem.</param>
void checkBoxBound(PackerProblem* problem, int counter) {

	if (problem->boxes[counter].width > problem->width || problem->boxes[counter].width > problem->length) {
		cout << "The box on line " << counter + 4 << " would not fit into the container.\n Terminating...\n";
		exit(4);
	}

}

/// <summary>
/// Writes the name of the box onto the container array.
/// </summary>
/// <param name="box">The box to be placed.</param>
/// <param name="x">x coordinate of the box</param>
/// <param name="y">y coordinate of the box</param>
void placeBox(PackerProblem* problem, Box* box, int x, int y) {

	int xBounds = x + box->width;
	int down = box->length;
	char name = box->name;

	//Placing the box in the container
	for (int i = x; i < xBounds; i++) {
		memset(&problem->container[i][y], name, down);
	}

}

/// <summary>
/// Chaa]nges the length with the width of the box, thus rotates it.
/// </summary>
/// <param name="box">Box to be rotated</param>
void rotateBox(Box* box) {
	int tmp = box->length;
	box->length = box->width;
	box->width = tmp;
}

/// <summary>
/// Writes empty char "0" into the container, writes the names of all the boxes from the stack and prints the container
/// </summary>
template <typename T>
void showContainer(PackerProblem* problem, Stack<Coordinates<T>>* pStack) {
	//Cleaning the container
	for (int i = 0; i < problem->width; i++) {
		for (int j = 0; j < problem->length; j++) {
			problem->container[i][j] = '0';
		}
	}
	//Writing the box into the container
	for (size_t i = 0; i < pStack->_top; i++) {
		placeBox(problem, pStack->_data[i].boxPlaced, pStack->_data[i].x, pStack->_data[i].y);
	}
	
	problem->printContainer();
}

/// <summary>
/// Reads the file, checks each line, writes errors if any, initializes the container array with "0" and allocates boxes.
/// </summary>
/// <param name="filename">The name of the file to be read.</param>
/// <returns>The structure that holds the boxes and the container.</returns>
PackerProblem* loadPackerProblem(string filename) {
	PackerProblem* problem = new PackerProblem();
	ifstream file(filename);
	string newLine;
	unsigned int counter = 0;
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
	checkInput(&newLine);
	problem->width = stoi(newLine);
		
	getline(file, newLine, '\n');
	checkInput(&newLine);
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
	checkInput(&newLine);
	problem->number_boxes = stoi(newLine);
	problem->allocate_boxes(problem->number_boxes);

	areaContainer = problem->width * problem->length;
	
	//Reading and initiating box sizes and their names
	while (file.good()) {
		getline(file, newLine, ' ');
		checkInput(&newLine);
		problem->boxes[counter].width = stoi(newLine);
		checkBoxBound(problem, counter);

		getline(file, newLine, ' ');
		checkInput(&newLine);
		problem->boxes[counter].length = stoi(newLine);
		checkBoxBound(problem, counter);

		//No input checks for the name. We expect any char.
		getline(file, newLine, '\n');
		problem->boxes[counter].name = newLine[0];

		sumAreaBoxes = sumAreaBoxes + boxArea(&problem->boxes[counter]);
		counter++;
	}

	if (sumAreaBoxes > areaContainer) {
		cout << "The problem is unsolvable!" << endl;
		cout << "The area of boxes exceed the area of the container!" << endl;
		cout << "Container area: " << areaContainer << " " << " Sum of the boxes' area: " << sumAreaBoxes << endl;
		exit(3);
	}

	if (counter != problem->number_boxes) {
		cout << "The file is currupted!" << endl;
		cout << "Incorrect number of boxes provided/specified." << endl;
		exit(3);
	}

	//TODO After solving, compare to unused space and calulate efficiency
	cout << "The optimal solution would use " << sumAreaBoxes << " square m of space, leaving empty " << areaContainer - sumAreaBoxes << " sq.m. of space." << endl << endl;
	

	return problem;
}


/// <summary>
/// Accepts a poiner to a box and checks if it would collide with any of the already placed boxes. If it collides jumps the coordinates to the edge
/// of the box that it collided with.
/// </summary>
/// <param name="problem">Stucture to hold boxs, container</param>
/// <param name="pStack">Pointer to the stack</param>
/// <param name="boxToCheck">Box to be checked</param>
/// <param name="px">Pointer to x coords now beeing tested</param>
/// <param name="py">Pointer to x coords now beeing tested</param>
/// <returns>True, if the box has collided or not.</returns>
template <typename T>
bool boxCollided(PackerProblem* problem, Stack<Coordinates<T>>* pStack, Box* boxToCheck, T* px, T* py) {
	int width = boxToCheck->width;
	int length = boxToCheck->length;
	int x = *px;
	int y = *py;

	for (size_t i = 0; i < pStack->_top; i++) {

		//Regular collision
		if (x > pStack->_data[i].x - width
			&& x < pStack->_data[i].x + pStack->_data[i].boxPlaced->width
			&& y > pStack->_data[i].y - length
			&& y < pStack->_data[i].y + pStack->_data[i].boxPlaced->length)
		{
			//The box has collided, jump the coords to the edge of the box
			*px = pStack->_data[i].x + pStack->_data[i].boxPlaced->width - 1;
			return true;
		}
	}
	
	return false;
}

/// <summary>
/// Turns the coordinates and names of the boxes into a string for storage and easier checking if solution already exists.
/// </summary>
/// <param name="pStack">Pointer to the stack</param>
/// <returns>Return pointer to the string.</returns>
template <typename T>
string* stringifyStack(Stack<Coordinates<T>>* pStack) {
	string* s = new string();

	for (int i = 0; i < pStack->_top; i++) {
		s->append(to_string(pStack->_data[i].x) + to_string(pStack->_data[i].y)+ (char)pStack->_data[i].boxPlaced->name);
	}
	return s;
}

/// <summary>
/// Iterates through the vector containing all the solutions, and checks if the string representing unique solution exists.
/// </summary>
/// <param name="solutions">Pointer to the vector of all solutions</param>
/// <param name="newSolution">Pointer to the string of the solutions.</param>
/// <returns>Return if the solution has been already found.</returns>
bool solutionExists(vector<string>* solutions, string* newSolution) {

	vector<string>::iterator it;

	for (it = solutions->begin(); it != solutions->end(); it++) {
		if (*newSolution == *it) {
			return true;
		};
	}
	return false;
}

template <typename T>
void tryNextPosition(Stack<Coordinates<T>>* pStack, T* x, T* y) {
	*x = pStack->top().x + 1;
	*y = pStack->top().y;
	pStack->pop();
}

/// <summary>
/// Uses backtracking to find solutions/solutions.
/// </summary>
/// <param name="problem">Structure to hold boxs, container</param>
/// <param name="solver">Structure to hold the problem structure, number of solutions, and number of steps to solve one solution.</param>
/// <param name="solveAll">Find just one or all solutions.</param>
/// <returns>Return if the solution has been already found.</returns>
template <typename T>
void solveProblem(PackerProblem* problem, PackerSolver* solver, bool solveAll = false) {
	Stack<Coordinates<T>> stack(problem->number_boxes);

	T maxDepth = 0;
	int boxIndex = problem->number_boxes - 1;
	bool boxPlaced = false;
	bool boxRotated = false;
	T x = 0;
	T y = 0;
	//To redice number of requests to length and width of the container
	T length = problem->length;
	T width = problem->width;
	Box* currentBox;

	//Find just one solution
	if (!solveAll) {
		while (boxIndex > -1) {
			solver->steps++;
			boxPlaced = false;
			currentBox = &problem->boxes[boxIndex];

			//Check the box placemt not(!) up to the edge of the container but to edge minus the width/length of the box.
			for (T i = y; !boxPlaced && i < length - currentBox->length + 1; i++) {
				for (T j = x; !boxPlaced && j < width - currentBox->width + 1; j++) {
					if (!boxCollided<T>(problem, &stack, currentBox, &j, &i)) {
						stack.push(Coordinates<T>({ j, i, currentBox }));
						boxPlaced = true;
						boxIndex--;
						if (boxRotated) {
							boxRotated = false;
						}
					}
				}
				//After the box has been poped, the previous starts where it was, 
				//but it needs to start from '0' on the next line of the container
				x = 0;
			}
			y = 0;


			if (!boxPlaced && stack.isEmpty()) {
				cout << "There is no solution!\n";
				exit(5);
			}


			if (!boxPlaced) {
				if (boxRotated) {
					//"unrotate" the box
					rotateBox(currentBox);
					boxRotated = false;

					tryNextPosition(&stack, &x, &y);
					boxIndex++;
				}
				//If the box can be rotated we rotated it
				else if (!boxRotated && currentBox->length != currentBox->width) {
					rotateBox(currentBox);
					boxRotated = true;
				}
				//If the sides are the smae, we do not need to rotate
				else if (currentBox->length == currentBox->width) {
					if (boxIndex == problem->number_boxes - 1) {
						x = 0;
						y = 0;
					}
					else {
						tryNextPosition(&stack, &x, &y);
						boxIndex++;
					}
				}
				//Remembering the "depth" we reach when placing the boxes. I.e. if we are constantly on the same depth, the algorithm is stuck.
				if (maxDepth < boxIndex) {
					maxDepth = boxIndex;
				}

			}
		}

		showContainer(problem, &stack);
		solver->finalSolution = stringifyStack(&stack);
		cout << solver->steps << " steps needed.\n";
		
	}
	//Find all the solutions
	else if (solveAll) {
		
		//Initailize vecort that will contain all solutions 
		vector<string> allSolutions;

		while (boxIndex > -1) {

			solver->steps++;
			boxPlaced = false;
			currentBox = &problem->boxes[boxIndex];

			for (T i = y; !boxPlaced && i < length - currentBox->length + 1; i++) {
				for (T j = x; !boxPlaced && j < width - currentBox->width + 1; j++) {
					if (!boxCollided<T>(problem, &stack, currentBox, &j, &i)) {
						stack.push(Coordinates<T>({ j, i, currentBox }));
						boxPlaced = true;
						boxIndex--;
						if (boxRotated) {
							boxRotated = false;
						}
					}
				}
				//After the box has been poped, the previous starts where it was, 
				//but it needs to start from '0' on the next line of the container
				x = 0;
			}
			y = 0;

			if (boxIndex == -1) {
				string* textStack = stringifyStack(&stack);

				if (solutionExists(&allSolutions, textStack)) {
					stack._top = problem->number_boxes - maxDepth - 1;
					if (stack.isEmpty()) {
						cout << "Found " << solver->numberOfSolutions << " solutions!\n";
						return;
					}
					//Poping the box we are stuck on.
					boxIndex = maxDepth + 1;
					boxPlaced = true;
					
					//Going deeper if the solution exists again
					maxDepth++;
				}
				else {
					solver->numberOfSolutions++;
					cout << "Found solution number " << solver->numberOfSolutions << ": \n";
					allSolutions.push_back(*textStack);
					boxIndex++;
					maxDepth = 0;
				}
				//Regardless if the solution exists or we found it, we need to backtrack to previous box and move it further
				tryNextPosition(&stack, &x, &y);

			}


			if (!boxPlaced) {
				if (boxRotated) {
					//"unrotate" the box
					rotateBox(currentBox);
					boxRotated = false;

					tryNextPosition(&stack, &x, &y);
					boxIndex++;
				}
				else if (!boxRotated && currentBox->length != currentBox->width) {
					rotateBox(currentBox);
					boxRotated = true;
				}
				else if (currentBox->length == currentBox->width) {
					if (boxIndex == problem->number_boxes - 1) {
						x = 0;
						y = 0;
					}
					else {
						tryNextPosition(&stack, &x, &y);
						boxIndex++;
					}
				}
				//Remembering the "depth" we reach when placing the boxes. I.e. if we are constantly on the same depth, the algorithm is stuck.
				if (maxDepth < boxIndex) {
					maxDepth = boxIndex;
				}
			}
		}
	}
}


int main() {

	

	//testing single case
	if (true) {
		PackerSolver* pSolver = new PackerSolver();

		pSolver->pProblem = loadPackerProblem("input.txt");

		high_resolution_clock::time_point start, finish;
		duration<double> duration;
		start = high_resolution_clock::now();

		QuickSort<Box>(pSolver->pProblem->boxes, 0, pSolver->pProblem->number_boxes);
		solveProblem<unsigned short>(pSolver->pProblem, pSolver, true);

		finish = high_resolution_clock::now();
		duration = finish - start;

		

		cout << duration.count() << "\n";
	
	} 
	//testing avgtime
	else {

		PackerSolver* pSolver = new PackerSolver();

		pSolver->pProblem = loadPackerProblem("1input.txt");

		QuickSort<Box>(pSolver->pProblem->boxes, 0, pSolver->pProblem->number_boxes);


		high_resolution_clock::time_point start, finish;
		duration<double> duration;


		for (int j = 0; j < 15; j++){
			start = high_resolution_clock::now();
			for (int i = 0; i < 2000000; i++) {
				solveProblem<unsigned short>(pSolver->pProblem, pSolver, !true);
			}
			finish = high_resolution_clock::now();
			duration = finish - start;
			cout << duration.count() << "\n";

		}
	}
}