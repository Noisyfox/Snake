#ifndef SNAKE_H
#define SNAKE_H
#include "SnakeConfig.h"
#define DIR_RIGHT 0
#define DIR_UP 1
#define DIR_DOWN 2
#define DIR_LEFT 3

#define FLAG_EMPTY 0
#define FLAG_CANDY -1

typedef int SnakeMovement;

class CSnake{
public:
	const static int movement[4][2];
	int snakeLength;
	SnakeMovement snakeDir;
	int map[HEIGHT][WIDTH+1];
	int snake[HEIGHT * WIDTH][2];
	int candy[2];
	CSnake();
	~CSnake();
	void putCandyRandom();
	bool moveTowards(SnakeMovement direction);
	void drawSnake();
};

#endif
