/*
 ============================================================================
 Name        : ParallelMerger.h
 Author      :
 Version     :
 Copyright   :
 Description :
 ============================================================================
 */

#ifndef PARALLELMERGER_H_
#define PARALLELMERGER_H_

#include <stdlib.h>

int* merge_secion(int* a, int *b, int a_from, int a_to, int* merged_len);
int* merge(int* a, int *b, int a_from, int a_to, int b_from, int b_to);
int binary_search(int* arr, int from, int to, int val);


#endif /* PARALLELMERGER_H_ */
