#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>


using namespace std;


typedef struct PackerProblem {
	unsigned short numberOfBoxes;
	unsigned char width;
	unsigned char length;
	box boxes[];

};

typedef struct box {
	unsigned char width;
	unsigned char length;
	unsigned char name;

};

PackerProblem* loadPackerProblem(string filename) {
	ifstream file(filename);

	
	string firstLine;

	getline(file, firstLine);
	getline(file, firstLine);
	PackerProblem problem = {(char) (*firstLine - "0"), 20, 20, 20};

	while (file.good()) {
		problem.boxes[0].length = 
	}
}



int main() {
	PackerProblem container = {2000, 20};
	
	cout << (int)container.width;
	

	
}