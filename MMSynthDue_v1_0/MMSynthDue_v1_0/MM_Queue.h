#pragma once
#include <Arduino.h>

struct node
{
	int data;  //? will be notes or midi messages?
	node *prev;
};
typedef struct node Node;

class MM_Queue
{
public:
	MM_Queue(int len);
	void enqueue(int val) volatile;
	int dequeue() volatile;
	bool hasNext() volatile;

private:
	void clear() volatile;
	volatile Node *head;
	volatile Node *tail;
	volatile int max_len;
	volatile int size;
};
