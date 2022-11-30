#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>


using namespace std;


/// <summary>
/// A box to be placed in the container. Has three dimentions: width, length, and name.
/// </summary>
typedef struct box {
	unsigned char width;
	unsigned char length;
	unsigned char name;
};

/// <summary>
/// The container that holds the number of boxes to be placed, its width, length, a pointer to boxes.
/// Two functions to dynamically allocate boxes and to print all of the configurations.
/// </summary>
typedef struct PackerProblem {
	unsigned short number_boxes = 0;
	unsigned char width = 0;
	unsigned char length = 0;
	
	box* boxes;

	/// <summary>
	/// Constructor for dynamic boxes.
	/// </summary>
	/// <param name="num_boxes">Number of boxes that needs to be allocated</param>
	void allocate_boxes(short num_boxes) {
		boxes = new box[num_boxes];
	}

	/// <summary>
	/// Prints the container boxes and some additional information.
	/// </summary>
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

		cout << number_boxes << " Boxes to be placed" << endl;
		//boxes
		for (int i = 0; i < number_boxes; i++) {
			cout << boxes[i].name << " (" << (int)boxes[i].width << "x" << (int)boxes[i].length << ")" << endl;
			for (int j = 0; j < boxes[i].length; j++) {
				cout << string(boxes[i].width, boxes[i].name) << endl;
			}
			cout << endl;
		}

	};
};

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

PackerProblem* loadPackerProblem(string filename) {
	PackerProblem* container = new PackerProblem();
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
	container->width = stoi(newLine);
		
	getline(file, newLine, '\n');
	check_int(&newLine);
	container->length = stoi(newLine);

	getline(file, newLine);
	check_int(&newLine);
	container->number_boxes = stoi(newLine);
	container->allocate_boxes(container->number_boxes);

	areaContainer = container->width * container->length;
	
	
	
	//Reading and initiating box sizes and their names
	while (file.good()) {
		getline(file, newLine, ' ');
		check_int(&newLine);
		container->boxes[counter].length = stoi(newLine);

		getline(file, newLine, ' ');
		check_int(&newLine);
		container->boxes[counter].width = stoi(newLine);

		//No input checks for the name. We expect any char.
		getline(file, newLine, '\n');
		container->boxes[counter].name = newLine[0];

		sumAreaBoxes = sumAreaBoxes + (int)container->boxes[counter].length * (int)container->boxes[counter].width;
		counter++;
	}

	if (sumAreaBoxes > areaContainer) {
		cout << "The problem is unsolvable!" << endl;
		cout << "The area of boxes exceed the area of the container->" << endl;
		cout << "Container area: " << areaContainer << " " << " Sum of the boxes' area: " << sumAreaBoxes << endl;
		exit(3);
	}

	if (counter != container->number_boxes) {
		cout << "The file is currupted!" << endl;
		cout << "Incorrect number of boxes provided/specified." << endl;
		exit(3);
	}

	//TODO After solving, compare to unused space and calulate efficiency
	cout << "The optimal solution would use " << sumAreaBoxes << " square sm of space, leaving empty " << areaContainer - sumAreaBoxes << " sq.sm. of space." << endl << endl;
	
	container->print();

	return container;
}


//TODO Delete integer casting when qsort is woring
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
	

	qsort(pC->boxes, 25, sizeof(pC->boxes[0]), compare);

	cout << "ok";

};
//TODO Ask Questions:  2. doesn't hueristics oppose the "find all solutions" additional requirement 3. Function to print the box configurations and the container in the PackerProblem DS