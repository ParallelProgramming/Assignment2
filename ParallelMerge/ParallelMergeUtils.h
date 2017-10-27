/*
 ============================================================================
 Name        : ParallelMergeUtils.h
 Author      :
 Version     :
 Copyright   :
 Description :
 ============================================================================
 */

#ifndef PARALLELMERGEUTILS_H_
#define PARALLELMERGEUTILS_H_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

time_t 		t;

void generate_arrays();
int my_rand();
void p_printf(const char* format, ...);
void p_fprintf(const char* format, ...);
char* int_array_to_string(int* arr, int arr_len);
void print_merged_array(int** c, int* c_len, int tasks);

#endif /* PARALLELMERGEUTILS_H_ */
