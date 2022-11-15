#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>


using namespace std;

struct box {
	unsigned char width;
	unsigned char length;
	unsigned char name;
};

typedef struct PackerProblem {
	unsigned short numberOfBoxes;
	unsigned char width;
	unsigned char length;
	
	box boxes[25];

};


PackerProblem* loadPackerProblem(string filename) {
	PackerProblem container;
	ifstream file(filename);
	string newLine;
	unsigned short counter = 0;
	unsigned short areaContainer = 0;
	unsigned short sumAreaBoxes = 0;

	//Reading and proof-checking the first line of the file
	getline(file, newLine);
	cout << "Loading file: " << newLine << endl;

	
	if (newLine.length() < 5 || (int)newLine[0] < 65) {
		cout << "The file is currupted." << endl;
		cout << "The first line of the file is incorrect." << endl;
		exit(3);
	}

	//Reading and initiating main class with number of boxes and size of the cont
	
	
	try {
		getline(file, newLine, ' ');
		container.width = stoi(newLine);
		
		getline(file, newLine, '\n');
		container.length = stoi(newLine);

		getline(file, newLine);
		container.numberOfBoxes = stoi(newLine);

		areaContainer = container.width * container.length;
	}
	catch (const std::invalid_argument& ex) {
		cout << "The numerical size of a contaner/box is currupted." << endl;
		exit(3);
	}
	
	//Reading and initiating box sizes and their names
	while (file.good()) {
		getline(file, newLine, ' ');
		container.boxes[counter].length = stoi(newLine);

		getline(file, newLine, ' ');
		container.boxes[counter].width = stoi(newLine);

		getline(file, newLine, '\n');
		container.boxes[counter].name = newLine[0];

		sumAreaBoxes = sumAreaBoxes + (int)container.boxes[counter].length * (int)container.boxes[counter].width;
		counter++;
	}

	if (sumAreaBoxes > areaContainer) {
		cout << "The problem is unsolvable!" << endl;
		cout << "The area of boxes exceed the area of the container." << endl;
		cout << "Container area: " << areaContainer << " " << " Sum of the boxes' area: " << sumAreaBoxes << endl;
		exit(3);
	}

	if (counter != container.numberOfBoxes) {
		cout << "The file is currupted!" << endl;
		cout << "Incorrect number of boxes provided/specified." << endl;
		exit(3);
	}

	//TODO After solving, compare to unused space and calulate efficiency
	cout << "The optimal solution would use " << sumAreaBoxes << " square sm of space, leaving empty " << areaContainer - sumAreaBoxes << " sq.sm. of space." << endl << endl;
	//print container and boxes
	cout << "Container Configuration (" << (int)container.length << "x" << (int)container.width << ")" << endl;
	
	//top
	cout << '+' << string(container.width, '-') << '+' << endl;
	
	//body
	for (int i = 0; i < container.length; i++) {
		cout << '|' << string(container.width, 'X') << '|' << endl;
	}
	//bottom
	cout << '+' << string(container.width, '-') << '+' << endl;

	cout << container.numberOfBoxes << " Boxes to be placed" << endl;
	//boxes
	for (int i = 0; i < container.numberOfBoxes; i++) {
		cout << container.boxes[i].name << " (" << (int)container.boxes[i].width << "x" << (int)container.boxes[i].length << ")" << endl;
		for (int j = 0; j < container.boxes[i].length; j++) {
			cout << string(container.boxes[i].width, container.boxes[i].name) << endl;
		}
		cout << endl;
	}

	return &container;
}

//TODO Implement compare objects fuction by volume
int compare(const void* a, const void* b) {
	box *x = (box*)a;
	box *y = (box*)b;

	if ((int)x->length * (int)x->width > (int)y->length * (int)y->width) {
		return 1;
	}
		
	else if ((int)x->length * (int)x->width < (int)y->length * (int)y->width) {
		return -1;
	}


	return 0;
}

int main() {
	
	box a = { 1,2,3 };
	
	PackerProblem* pC = loadPackerProblem("input.txt");

	qsort(pC->boxes, pC->numberOfBoxes, sizeof(pC->boxes[0]), compare);

	cout << sizeof(pC->boxes[0]);

	
}
//TODO Ask Questions: 1. typedef? 2.returning address of a local variable(loadpackerPrblem). 3. Quicksort. 4.Iterate each input with for-loop matching ASCII? RegEx? 5. doesn't hueristics oppose the "find all solutions" additional requirement 6. Function to print the box configurations and the container in the PackerProblem DS