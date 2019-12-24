#include "MM_Queue.h"

MM_Queue::MM_Queue(int len)
{
	head = NULL;
	tail = NULL;
	size = 0;
	max_len = len;
}

void MM_Queue::enqueue(int val) volatile
{
	if (size >= max_len) {
		clear();
	}

	Node *newNode = new Node();
	if (newNode)
	{	
		newNode->prev = NULL;
		newNode->data = val;
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
		size++;
	}
}

//return -1 if empty so we can avoid bad dac updates, will not update output on -1 in main.
int MM_Queue::dequeue() volatile
{
	int rtn = -1;
	if (head)
	{
		rtn = head->data;
		if (size > 1) {
			volatile Node *temp = head;
			head = head->prev;
			delete temp;
			size--;
		}
		else if (size == 1)
		{
			delete head;
			size--;
		}
	}

	return rtn;
}

void MM_Queue::clear() volatile
{
	volatile Node *current = head;
	while (current)
	{
		volatile Node *temp = current;
		current = current->prev;
		delete temp;
	}
	size = 0;
}

bool MM_Queue::hasNext() volatile
{
	return size > 0;
}