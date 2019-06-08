#include <stdio.h>

struct QueueNode
{
	void* pData;
	struct QueueNode* nextNode;

};

class Queue
{
public:
	Queue();
	~Queue(void);

	int       Enqueue(void* pVoid);
	void* Dequeue(void);
	void* GetFirst();
	void* GetNext();
	int getCount(void);



protected:

	struct QueueNode* pFirstNode;
	struct QueueNode* pCurrentNode;
	struct QueueNode* pEndNode;
	int tCount;
};

// Queue Constructor function
Queue::Queue()
{

	pFirstNode = NULL;
	pCurrentNode = NULL;
	pEndNode = NULL;
	tCount = 0;
}


Queue::~Queue(void)
{
}

// Enqueue() function

int Queue::getCount(void)
{
	return tCount;
}

int Queue::Enqueue(void* pVoid)
{

	struct QueueNode* pNewNode = new struct QueueNode;
	pNewNode->pData = pVoid;
	pNewNode->nextNode = NULL;

	if (pFirstNode == NULL)
	{
		pFirstNode = pNewNode;
		pCurrentNode = pFirstNode;
		pEndNode = pFirstNode;
		tCount = 1;
	}
	else
	{
		pEndNode->nextNode = pNewNode;
		pEndNode = pNewNode;
		tCount += 1;
	}

	return 0;
}

// Dequeue() function

void* Queue::Dequeue(void)
{
	void* pData = pFirstNode->pData;
	if (tCount == 1)
	{
		pFirstNode = NULL;
		pCurrentNode = NULL;
		pEndNode = NULL;
		tCount = 0;
	}
	else
	{
		struct QueueNode* pTempNode = pFirstNode;
		if (pCurrentNode == pFirstNode)
		{
			pCurrentNode = pFirstNode->nextNode;
		}
		pFirstNode = pFirstNode->nextNode;
		tCount--;
		delete pTempNode;
	}


	return pData;
}

void* Queue::GetFirst(void)
{
	pCurrentNode = pFirstNode;
	void* pData = pCurrentNode->pData;
	return pData;
}

void* Queue::GetNext(void)
{

	if (pCurrentNode->nextNode == NULL)
	{
		return NULL;
	}
	else
	{
		pCurrentNode = pCurrentNode->nextNode;
		return pCurrentNode->pData;
	}
}
