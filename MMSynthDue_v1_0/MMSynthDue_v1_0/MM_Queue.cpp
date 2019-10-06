#include "MM_Queue.h"

MM_Queue::MM_Queue()
{
	head = NULL;
	tail = NULL;
	size = 0;
	max_len = 10;
}

void MM_Queue::enqueue(int val)
{
	if (size == max_len) {
		size = 0;
		clear();
	}

	volatile Node *newNode = new Node();
	if (newNode)
	{
		newNode->val = val;
		if (head)
		{
			tail->prev = newNode;
			tail = newNode;
		}
		else
		{
			head = newNode;
			tail = newNode;
		}
	}

	size++;
}

int MM_Queue::dequeue()
{
	int rtn = -1;
	if (head)
	{
		rtn = head->val;
		if (size > 1) {
			volatile Node *temp = head;
			head = head->prev;
			delete temp;
			size--;
		}
	}
	return rtn;
}

void MM_Queue::clear()
{
	volatile Node *current = head;
	while (current)
	{
		volatile Node *temp = current;
		current = current->prev;
		delete temp;
	}
}