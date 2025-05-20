#include "linked_list.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


void PrintIntLinkedList(IntLinkedList* linked) {
	if (!linked->size) {
		printf("Empty List !!! ...\n");
		return;
	}
		
	IntNode* iter = linked->start;
	int i = 0;
	while (1) {
		printf("Node %d : %d\n", i, iter->value);
		if (iter->next == NULL)
			break;
		i++;
		iter = iter->next;
	}
}

void PrintIntLinkedListReversed(IntLinkedList* linked) {
	if (!linked->size) {
		printf("Empty List !!! ...\n");
		return;
	}

	IntNode* iter = linked->end;
	int i = linked->size - 1;
	while (1) {
		printf("Node %d : %d\n", i, iter->value);
		if (iter->previous == NULL)
			break;
		i--;
		iter = iter->previous;
	}
}

inline uint32_t CenterIdx(IntLinkedList* linkedList) {
	if (!linkedList->size)
		return 0;
	return ceil(linkedList->size / 2.0) - 1;
}

inline uint32_t IsEmpty(IntLinkedList* linked) {
	return !linked->size;
}
IntNode* IterNode(IntNode* node, int64_t iter, uint8_t leftRight) {
	if (iter == 0)
		return node;
	if (leftRight)
		return IterNode(node->next, iter - 1, leftRight);
	return IterNode(node->previous, iter - 1, leftRight);
}

void UpdateCenterIdx(IntLinkedList* linked, int64_t oldIdx) {
	if (IsEmpty(linked)) {
		linked->center = NULL;
		return;
	}
	if (linked->size == 1) {
		linked->center = linked->start;
		return;
	}

	IntNode* center = linked->center;
	uint32_t centerIdx = CenterIdx(linked);
	int64_t distance = centerIdx - oldIdx;
	uint32_t leftRight = centerIdx > oldIdx;

	uint32_t iter = leftRight ? distance : distance * -1;

	IntNode* newCenter = IterNode(center, iter, leftRight);
	linked->center = newCenter;
}

IntLinkedList* InitIntLinkedList(int64_t values[], int length) {
	IntLinkedList* linked = malloc(sizeof(IntLinkedList));
	linked->size = length;
	if (!length) {
		linked->start = NULL;
		linked->end = NULL;
		linked->center = NULL;
		return linked;	
	}
	IntNode** p = malloc(sizeof(IntNode*) * length);
	for (uint32_t i = 0; i < length; i++) {
		p[i] = malloc(sizeof(IntNode));
		p[i]->value = values[i];
	}
	linked->start = p[0];
	linked->start->previous = NULL;
	linked->end = p[length - 1];
	linked->end->next = NULL;

	if (length != 1) {
		linked->start->next = p[1];
		linked->end->previous = p[length - 2];
	}
	
	for (uint32_t i = 1; i < length - 1; i++) {
		p[i]->previous = p[i-1];
		p[i]->next = p[i+1];
	}
	
	linked->center = p[0];
	UpdateCenterIdx(linked, 0);

	free(p);
	return linked;
}

void DestroyIntLinkedList(IntLinkedList* linkedList) {
	if (IsEmpty(linkedList)) {
		free(linkedList);
		return;
	}
	IntNode* current = linkedList->end;

	while (current->previous != NULL) {
		current = current->previous;
		free(current->next);		
	}
	free(current);
	free(linkedList);
}

IntNode* GetIntNode(IntLinkedList* linkedList, uint32_t idx) {
	uint32_t centerIdx = CenterIdx(linkedList);

	uint32_t distanceStart = idx;
	uint32_t distanceEnd = linkedList->size - 1 - idx;
	uint32_t distanceCenter = centerIdx - idx;

	if (distanceStart < distanceEnd && distanceStart < distanceCenter)
		return IterNode(linkedList->start, distanceStart, 1);
	if (distanceEnd < distanceStart && distanceEnd < distanceCenter)
		return IterNode(linkedList->end, distanceEnd, 0);
	if (distanceCenter >= 0)
		return IterNode(linkedList->center, distanceCenter, 0);
	return IterNode(linkedList->center, distanceCenter * -1, 1);
}

int64_t RemoveIntNodeAt(IntLinkedList* linked, uint32_t idx) {
	if (!idx)
		return PopIntNode(linked);
	if (idx == linked->size - 1)
		return RemoveTail(linked);

	IntNode* node = GetIntNode(linked, idx);

	node->previous->next = node->next;
	node->next->previous = node->previous;

	uint32_t centerIdx = CenterIdx(linked);
	linked->size--;
	UpdateCenterIdx(linked, centerIdx);
	int64_t value = node->value;
	free(node);
	return value;
}

void PutIntNodeAt(IntLinkedList* linked, int64_t value, uint32_t idx) {
	if (!idx) {
		PushIntNode(linked, value);
		return;
	}
	if (idx == linked->size-1) {
		AppendIntNode(linked, value);
		return;
	}
	IntNode* node = GetIntNode(linked, idx);
	IntNode* newNode = malloc(sizeof(IntNode));

	node->previous->next = newNode;
	newNode->previous = node->previous;

	node->previous = newNode;

	newNode->value = value;
	newNode->next = node;


	uint32_t centerIdx = CenterIdx(linked);
	linked->size++;
	UpdateCenterIdx(linked, centerIdx);
}
// removes at first position
int64_t PopIntNode(IntLinkedList* linkedList) {
	uint32_t oldCenterIdx = CenterIdx(linkedList);
	IntNode* pStart = linkedList->start;
	IntNode* node = pStart;

	linkedList->start = node->next;
	if (linkedList->start != NULL)
		linkedList->start->previous = NULL;
	linkedList->size--;
	UpdateCenterIdx(linkedList, oldCenterIdx);
	if (IsEmpty(linkedList)) {
		linkedList->end = NULL;
	}

	int64_t value = node->value;
	free(node);
	return value;
}
// inserts at first position
void PushIntNode(IntLinkedList* linkedList, int64_t value) {
	uint32_t oldCenterIdx = CenterIdx(linkedList);
	
	IntNode* node = malloc(sizeof(IntNode));
	node->previous = NULL;
	node->value = value;
	node->next = linkedList->start;
	
	if (linkedList->start != NULL)
		linkedList->start->previous = node;	
	linkedList->start = node;
	linkedList->size++;

	UpdateCenterIdx(linkedList, oldCenterIdx);

	if (linkedList->end == NULL)
		linkedList->end = node;
}

void AppendIntNode(IntLinkedList* linkedList, int64_t value) {
	uint32_t oldCenterIdx = CenterIdx(linkedList);	
	IntNode* node = malloc(sizeof(IntNode));
	node->previous = linkedList->end;
	node->value = value;
	node->next = NULL;

	if (linkedList->end != NULL)
		linkedList->end->next = node;
	linkedList->end = node;
	linkedList->size++;

	UpdateCenterIdx(linkedList, oldCenterIdx);

	if (linkedList->start == NULL)
		linkedList->start = node;
}

int64_t RemoveTail(IntLinkedList* linkedList) {
	uint32_t oldCenterIdx = CenterIdx(linkedList);

	IntNode* tail = linkedList->end;
	IntNode* pSecondLast = linkedList->end->previous;

	if (pSecondLast != NULL)
		pSecondLast->next = NULL;

	linkedList->end = linkedList->end->previous;
	linkedList->size--;
	UpdateCenterIdx(linkedList, oldCenterIdx);
	if (IsEmpty(linkedList)) {
		linkedList->start = NULL;
		linkedList->center = NULL;
	}

	int64_t value = tail->value;
	free(tail);
	return value;
}