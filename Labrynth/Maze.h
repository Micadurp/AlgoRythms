#ifndef MAZE_H
#define MAZE_H

#include "DisjointSet.h"
#include <windows.h>
#include <stdio.h> 


class Maze
{
public:
	enum WallOrientation
	{
		NORTH = 0,
		EAST = 1,
		SOUTH = 2,
		WEST = 3,

		NONE = 4,
	};
	struct Wall
	{
		int index;
		WallOrientation wall;

		Wall()
		{
			index = -1; wall = NONE;
		}
		Wall(int _index, WallOrientation _wall)
		{
			index = _index;
			wall = _wall;
		}
		Wall(const Wall& obj)
		{
			this->index = obj.index;
			this->wall = obj.wall;
		}
		bool operator!=(const Wall& obj)
		{
			return	(this->index != obj.index)
				&&
				(this->wall != obj.wall);
		}
		bool operator==(const Wall& obj)
		{
			return (this->index == obj.index)
				&&
				(this->wall == obj.wall);
		}
	};
	struct Grid
	{
		WallOrientation north;
		WallOrientation east;
		WallOrientation south;
		WallOrientation west;

		Grid()
		{
			north = NORTH;
			east = EAST;
			south = SOUTH;
			west = WEST;
		}
	};




	Maze();
	virtual~Maze();

	void generateMaze(int w, int h,  bool useRank = true, bool useCompression = true);

	vector<Maze::Grid> getMaze();

	bool saveToBMP(BYTE* Buffer, int width, int height, long paddedsize, LPCTSTR bmpfile);

	BYTE* DrawMazeToBMPBuffer(int width, int height, long* newsize);

private:
	vector<Maze::Grid> maze;
};

#endif