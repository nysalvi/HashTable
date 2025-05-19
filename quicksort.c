#include "quicksort.h"
#include <math.h>
#include <string.h>

void QuickSort(QuickSortArgs args) {	
	int start = args.start;
	int end = args.end;
	void** data = args.data;
	int (*ToInt) (void*) = args.ToInt;

	int leftIdx = start;
	int rightIdx = end;
	int pivotIdx = ceil((end + start) / 2.0);

	while (leftIdx < rightIdx) {		
		int leftValue = ToInt(data[leftIdx]);
		int rightValue = ToInt(data[rightIdx]);
		int pivotValue = ToInt(data[pivotIdx]);

		while (leftValue < pivotValue) {
			leftIdx++;
			leftValue = ToInt(data[leftIdx]);
		}
		while (rightValue > pivotValue) {
			rightIdx--;
			rightValue = ToInt(data[rightIdx]);
		}
				
		if (leftIdx <= rightIdx) {			
			void* temp = data[leftIdx];
			data[leftIdx] = data[rightIdx];
			data[rightIdx] = temp;
			leftIdx++;
			rightIdx--;
		}
	}
	if ((rightIdx - start) > 0) {
		QuickSortArgs rightArgs;
		rightArgs.data = data;
		rightArgs.start = start;
		rightArgs.end = rightIdx;
		rightArgs.ToInt = ToInt;
		QuickSort(rightArgs);
	}

	if ((end - leftIdx) > 0) {
		QuickSortArgs leftArgs;
		leftArgs.data = data;
		leftArgs.start = leftIdx;
		leftArgs.end = end;
		leftArgs.ToInt = ToInt;
		QuickSort(leftArgs);
	}
	
}