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

#define getFarther(c) ((((c) + 1) >> 1) - 1)
#define getLChild(c) (((c) << 1) + 1)
#define getRChild(c) (((c) << 1) + 2)

void CSnakeAI::AddHeapNode(int F, int location){
	OpenListLen++;
	OpenList[OpenListLen - 1].F = F;
	OpenList[OpenListLen - 1].location = location;

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

	OpenList[0] = OpenList[OpenListLen - 1];
	OpenListLen--;
	int ptr = 0;
	SHeapNode _tempNode;
	while(ptr < OpenListLen && getLChild(ptr) < OpenListLen){
		if(getRChild(ptr) < OpenListLen && OpenList[getLChild(ptr)].F > OpenList[getRChild(ptr)].F){
			if(OpenList[getRChild(ptr)].F < OpenList[ptr].F){
				_tempNode = OpenList[getRChild(ptr)];
				OpenList[getRChild(ptr)] = _tempNode;
				OpenList[ptr] = _tempNode;
				ptr = getRChild(ptr);
			}else{
				break;
			}
		}else{
			if(OpenList[getLChild(ptr)].F < OpenList[ptr].F){
				_tempNode = OpenList[getLChild(ptr)];
				OpenList[getLChild(ptr)] = _tempNode;
				OpenList[ptr] = _tempNode;
				ptr = getLChild(ptr);
			}else{
				break;
			}
		}
	}
}

ptrSHeapNode CSnakeAI::GetHeapRoot(){
	return OpenListLen > 0 ? OpenList : NULL;
}
