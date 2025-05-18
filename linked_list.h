#pragma once
#include <stdint.h>
typedef struct IntNode IntNode;

struct IntNode{
	IntNode* previous;
	IntNode* next;
	uint32_t value;
};

typedef struct {
	IntNode* start;
	IntNode* center;
	IntNode* end;
	uint32_t size;
} IntLinkedList;

IntLinkedList* InitIntLinkedList(uint32_t values[], int length);

void DestroyIntLinkedList(IntLinkedList* linkedList);

uint32_t RemoveIntNodeAt(IntLinkedList* linked, uint32_t idx);
void PutIntNodeAt(IntLinkedList* linked, uint32_t value, uint32_t idx);

// removes at first position
uint32_t PopIntNode(IntLinkedList* linkedList);
// inserts at first position
void PushIntNode(IntLinkedList* linkedList, uint32_t value);

void AppendIntNode(IntLinkedList* linkedList, uint32_t value);

uint32_t RemoveTail(IntLinkedList* linkedList);

void PrintIntLinkedList(IntLinkedList* linked);
void PrintIntLinkedListReversed(IntLinkedList* linked);