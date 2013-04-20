#include <cstdlib>
#include <cstring>
#include "AI.h"
#include <cstdio>

// 实用宏定义
#define getLocation(x, y) ((x) + (y) * WIDTH)
#define getLLocation(l) ((l) - 1)
#define getRLocation(l) ((l) + 1)
#define getULocation(l) ((l) - WIDTH)
#define getDLocation(l) ((l) + WIDTH)
#define getX(c) ((c) % WIDTH)
#define getY(c) ((c) / WIDTH)

#define calcF(n) ((n).G + (n).H)

#define getFarther(c) ((((c) + 1) >> 1) - 1)
#define getLChild(c) (((c) << 1) + 1)
#define getRChild(c) (((c) << 1) + 2)

CSnakeAI::CSnakeAI(CSnake &snake) : snake(snake), OpenList(NULL), mapNodes(NULL), pathLocations(NULL){
	initMemory();
}

CSnakeAI::~CSnakeAI(){
	freeMemory();
}

SnakeMovement CSnakeAI::AICallBack(){
	if(pathLength <= 1){
		initMemory();
		int result = doAASearch();
		if(result == 0)return -1;
	}
	int loc1 = pathLocations[pathLength - 1];
	int loc2 = pathLocations[pathLength - 2];
	int cX = getX(loc1);
	int cY = getY(loc1);
	int tX = getX(loc2);
	int tY = getY(loc2);

	pathLength--;

	for(int i = 0; i < 4; i++){
		if(cX + CSnake::movement[i][0] == tX 
			&& cY + CSnake::movement[i][1] == tY)return i;
	}

	return -1;
}

void CSnakeAI::initMemory(){
	OpenListLen = 0;
	pathLength = 0;
	if(OpenList == NULL)OpenList = new SHeapNode[WIDTH * HEIGHT];
	if(mapNodes == NULL)mapNodes = new SNode[WIDTH * HEIGHT];
	if(pathLocations == NULL)pathLocations = new int[WIDTH * HEIGHT];
	memset(mapNodes, 0, sizeof(SNode) * WIDTH * HEIGHT);
}

void CSnakeAI::freeMemory(){
	if(OpenList != NULL)delete [] OpenList;
	if(mapNodes != NULL)delete [] mapNodes;
	if(pathLocations != NULL)delete [] pathLocations;
	OpenList = NULL;
	mapNodes = NULL;
	pathLocations = NULL;
}

void CSnakeAI::AddHeapNode(int F, int location){
	OpenListLen++;
	OpenList[OpenListLen - 1].F = F;
	OpenList[OpenListLen - 1].location = location;

#ifdef DEBUG
	printf("A%d,%d\n", OpenListLen,location);
#endif

	int ptr = OpenListLen - 1;
	SHeapNode _tempNode;
	while(ptr > 0 && OpenList[ptr].F < OpenList[getFarther(ptr)].F){
		_tempNode = OpenList[ptr];
		OpenList[ptr] = OpenList[getFarther(ptr)];
		OpenList[getFarther(ptr)] = _tempNode;
		ptr = getFarther(ptr);
	}
}

void CSnakeAI::RemoveHeapRoot(){
	if(OpenListLen <= 0)return;

#ifdef DEBUG
	printf("C%d,%d\n", OpenListLen,OpenList[0].location);
	for(int i = 0; i < WIDTH * HEIGHT; i++){
		if(i != 0 && i % WIDTH == 0)printf("\n");
		if(getLocation(snake.snake[snake.snakeLength - 1][0], snake.snake[snake.snakeLength - 1][1]) == i)printf("S");
		else if(mapNodes[i].inOpen)printf("O");
		else if(mapNodes[i].inClose)printf("C");
		else printf("N");
	}
	printf("\n\n");
#endif

	OpenList[0] = OpenList[OpenListLen - 1];
	OpenListLen--;
	int ptr = 0;
	SHeapNode _tempNode;
	while(getLChild(ptr) < OpenListLen){
		if(getRChild(ptr) < OpenListLen && OpenList[getLChild(ptr)].F > OpenList[getRChild(ptr)].F){
			if(OpenList[getRChild(ptr)].F < OpenList[ptr].F){
				_tempNode = OpenList[getRChild(ptr)];
				OpenList[getRChild(ptr)] = OpenList[ptr];
				OpenList[ptr] = _tempNode;
				ptr = getRChild(ptr);
			}else{
				break;
			}
		}else{
			if(OpenList[getLChild(ptr)].F < OpenList[ptr].F){
				_tempNode = OpenList[getLChild(ptr)];
				OpenList[getLChild(ptr)] = OpenList[ptr];
				OpenList[ptr] = _tempNode;
				ptr = getLChild(ptr);
			}else{
				break;
			}
		}
	}
}

void CSnakeAI::UpdateF(int location, int newF){
	int ptr = -1;
	for(int i = 0; i < OpenListLen; i++){
		if(OpenList[i].location == location){
			ptr = i;
			break;
		}
	}
	if(ptr == -1)return;
	OpenList[ptr].F = newF;
	//更新位置,由于新的F只可能更小，所以不考虑F变大的情况
	SHeapNode _tempNode;
	while(ptr > 0 && OpenList[ptr].F < OpenList[getFarther(ptr)].F){
		_tempNode = OpenList[ptr];
		OpenList[ptr] = OpenList[getFarther(ptr)];
		OpenList[getFarther(ptr)] = _tempNode;
		ptr = getFarther(ptr);
	}
}

ptrSHeapNode CSnakeAI::GetHeapRoot(){
	return OpenListLen > 0 ? OpenList : NULL;
}

int CSnakeAI::doAASearch(){
	// prepare
	int cloc = getLocation(snake.snake[snake.snakeLength - 1][0], snake.snake[snake.snakeLength - 1][1]);
	int tloc = getLocation(snake.candy[0], snake.candy[1]);
	mapNodes[cloc].inOpen = true;
	AddHeapNode(calcF(mapNodes[cloc]), cloc);

	while(cloc != tloc && OpenListLen > 0){
		cloc = GetHeapRoot()->location;
		RemoveHeapRoot();
		mapNodes[cloc].inOpen = false;
		mapNodes[cloc].inClose = true;
		checkAround(cloc);
		ptrSHeapNode root = GetHeapRoot();
		if(root != NULL){
			cloc = root->location;
		}else{
			//没有路径了

#ifdef DEBUG
			for(int i = 0; i < WIDTH * HEIGHT; i++){
				if(i != 0 && i % WIDTH == 0)printf("\n");
				if(getLocation(snake.snake[snake.snakeLength - 1][0], snake.snake[snake.snakeLength - 1][1]) == i)printf("S");
				else if(i == tloc)printf("T");
				else if(i == cloc)printf("K");
				else if(mapNodes[i].inOpen)printf("O");
				else if(mapNodes[i].inClose)printf("C");
				else printf("N");
			}
			printf("\n\n");
#endif

			pathLength = 0;
			return 0;
		}

	}

	pathLength = mapNodes[cloc].G + 1;

	for(int i = 0; i < pathLength; i++){
		pathLocations[i] = cloc;
		cloc = mapNodes[cloc].father;
	}

	return pathLength;
}

int CSnakeAI::checkAround(int location){
	int newG = mapNodes[location].G + 1;
	int newAdd = 0;

	// 检测4个方向
	for(int i = 0; i < 4; i++){
		int newX = getX(location) + CSnake::movement[i][0];
		int newY = getY(location) + CSnake::movement[i][1];

		if(newX < 0 || newX >= WIDTH 
			|| newY < 0 || newY >= HEIGHT)continue;

		//判断是不是蛇身
		if(snake.map[newY][newX] > newG)continue;

		int loc = getLocation(newX, newY);
		SNode & nextNode = mapNodes[loc];

		if(nextNode.inClose)continue;

		if(nextNode.inOpen){
			if(nextNode.G > newG){
				nextNode.G = newG;
				nextNode.father = location;
				UpdateF(loc, calcF(nextNode));
			}
		}else{
			nextNode.father = location;
			nextNode.inOpen = true;
			nextNode.G = newG;
			nextNode.H = abs(snake.candy[0] - newX) + abs(snake.candy[1] - newY);
			AddHeapNode(calcF(nextNode), loc);
			newAdd++;
		}
	}

	return newAdd;
}
