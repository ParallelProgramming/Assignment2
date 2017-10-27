/*
 ============================================================================
 Name        : ParallelMerger.c
 Author      :
 Version     :
 Copyright   :
 Description :
 ============================================================================
 */
#include "ParallelMerge.h"
#include "ParallelMerger.h"

int* merge_secion(int* a, int *b, int a_from, int a_to, int* merged_len) {
	int b_from;
	int b_to;
	if (a_from == 0) {
		b_from = 0;
	} else {
		// larger than the previous largest
		b_from = binary_search(b, 0, lenSecond - 1, a[a_from - 1]);
		if (secondArray[b_from] < firstArray[a_from - 1]) {
			b_from += 1;
		}
	}
	if (a_to == lenFirst - 1) {
		b_to = lenSecond - 1;
	} else {
		b_to = binary_search(b, b_from, lenSecond - 1, a[a_to]);
		if (secondArray[b_to] > a[a_to]) {
			b_to -= 1;
		}
	}

	int a_length = a_to - a_from + 1;
	int b_length = b_to - b_from + 1;
	if (b_from == lenSecond) {
		b_length = 0;
		b_from = -1;
	}

	int size = a_length + b_length;
	*merged_len = size;
	return merge(a, b, a_from, a_to, b_from, b_to);
}

int* merge(int* a, int *b, int a_from, int a_to, int b_from, int b_to) {
	int a_length = a_to - a_from + 1;
	int b_length = b_to - b_from + 1;
	if (b_from == -1) {
		b_length = 0;
	}
	int size = a_length + b_length;
	int* c = malloc(sizeof(int) * size);
	int i = a_from;
	int j = b_from;
	int n = 0;

	if (b_length > 0) {
		while (i <= a_to && j <= b_to) {
			if (a[i] < b[j]) {
				c[n] = a[i];
				i++;
			} else {
				c[n] = b[j];
				j++;
			}
			n++;
		}

		while (j <= b_to && b_length > 0) {
			c[n] = b[j];
			j++;
			n++;
		}
	}

	while (i <= a_to) {
		c[n] = a[i];
		i++;
		n++;
	}

	//p_printf("After merge (len %d) %s\n", size,  int_array_to_string(c, size));
//	if(c[0] == 0){
//		p_printf("Merged with 0, b from %d to %d. a from %d to %d.\n", b_from, b_to, a_from, a_to);
//	}
	return c;
}

int binary_search(int* arr, int from, int to, int val) {
	//p_printf("searching from %d between %d and %d\n,",val, from, to);
	if (from >= to) {
		return from;
	}
	int mid = from + (to - from) / 2;
	if (arr[mid] > val) {
		return binary_search(arr, from, mid, val);
	} else {
		return binary_search(arr, mid + 1, to, val);
	}
}
