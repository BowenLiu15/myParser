#include <stdio.h>

struct SimpleQueueNode
{
	void* pData;
	struct SimpleQueueNode* nextNode;

};

class SimpleQueue
{
public:
	SimpleQueue();
	~SimpleQueue(void);

	int   Enqueue(void* pVoid);
	void* Dequeue(void);
	void* GetFirst();
	void* GetNext();
	int   getCount(void);

protected:

	struct SimpleQueueNode* pFirstNode;
	struct SimpleQueueNode* pCurrentNode;
	struct SimpleQueueNode* pEndNode;
	int                     tCount;
};

// SimpleQueue Constructor function
inline SimpleQueue::SimpleQueue()
{

	pFirstNode = NULL;
	pCurrentNode = NULL;
	pEndNode = NULL;
	tCount = 0;
}


inline SimpleQueue::~SimpleQueue(void)
{
}

// Enqueue() function

inline int SimpleQueue::getCount(void)
{
	return tCount;
}

inline int SimpleQueue::Enqueue(void* pVoid)
{
	struct SimpleQueueNode* pNewNode = new struct SimpleQueueNode;
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

inline void* SimpleQueue::Dequeue(void)
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
		struct SimpleQueueNode* pTempNode = pFirstNode;
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

inline void* SimpleQueue::GetFirst(void)
{
	pCurrentNode = pFirstNode;
	void* pData = pCurrentNode->pData;
	return pData;
}

inline void* SimpleQueue::GetNext(void)
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
