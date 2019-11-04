#pragma once
#include <Arduino.h>

struct node
{
	int val;
	//you never use next
	node *next;
	node *prev;
};
typedef struct node Node;

class MM_Queue
{
public:
	MM_Queue();
	int size;
	void enqueue(int val) volatile;
	int dequeue() volatile;

private:
	void clear() volatile;
	Node *head;
	Node *tail;
	int max_len;
};
