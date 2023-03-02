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


typedef struct PackerSolver {

	unsigned long long steps = 0;

	unsigned long long numberOfSolutions = 0;

	PackerProblem* pProblem;

};

int boxArea(box* box) {
	return (box->width * box->length);
}

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

		sumAreaBoxes = sumAreaBoxes + boxArea(&problem->boxes[counter]);
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

void placeBox(PackerProblem* problem, box* box, int x, int y) {

	int xBounds = x + box->width;
	int down = box->length;
	char name = box->name;
	
	//Placing the box in the container
	for (int i = x; i < xBounds; i++) {
		memset(&problem->container[i][y], name, down);
	}

}

void rotateBox(box* box) {
	int tmp = box->length;
	box->length = box->width;
	box->width = tmp;
}

void cleanContainer(PackerProblem* problem) {
	for (int i = 0; i < problem->width; i++) {
		for (int j = 0; j < problem->length; j++) {
			problem->container[i][j] = '0';
		}
	}
}

bool boxCollided(PackerProblem* problem, Stack<Coordinates>* pStack, box* boxToCheck, int* px, int* py) {
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
			//if collided, check that maybe the rotated box would not collide
			if (width != length && x + length < problem->width && y + width < problem->length) {
				for (size_t j = 0; j < pStack->_top; j++) 
				{	
					//Rotated collision
					if (x > pStack->_data[j].x - length
						&& x < pStack->_data[j].x + pStack->_data[j].boxPlaced->width
						&& y > pStack->_data[j].y - width
						&& y < pStack->_data[j].y + pStack->_data[j].boxPlaced->length)
					{
						*px += pStack->_data[i].boxPlaced->width - 1;
						return true;
					}
				}

				rotateBox(boxToCheck);
				return false;
			}
			//The box has same sides and collided - return
			*px += pStack->_data[i].boxPlaced->width - 1;
			return true;
		}
	}
	
	return false;
}

//TODO Delete (Depreciated or under development)
bool __boxCollided(PackerProblem* problem, Stack<Coordinates>* pStack, box* boxToCheck, int* px, int* py) {
	int width = boxToCheck->width;
	int length = boxToCheck->length;
	int x = *px;
	int y = *py;
	//inside the container
	if (x + width <= problem->width && y + length <= problem->length) {
		//inside the container while rotated and sides are of different length
		if (x + length < problem->width && y + width < problem->length && width != length) {
			for (size_t i = 0; i < pStack->_top; i++) {
				if (x > pStack->_data[i].x - width
					&& x < pStack->_data[i].x + pStack->_data[i].boxPlaced->width
					&& y > pStack->_data[i].y - length
					&& y < pStack->_data[i].y + pStack->_data[i].boxPlaced->length) {
					//still collides while rotated
					if (x > pStack->_data[i].x - length
						&& x < pStack->_data[i].x + pStack->_data[i].boxPlaced->width
						&& y > pStack->_data[i].y - width
						&& y < pStack->_data[i].y + pStack->_data[i].boxPlaced->length) {
						*px += pStack->_data[i].boxPlaced->width - 1;
						return true;

					}
					//need to recheck that the rotated box does not collide with any of the boxes
					else {
						for (size_t j = 0; j < pStack->_top; j++) {
							if (x > pStack->_data[i].x - length
								&& x < pStack->_data[i].x + pStack->_data[i].boxPlaced->width
								&& y > pStack->_data[i].y - width
								&& y < pStack->_data[i].y + pStack->_data[i].boxPlaced->length)
							{
								*px += pStack->_data[i].boxPlaced->width - 1;
								return true;
							}
						}
						rotateBox(boxToCheck);

						return false;
					}
				}
			}
			return false;
		}
		//outside of the container while rotated
		else {
			//do regular collision detection
			for (size_t i = 0; i < pStack->_top; i++) {
				if (x > pStack->_data[i].x - width
					&& x < pStack->_data[i].x + pStack->_data[i].boxPlaced->width
					&& y > pStack->_data[i].y - length
					&& y < pStack->_data[i].y + pStack->_data[i].boxPlaced->length)
				{
					*px += pStack->_data[i].boxPlaced->width - 1;
					return true;
				}
			}
			//did not collide
			return false;
		}
	}
	//outside of the container
	else {
		//inside the container while rotated
		if (x + length <= problem->width && y + width <= problem->length && width != length) {
			//colledes with all the boxes while rotated
			for (size_t i = 0; i < pStack->_top; i++) {
				if (x > pStack->_data[i].x - length
					&& x < pStack->_data[i].x + pStack->_data[i].boxPlaced->width
					&& y > pStack->_data[i].y - width
					&& y < pStack->_data[i].y + pStack->_data[i].boxPlaced->length)
				{
					*px += pStack->_data[i].boxPlaced->width - 1;
					return true;
				}
			}
			//did not collide with all of the boxes while rotated
			rotateBox(boxToCheck);

			return false;
		}
		//outside of the container even if rotated
		else {
			return true;
		}
	}

	return true;
}
bool _boxCollided(PackerProblem* problem, Stack<Coordinates>* pStack, box* boxToCheck, int* px, int* py) {
	int width = boxToCheck->width;
	int length = boxToCheck->length;
	int x = *px;
	int y = *py;
	for (size_t i = 0; i < pStack->_top; i++) {
		if (x > pStack->_data[i].x - width
			&& x < pStack->_data[i].x + pStack->_data[i].boxPlaced->width
			&& y > pStack->_data[i].y - length
			&& y < pStack->_data[i].y + pStack->_data[i].boxPlaced->length)
		{	
			*px += pStack->_data[i].boxPlaced->width - 1;
			return true;
		}
	}
	return false;
}
void _solveProblem(PackerProblem* problem) {
	Stack<Coordinates> stack;

	int boxIndex = problem->number_boxes-1;
	bool boxPlaced = false;
	int x = 0;
	int y = 0;
	unsigned short length = problem->length;
	unsigned short width = problem->width;

	while (boxIndex > -1){
		box* tmp = &problem->boxes[boxIndex];
		for (int i = y; !boxPlaced && i < length; i++) {
			for (int j = x; !boxPlaced && j < width; j++) {
				if (!__boxCollided(problem, &stack, tmp, &j, &i)) {						
					stack.push(Coordinates({ j, i, tmp }));					
					boxPlaced = true;
					boxIndex--;						
				}				
			}
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
		
		placeBox(problem, stack._data[i].boxPlaced, stack._data[i].x, stack._data[i].y);

	}
}

void solveProblem(PackerProblem* problem, PackerSolver* solver, bool solveAll = false) {
	Stack<Coordinates> stack;

	int boxIndex = problem->number_boxes - 1;
	bool boxPlaced = false;
	bool boxRotated = false;
	int x = 0;
	int y = 0;
	unsigned short length = problem->length;
	unsigned short width = problem->width;
	box* currentBox;

	while (boxIndex > -1) {
		/*cleanContainer(problem);

		for (size_t i = 0; i < stack._top; i++) {
			placeBox(problem, stack._data[i].boxPlaced, stack._data[i].x, stack._data[i].y);
		}
		problem->printContainer();

		solver->steps++;*/
		boxPlaced = false;
		
		currentBox = &problem->boxes[boxIndex];

		for (int i = y; !boxPlaced && i < length - currentBox->length + 1; i++) {
			for (int j = x; !boxPlaced && j < width - currentBox->width + 1; j++) {
				if (!boxCollided(problem, &stack, currentBox, &j, &i)) {
					stack.push(Coordinates({ j, i, currentBox }));
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

		if (solveAll && !boxPlaced && boxIndex == problem->number_boxes - 1) {
			cout << "Found " << solver->numberOfSolutions << " the solutions!\n";
			break;
		}
		if (solveAll && boxIndex == -1) {
			solver->numberOfSolutions++;
			cleanContainer(problem);

			for (size_t i = 0; i < stack._top; i++) {
				placeBox(problem, stack._data[i].boxPlaced, stack._data[i].x, stack._data[i].y);
			}
			problem->printContainer();
			x = stack.top().x + 1;
			y = stack.top().y;
			stack.pop();
			boxIndex++;
		}

		
		if (!boxPlaced && boxRotated) {
		
			//"unrotate the box
			rotateBox(currentBox);
			boxRotated = false;
		
			x = stack.top().x + 1;
			y = stack.top().y;
			stack.pop();
			boxIndex++;	
			
		}

		if (!boxPlaced && !boxRotated) {
			x = stack.top().x + 1;
			y = stack.top().y;
			stack.pop();
			boxIndex++;
		}
		
		if (!boxPlaced && !boxRotated && currentBox->length != currentBox->width) {
			rotateBox(currentBox);
			boxRotated = true;
		}
	}

	for (size_t i = 0; i < stack._top; i++) {
		placeBox(problem, stack._data[i].boxPlaced, stack._data[i].x, stack._data[i].y);
	}
}

int main() {

	if (true) {
		PackerSolver* pSolver = new PackerSolver();
		
		pSolver->pProblem = loadPackerProblem("1input.txt");

		//QuickSort<box>(pSolver->pProblem->boxes, 0, pSolver->pProblem->number_boxes);

		high_resolution_clock::time_point start, finish;
		duration<double> duration;
		start = high_resolution_clock::now();

		solveProblem(pSolver->pProblem, pSolver, true);

		finish = high_resolution_clock::now();
		duration = finish - start;

		pSolver->pProblem->printContainer();

		cout << duration.count() << "\n";

		cout << pSolver->steps << " steps needed.\n";

		
	
	} else {

		/*PackerProblem* pC = loadPackerProblem("2input.txt");
		QuickSort<box>(pC->boxes, 0, pC->number_boxes);


		high_resolution_clock::time_point start, finish;
		duration<double> duration;


		for (int j = 0; j < 15; j++){
			start = high_resolution_clock::now();
			for (int i = 0; i < 2000000; i++) {
				_solveProblem(pC);
			}
			finish = high_resolution_clock::now();
			duration = finish - start;
			cout << duration.count() << "\n";

		}
		cout << "\nWith complex rotation\n";
		for (int j = 0; j < 15; j++) {
			start = high_resolution_clock::now();
			for (int i = 0; i < 2000000; i++) {
				solveProblem(pC);
			}
			finish = high_resolution_clock::now();
			duration = finish - start;
			cout << duration.count() << "\n";

		}*/

	}
}