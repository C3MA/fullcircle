
#include "snake.h"
#include <stdlib.h>
#include <ctime>
#include <string.h>
#include <iostream>
#include <iomanip>

Snake::Snake(int x, int y)
{
	dimX = x;
	dimY = y;
	pos.x = dimX / 2;
	pos.y = dimY / 2;
	position tmp;
	tmp.x = pos.x;
	tmp.y = pos.y-2;
	snake.push_front(tmp);
	tmp.y++;
	snake.push_front(tmp);
	tmp.y++;
	snake.push_front(tmp);
	dir = 2;
	lastdir = 2;
	length = 3;
	srand(time(0));
	noms.x = rand() % dimX;
	noms.y = rand() % dimY;
	grid = new unsigned char[dimX*dimY];
	_alive = true;
}

Snake::~Snake()
{
	delete[] grid;
}

void Snake::newNoms()
{
	noms.x = rand() % dimX;
	noms.y = rand() % dimY;
	//check for collisions
	for(std::list<position>::iterator itr = snake.begin(); itr !=  snake.end(); ++itr)
	{
		position tmp = *itr;
		if(tmp.x == noms.x && tmp.y == noms.y)
		{
			newNoms();
		}
	}
}

void Snake::move()
{
	switch(dir)
	{
		case 1:
			pos.x += 1;
			if(pos.x >= dimX)
				pos.x = 0;
		break;
		case 2:
			pos.y += 1;
			if(pos.y >= dimY)
				pos.y = 0;
		break;
		case 3:
			pos.x -= 1;
			if(pos.x < 0)
				pos.x = dimX-1;
		break;
		case 4:
			pos.y -= 1;
			if(pos.y < 0)
				pos.y = dimY-1;
		break;
	}
	lastdir = dir;
	if(pos.x == noms.x && pos.y == noms.y)
	{
		length += 1;
		newNoms();
	}
	if(snake.size() < length)
	{
	}
	else
	{
	//	snake.push_front(pos);
		snake.pop_back();
	}
	//check for collisions
	for(std::list<position>::iterator itr = snake.begin(); itr !=  snake.end(); ++itr)
	{
		position tmp = *itr;
		if(tmp.x == pos.x && tmp.y == pos.y)
		{
			std::cout << "you lost!" << std::endl;
			std::cout << "your score: " << length << std::endl;
			_alive=false;
//			exit(0);
		}
	}
	snake.push_front(pos);


}

void Snake::render()
{
	memset(grid,0,dimX*dimY);
	grid[noms.x*dimY+noms.y]=0x11;
	int intensity = 255;
	int intstep = (intensity-80)/snake.size();
	for(std::list<position>::iterator itr = snake.begin(); itr !=  snake.end(); ++itr)
	{
		position tmp = *itr;
		if(tmp.x < dimX && tmp.y < dimY)
		{
			grid[tmp.x*dimY+tmp.y] = intensity;
			intensity -= intstep;
		}
	}
	
}

void Snake::debugPrint()
{
	using namespace std;
	cout.flags(ios::hex);
	//cout <<  setfill(' ') << setw(4);
	cout << endl;
	for (int y = 0 ; y < dimY; y++)
	{
		for(int x = 0; x < dimX; x++)
		{
			cout <<  setw(3) << (int)grid[x*dimY+y];
		}
		cout << endl;
	}

}

bool Snake::alive()
{
	return _alive;
}
