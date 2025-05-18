#pragma once
#include <stdint.h>
typedef struct IntNode IntNode;

struct IntNode{
	IntNode* previous;
	IntNode* next;
	int64_t value;
};

typedef struct {
	IntNode* start;
	IntNode* center;
	IntNode* end;
	int64_t size;
} IntLinkedList;

IntLinkedList* InitIntLinkedList(int64_t values[], int length);

void DestroyIntLinkedList(IntLinkedList* linkedList);

int64_t RemoveIntNodeAt(IntLinkedList* linked, int64_t idx);
void PutIntNodeAt(IntLinkedList* linked, int64_t value, int64_t idx);

// removes at first position
int64_t PopIntNode(IntLinkedList* linkedList);
// inserts at first position
void PushIntNode(IntLinkedList* linkedList, int64_t value);

void AppendIntNode(IntLinkedList* linkedList, int64_t value);

int64_t RemoveTail(IntLinkedList* linkedList);

void PrintIntLinkedList(IntLinkedList* linked);
void PrintIntLinkedListReversed(IntLinkedList* linked);