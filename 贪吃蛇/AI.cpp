#include <cstdlib>
#include <cstring>
#include "AI.h"

CSnakeAI::CSnakeAI(CSnake &snake) : snake(snake), OpenList(NULL), mapNodes(NULL){
	initMemory();
}

CSnakeAI::~CSnakeAI(){
	freeMemory();
}

SnakeMovement CSnakeAI::AICallBack(){
	return -1;
}

void CSnakeAI::initMemory(){
	OpenListLen = 0;
	if(OpenList == NULL)OpenList = new SHeapNode[WIDTH * HEIGHT];
	if(mapNodes == NULL)mapNodes = new SNode[WIDTH * HEIGHT];
	memset(OpenList, 0, sizeof(SHeapNode) * WIDTH * HEIGHT);
	memset(mapNodes, 0, sizeof(SNode) * WIDTH * HEIGHT);
}

void CSnakeAI::freeMemory(){
	if(OpenList != NULL)delete [] OpenList;
	if(mapNodes != NULL)delete [] mapNodes;
	OpenList = NULL;
	mapNodes = NULL;
}
