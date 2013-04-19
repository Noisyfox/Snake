#include "snake.h"
#include <cstdlib>
#include <ctime>
#include <gl/glut.h>
#include "GLUtil.h"

const int CSnake::movement[4][2] = {{1, 0}, {0, -1}, {0, 1}, {-1, 0}};

CSnake::CSnake(){
	srand((unsigned)time(0));

	snakeLength = 3;
	snakeDir = DIR_UP;
	snake[0][0] = WIDTH / 2;
	snake[0][1] = HEIGHT / 2 + 1;

	snake[1][0] = WIDTH / 2;
	snake[1][1] = HEIGHT / 2;

	snake[2][0] = WIDTH / 2;
	snake[2][1] = HEIGHT / 2 - 1;

	map[snake[0][1]][snake[0][0]] = 1;
	map[snake[1][1]][snake[1][0]] = 2;
	map[snake[2][1]][snake[2][0]] = 3;
	
	map[snake[0][1]][WIDTH] = 1;
	map[snake[1][1]][WIDTH] = 1;
	map[snake[2][1]][WIDTH] = 1;

	putCandyRandom();
}

CSnake::~CSnake(){
}

bool CSnake::moveTowards(int direction){
	if(direction > 3 || direction < 0 || 
		direction + snakeDir == 3)direction = snakeDir;

	int newHead_x = snake[snakeLength - 1][0] + movement[direction][0];
	int newHead_y = snake[snakeLength - 1][1] + movement[direction][1];

	if(newHead_x < 0 || newHead_x >= WIDTH 
		|| newHead_y < 0 || newHead_y >= HEIGHT)return false;

	if(map[newHead_y][newHead_x] == FLAG_CANDY){
		map[newHead_y][WIDTH]++;
		snakeLength++;
		snake[snakeLength - 1][0] = newHead_x;
		snake[snakeLength - 1][1] = newHead_y;
		map[newHead_y][newHead_x] = snakeLength;
		putCandyRandom();
	}else if(map[newHead_y][newHead_x] == FLAG_EMPTY || map[newHead_y][newHead_x] == 1){
		map[newHead_y][WIDTH]++;
		map[snake[0][1]][WIDTH]--;
		for(int i = 0; i < snakeLength - 1; i++){
			map[snake[i][1]][snake[i][0]]--;
			snake[i][0] = snake[i + 1][0];
			snake[i][1] = snake[i + 1][1];
		}
		map[snake[snakeLength - 1][1]][snake[snakeLength - 1][0]]--;
		snake[snakeLength - 1][0] = newHead_x;
		snake[snakeLength - 1][1] = newHead_y;
		map[newHead_y][newHead_x] = snakeLength;
	}else{
		return false;
	}

	snakeDir = direction;
	return true;
}

void CSnake::putCandyRandom(){
	if(map[candy[1]][candy[0]] == FLAG_CANDY){
		map[candy[1]][candy[0]] = FLAG_EMPTY;
	}

	//int ran = rand();
	int loc = rand() % (HEIGHT * WIDTH - snakeLength);
	//int loc = (int)(ran / (RAND_MAX + 1.0) * k);
	for(int i = 0; i < HEIGHT; i++){
		if(loc < WIDTH - map[i][WIDTH]){
			for(int j = 0; j < WIDTH; j++){
				if(map[i][j] == FLAG_EMPTY){
					if(loc <= 0){
						if(loc < 0)loc = 0;
						candy[0] = j;
						candy[1] = i;
						map[i][j] = FLAG_CANDY;
						break;
					}else{
						loc--;
					}
				}
			}
			break;
		}else{
			loc -= WIDTH - map[i][WIDTH];
		}
	}
}

void CSnake::drawSnake(){
	glLineWidth(LINEWIDTH);

	glColor3f(255, 255, 255);
	glBegin(GL_LINE_LOOP);
	{
		glVertex2f(EDGE, EDGE);
		glVertex2f(EDGE * 3 + WIDTH * PIXELUNIT, EDGE);
		glVertex2f(EDGE * 3 + WIDTH * PIXELUNIT, EDGE * 3 + HEIGHT * PIXELUNIT);
		glVertex2f(EDGE, EDGE * 3 + HEIGHT * PIXELUNIT);
	}
	glEnd();
	
	glLineWidth(1);
	for(int i = snakeLength - 2; i >= 0; i--){
		GLfloat x = snake[i][0] * PIXELUNIT + EDGE * 2;
		GLfloat y = snake[i][1] * PIXELUNIT + EDGE * 2;
		glColor3f(255, 255, 255);
		glBegin(GL_QUADS);
		{  
			glVertex2f(x, y);    
			glVertex2f(x, y + PIXELUNIT);     
			glVertex2f(x + PIXELUNIT, y + PIXELUNIT);     
			glVertex2f(x + PIXELUNIT, y);
		}
		glEnd();
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		{  
			glVertex2f(x, y);    
			glVertex2f(x, y + PIXELUNIT);     
			glVertex2f(x + PIXELUNIT, y + PIXELUNIT);     
			glVertex2f(x + PIXELUNIT, y);
		}
		glEnd();
	}
	GLfloat x = snake[snakeLength - 1][0] * PIXELUNIT + EDGE * 2;
	GLfloat y = snake[snakeLength - 1][1] * PIXELUNIT + EDGE * 2;
	GLfloat vi[4][2];
	switch(snakeDir){
	case DIR_RIGHT:
		vi[0][0] = x + PIXELUNIT;
		vi[0][1] = y + PIXELUNIT * 0.25;
		vi[1][0] = x + PIXELUNIT;
		vi[1][1] = y + PIXELUNIT * 0.75;
		vi[2][0] = x;
		vi[2][1] = y + PIXELUNIT;
		vi[3][0] = x;
		vi[3][1] = y;
		break;
	case DIR_UP:
		vi[0][0] = x + PIXELUNIT * 0.25;
		vi[0][1] = y;
		vi[1][0] = x + PIXELUNIT * 0.75;
		vi[1][1] = y;
		vi[2][0] = x + PIXELUNIT;
		vi[2][1] = y + PIXELUNIT;
		vi[3][0] = x;
		vi[3][1] = y + PIXELUNIT;
		break;
	case DIR_LEFT:
		vi[0][0] = x;
		vi[0][1] = y + PIXELUNIT * 0.25;
		vi[1][0] = x;
		vi[1][1] = y + PIXELUNIT * 0.75;
		vi[2][0] = x + PIXELUNIT;
		vi[2][1] = y + PIXELUNIT;
		vi[3][0] = x + PIXELUNIT;
		vi[3][1] = y;
		break;
	case DIR_DOWN:
		vi[0][0] = x + PIXELUNIT * 0.25;
		vi[0][1] = y + PIXELUNIT;
		vi[1][0] = x + PIXELUNIT * 0.75;
		vi[1][1] = y + PIXELUNIT;
		vi[2][0] = x + PIXELUNIT;
		vi[2][1] = y;
		vi[3][0] = x;
		vi[3][1] = y;
		break;
	}
	
	glColor3f(255, 255, 255);
	glBegin(GL_QUADS);
		{  
			glVertex2fv(vi[0]);
			glVertex2fv(vi[1]);
			glVertex2fv(vi[2]);
			glVertex2fv(vi[3]);
		}
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
		{  
			glVertex2fv(vi[0]);
			glVertex2fv(vi[1]);
			glVertex2fv(vi[2]);
			glVertex2fv(vi[3]);
		}
	glEnd();
	
	glColor3f(255, 255, 255);
	//glPointSize(POINTSIZE);
	//glBegin(GL_POINTS);
	//{
	//	glVertex2f((candy[0] + 0.5) * PIXELUNIT + EDGE * 2,
	//		(candy[1] + 0.5) * PIXELUNIT + EDGE * 2);
	//}
	//glEnd();
	uDrawSolidCircle((candy[0] + 0.5) * PIXELUNIT + EDGE * 2,
		(candy[1] + 0.5) * PIXELUNIT + EDGE * 2, POINTSIZE, 10);
}
