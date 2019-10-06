#pragma once
#include <Arduino.h>

struct node
{
	volatile int val;
	volatile node *next;
	volatile node *prev;
};
typedef struct node Node;

class MM_Queue
{
public:
	MM_Queue();

	void enqueue(int val);
	int dequeue();
	void clear();

	int size;

private:
	volatile Node *head;
	volatile Node *tail;
	int max_len;
};
