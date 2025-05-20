#include "quicksort.h"
#include <math.h>
#include <string.h>
#include <process.h>
#include <Windows.h>


unsigned __stdcall QuickSort(void* pArgs) {
	QuickSortArgs* args = pArgs;
	int start = args->start;
	int end = args->end;
	void** data = args->data;
	int (*ToInt) (void*) = args->ToInt;

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

	HANDLE threads[2];
	int threadCount = 0;
	if ((rightIdx - start) > 0) {
		QuickSortArgs* rightArgs = malloc(sizeof(QuickSortArgs));
		*rightArgs = (QuickSortArgs){ data, start, rightIdx, ToInt };
		threads[threadCount++] = (HANDLE)_beginthreadex(NULL, 0, QuickSort, rightArgs, 0, NULL);
		//QuickSort(rightArgs);
		//free(rightArgs);
	}

	if ((end - leftIdx) > 0) {
		QuickSortArgs* leftArgs = malloc(sizeof(QuickSortArgs));
		*leftArgs = (QuickSortArgs){ data, leftIdx, end, ToInt };
		threads[threadCount++] = (HANDLE)_beginthreadex(NULL, 0, QuickSort, leftArgs, 0, NULL);
		//_beginthread(QuickSort, 0, leftArgs);		
		//QuickSort(leftArgs);
		//free(leftArgs);
	}

	if (threadCount > 0) {
		WaitForMultipleObjects(threadCount, threads, TRUE, INFINITE);
		for (int i = 0; i < threadCount; i++) {
			CloseHandle(threads[i]);
		}
	}
	free(args);

}