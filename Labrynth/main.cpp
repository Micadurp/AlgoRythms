#include <iostream>
#include <ctime>
#include <string>

#include "Maze.h"

using namespace std;


void CreateForest(int, int);

int main()
{ 
	int WIDTH;
	int HEIGHT;
	Maze maze;

	cout << "Enter width: ";
	cin >> WIDTH;
	cout << "Enter height: ";
	cin >> HEIGHT;
	cout << endl;
	cin.ignore();

	CreateForest(WIDTH, HEIGHT);

	system("PAUSE");

	return 0;
}

void CreateForest(int w, int h)
{
	Maze maze;
	vector<Maze::Grid> mazeRC;
	clock_t start = clock();
	maze.generateMaze(w, h);
	clock_t end = clock();
	clock_t total = end - start;
	cout << " Generating with RANK and COMPRESSION heuristics (" << total << ")" << endl;


	cout << "Enter filename: ";
	string filename = "";
	cin >> filename;
	filename = ".\\Mazes\\" + filename + ".bmp";

	long padSize = 0;
	int BMPWidth = w*2 + 1;
	int BMPHeight = h*2 + 1;

	BYTE* BMPMaze = maze.DrawMazeToBMPBuffer(BMPWidth, BMPHeight, &padSize);
	maze.saveToBMP(BMPMaze, BMPWidth, BMPHeight, padSize, filename.c_str());
}