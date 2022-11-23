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

	void print() {

		//print container and boxes
		cout << "Container Configuration (" << (int)length << "x" << (int)width << ")" << endl;

		//top
		cout << '+' << string(width, '-') << '+' << endl;

		//body
		for (int i = 0; i < length; i++) {
			cout << '|' << string(width, 'X') << '|' << endl;
		}
		//bottom
		cout << '+' << string(width, '-') << '+' << endl;

		cout << numberOfBoxes << " Boxes to be placed" << endl;
		//boxes
		for (int i = 0; i < numberOfBoxes; i++) {
			cout << boxes[i].name << " (" << (int)boxes[i].width << "x" << (int)boxes[i].length << ")" << endl;
			for (int j = 0; j < boxes[i].length; j++) {
				cout << string(boxes[i].width, boxes[i].name) << endl;
			}
			cout << endl;
		}

	};
};


void check_int(string* s) {
	for (auto ch : *s) {
		if (isdigit(ch) == 0) {
			cout << "Illegal charchter \"" << ch << "\" detected in stream.Only integers allowed.";
			exit(3);
		}
	}
}

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

	//The first line of the file should be of a specific length and start with a char
	if (newLine.length() < 5 || isdigit(newLine[0]) > 0) {
		cout << "The file is currupted." << endl;
		cout << "The first line of the file is incorrect." << endl;
		exit(3);
	}

	//Reading and initiating main object with number of boxes and size of the cont
	
	getline(file, newLine, ' ');
	check_int(&newLine);
	container.width = stoi(newLine);
		
	getline(file, newLine, '\n');
	check_int(&newLine);
	container.length = stoi(newLine);

	getline(file, newLine);
	check_int(&newLine);
	container.numberOfBoxes = stoi(newLine);

	areaContainer = container.width * container.length;
	
	
	
	//Reading and initiating box sizes and their names
	while (file.good()) {
		getline(file, newLine, ' ');
		check_int(&newLine);
		container.boxes[counter].length = stoi(newLine);

		getline(file, newLine, ' ');
		check_int(&newLine);
		container.boxes[counter].width = stoi(newLine);

		//No input checks for the name. We expect any char.
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
	
	container.print();

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



	PackerProblem* pC = loadPackerProblem("input.txt");

	qsort(pC->boxes, pC->numberOfBoxes, sizeof(pC->boxes[0]), compare);

	pC->print();


	cout << typeid(pC->boxes).name() << endl;

};
//TODO Ask Questions: 1.returning address of a local variable(loadpackerPrblem). 2. Quicksort. 3. doesn't hueristics oppose the "find all solutions" additional requirement 4. Function to print the box configurations and the container in the PackerProblem DS