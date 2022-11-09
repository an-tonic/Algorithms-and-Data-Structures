#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>


using namespace std;

typedef struct box {
	unsigned char width;
	unsigned char length;
	unsigned char name;

};

typedef struct PackerProblem {
	unsigned short numberOfBoxes;
	unsigned char width;
	unsigned char length;
	
	box boxes[30];

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
	cout << newLine << endl;
	
	//TODO Check input for strings
	if (newLine.length() < 5) {
		cout << "The file is currupted." << endl;
		cout << "The first line of the file is incorrect." << endl;
		exit(3);
	}

	//Reading and initiating main class with number of boxes and size of the cont
	getline(file, newLine, ' ');
	container.width = stoi(newLine);
	

	getline(file, newLine, '\n');
	container.length = stoi(newLine);
	
	getline(file, newLine);
	container.numberOfBoxes = stoi(newLine);
	
	areaContainer = container.width * container.length;

	
	//Reading and initiating box sizes and their names
	while (file.good()) {
		getline(file, newLine, ' ');
		container.boxes[counter].length = stoi(newLine);

		getline(file, newLine, ' ');
		container.boxes[counter].width = stoi(newLine);

		getline(file, newLine, '\n');
		container.boxes[counter].name = newLine[0];

		//cout << (container.boxes[counter].length - '0')*(container.boxes[counter].width - '0') << endl;

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


	//TODO print container and boxes
	return &container;
}



int main() {
	
	
	PackerProblem* pC = loadPackerProblem("input.txt");
	

	cout << sizeof(pC->numberOfBoxes);

	
}