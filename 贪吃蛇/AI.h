#ifndef AI_H
#define AI_H
#include "Snake.h"

typedef struct _HeapNode{
	int location;
	int F;
}SHeapNode, * ptrSHeapNode;

typedef struct _Node{
	int G, H;
	int father;
	bool inOpen;
	bool inClose;
}SNode, * ptrSNode;

class CSnakeAI{
public:
	CSnakeAI(CSnake &snake);
	~CSnakeAI();
	SnakeMovement AICallBack();
private:
	CSnake &snake;
	ptrSHeapNode OpenList;
	int OpenListLen;
	ptrSNode mapNodes;
	int * pathLocations;
	int pathLength;

	void initMemory();
	void freeMemory();
	
	void AddHeapNode(int F, int location);
	void RemoveHeapRoot();
	void UpdateF(int location, int newF);
	ptrSHeapNode GetHeapRoot();

	int doAASearch();//返回路径长度
	int checkAround(int loction);
};

#endif
