#ifndef AI_H
#define AI_H
#include "Snake.h"

typedef struct _HeapNode{
	int location;
	int F;
}SHeapNode, * ptrSHeapNode;

typedef struct _Node{
	int G;
	bool inOpen;
	bool inClose;
	bool isInited;
}SNode, * ptrSNode;

class CSnakeAI{
public:
	CSnakeAI(CSnake &snake);
	~CSnakeAI();
	SnakeMovement AICallBack();
private:
	CSnake &snake;
	ptrSHeapNode OpenList;
	ptrSNode mapNodes;
	int OpenListLen;

	void initMemory();
	void freeMemory();
	void HeapRemoveRoot();

};

#endif