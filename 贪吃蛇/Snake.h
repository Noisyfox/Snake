#ifndef SNAKE
#define SNAKE
#include "SnakeConfig.h"
#define DIR_RIGHT 0
#define DIR_UP 1
#define DIR_DOWN 2
#define DIR_LEFT 3

#define FLAG_EMPTY 0
#define FLAG_CANDY -1

class CSnake{
public:
	const static int movement[4][2];
	int snakeLength;
	int snakeDir;//0ср1ио3вС2об
	int map[HEIGHT][WIDTH+1];
	int snake[HEIGHT * WIDTH][2];
	int candy[2];
	CSnake();
	~CSnake();
	void putCandyRandom();
	bool moveTowards(int direction);
	void drawSnake();
};

#endif
