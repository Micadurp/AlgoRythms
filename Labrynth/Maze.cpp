#include "Maze.h"

#include <iostream>
#include <fstream>
#include <exception>


Maze::Maze()
{ 
}
Maze::~Maze()
{  
}

void Maze::generateMaze(int w, int h, bool useRank, bool useCompression)
{
	maze.clear();
	maze.resize(w * h);

	Maze::Wall *walls = new Maze::Wall[((w - 1)*h) + ((h - 1)*w)];
	int wallCount = 0;
	DisjointSet setGenerator(w * h, useRank, useCompression);

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int i = (y * (w)) + x;
			if (x > 0) { walls[wallCount++] = Maze::Wall(i, WEST); }
			if (y > 0) { walls[wallCount++] = Maze::Wall(i, NORTH); }

			setGenerator.Add(i);
		}
	}

	while (setGenerator.getSetCount() > 1)	//loop as loong as no valid room
	{

		//Get random wall
		int wallIndex = -1;
		if (wallCount)
		{
			wallIndex = rand() % wallCount;
			int a = walls[wallIndex].index;
			int b = (walls[wallIndex].wall == NORTH) ? (a - w) : (a - 1);

			//If sub-set is not in same set connect the two sets and rooms
			if (setGenerator.Find(a) != setGenerator.Find(b))
			{
				if (walls[wallIndex].wall == NORTH)
				{
					maze[a].north = NONE;
					maze[b].south = NONE;
				}
				else
				{
					maze[a].west = NONE;
					maze[b].east = NONE;
				}

				setGenerator.Union(a, b);
			}
			walls[wallIndex] = walls[wallCount - 1];
			wallCount--;
		}
	}

	delete[] walls;
}

vector<Maze::Grid> Maze::getMaze()
{
	return maze;
}

bool Maze::saveToBMP(BYTE* Buffer, int width, int height, long paddedsize, LPCTSTR bmpfile)
{
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER info;
	memset(&bmfh, 0, sizeof (BITMAPFILEHEADER));
	memset(&info, 0, sizeof (BITMAPINFOHEADER));

	bmfh.bfType = 0x4d42;       // 0x4d42 = 'BM'
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfSize = sizeof(BITMAPFILEHEADER)+
		sizeof(BITMAPINFOHEADER)+paddedsize;
	bmfh.bfOffBits = 0x36;


	info.biSize = sizeof(BITMAPINFOHEADER);
	info.biWidth = width;
	info.biHeight = height;
	info.biPlanes = 1;
	info.biBitCount = 24;
	info.biCompression = BI_RGB;
	info.biSizeImage = 0;
	info.biXPelsPerMeter = 0x0ec4;
	info.biYPelsPerMeter = 0x0ec4;
	info.biClrUsed = 0;
	info.biClrImportant = 0;

	HANDLE file = CreateFile(bmpfile, GENERIC_WRITE, FILE_SHARE_READ,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (NULL == file)
	{
		CloseHandle(file);
		return false;
	}

	unsigned long bwritten;
	if (WriteFile(file, &bmfh, sizeof (BITMAPFILEHEADER),
		&bwritten, NULL) == false)
	{
		CloseHandle(file);
		return false;
	}

	if (WriteFile(file, &info, sizeof (BITMAPINFOHEADER),
		&bwritten, NULL) == false)
	{
		CloseHandle(file);
		return false;
	}

	if (WriteFile(file, Buffer, paddedsize, &bwritten, NULL) == false)
	{
		CloseHandle(file);
		return false;
	}

	CloseHandle(file);
	return true;
}

BYTE* Maze::DrawMazeToBMPBuffer(int width, int height, long* newsize)
{
	int padding = 0;
	int scanlinebytes = width * 3;
	while ((scanlinebytes + padding) % 4 != 0)
		padding++;
	int psw = scanlinebytes + padding;


	*newsize = height * psw;
	BYTE* newbuf = new BYTE[*newsize];

	memset(newbuf, 0, *newsize);

	long bufpos = 0;
	long newpos = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < 3 * width; x += 3)
		{

			int mazeHeight = (y - 1) / 2;
			int mazeWidth = ((x/3) - 1) / 2;

			if (y % 2 == 0)
			{
				//even rows
				if (x % 2 == 0)
				{
					//even columns
					//draw black
					newpos = (height - y - 1) * psw + x; // position in padded buffer
					newbuf[newpos] = 0x00;
					newbuf[newpos + 1] = 0x00;
					newbuf[newpos + 2] = 0x00;
				}
				else
				{
					if (y == 0)
					{
						newpos = (height - y - 1) * psw + x; // position in padded buffer
						newbuf[newpos] = 0x00;
						newbuf[newpos + 1] = 0x00;
						newbuf[newpos + 2] = 0x00;
					}
					//odd columns
					//check north/south
					else if (maze.at((mazeHeight) * ((width - 1) / 2) + mazeWidth).south != Maze::SOUTH )
					{
						newpos = (height - y - 1) * psw + x; // position in padded buffer
						newbuf[newpos] = 0xFF;
						newbuf[newpos + 1] = 0xFF;
						newbuf[newpos + 2] = 0xFF;
					}
					else
					{
						newpos = (height - y - 1) * psw + x; // position in padded buffer
						newbuf[newpos] = 0x00;
						newbuf[newpos + 1] = 0x00;
						newbuf[newpos + 2] = 0x00;
					}
				}
			}
			else
			{
				//odd rows
				if (x % 2 == 0)
				{
					if (x == 0)
					{
						newpos = (height - y - 1) * psw + x; // position in padded buffer
						newbuf[newpos] = 0x00;
						newbuf[newpos + 1] = 0x00;
						newbuf[newpos + 2] = 0x00;
					}
					//even columns
					//check east/west
					else if ( maze.at((mazeHeight) * ((width - 1) / 2) + mazeWidth).east != Maze::EAST)
					{
						newpos = (height - y - 1) * psw + x; // position in padded buffer
						newbuf[newpos] = 0xFF;
						newbuf[newpos + 1] = 0xFF;
						newbuf[newpos + 2] = 0xFF;
					}
					else
					{
						newpos = (height - y - 1) * psw + x; // position in padded buffer
						newbuf[newpos] = 0x00;
						newbuf[newpos + 1] = 0x00;
						newbuf[newpos + 2] = 0x00;
					}
				}
				else
				{
					//odd columns

					//draw white
					newpos = (height - y - 1) * psw + x; // position in padded buffer
					newbuf[newpos] = 0xFF;
					newbuf[newpos + 1] = 0xFF;
					newbuf[newpos + 2] = 0xFF;
				}
			}
		}
	}

	//make entrence and exit

	newpos = (height - 1) * psw + 3; // position in padded buffer
	newbuf[newpos] = 0xFF;
	newbuf[newpos + 1] = 0xFF;
	newbuf[newpos + 2] = 0xFF;


	newpos = (width * 3) - 6; // position in padded buffer
	newbuf[newpos] = 0xFF;
	newbuf[newpos + 1] = 0xFF;
	newbuf[newpos + 2] = 0xFF;


	return newbuf;
}