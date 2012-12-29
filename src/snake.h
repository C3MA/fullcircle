
#ifndef _SNAKE_H_
#define _SNAKE_H_

//#define GRIDX 10
//#define GRIDY 12

#include <list>

struct position{
	int x, y;
};

class Snake
{
	public:
 
	Snake(int x, int y);

	~Snake();

	void move();

	void render();

	void newNoms();

	void debugPrint();

	bool alive();

	position pos;

	unsigned int length;

	unsigned char *grid;

	std::list<position> snake;

	int dimX, dimY;

	int dir; // 4 UP 1 RIGHT 2 DOWN 3 LEFT

	position noms;

	bool _alive;

	int lastdir;


};

#endif
