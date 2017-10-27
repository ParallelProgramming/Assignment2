/*
 ============================================================================
 Name        : ParallelMergeUtils.c
 Author      :
 Version     :
 Copyright   :
 Description :
 ============================================================================
 */

#include "ParallelMergeUtils.h"
#include "ParallelMerge.h"

void generate_arrays() {
	srand(time(NULL));

	int i = 0;
	firstArray[i] = my_rand();
	for (i = 1; i < lenFirst; i++) {
		firstArray[i] = firstArray[i - 1] + my_rand();
	}
	i = 0;
	secondArray[i] = my_rand();
	for (i = 1; i < lenSecond; i++) {
		secondArray[i] = secondArray[i - 1] + my_rand();
	}
}

// limit the range
int my_rand() {
	return rand() % 10;
}


void p_printf(const char* format, ...){
	t = time(NULL);
	printf("%s Process %d / %d: ", strtok(ctime(&t), "\n"), rank, p_num);

	va_list arg;
	va_start(arg, format);
	vfprintf (stdout, format, arg);
	va_end(arg);
	printf("\n");
}

void p_fprintf(const char* format, ...){
	FILE *f = fopen("pm.log", "a");
	if (f == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}

	t = time(NULL);
	fprintf(f, "%s Process %d / %d: ", strtok(ctime(&t), "\n"), rank, p_num);

	va_list arg;
	va_start(arg, format);
	vfprintf (f, format, arg);
	va_end(arg);
	fprintf(f, "\n");

	fclose(f);
}

char* int_array_to_string(int* arr, int arr_len){
	char *str = malloc(sizeof(char)*arr_len);
	int i=0;
	int index = 0;
	for (i=0; i<arr_len; i++)
	   index += sprintf(&str[index], "%d,", arr[i]);
	return str;
}

// print and check its sorted.
void print_merged_array(int** c, int* c_len, int tasks){
	int counter = 0;
	int sorted = 0;
	int n = 0;
	int i, j;
	for (i = 0; i < tasks; i++) {
		counter += c_len[i];
		int *portion = c[i];
		for (j = 0; j < c_len[i]; j++) {
			if (n > portion[j]) {
				printf("Error.");
				sorted = 1;
			}
			n = portion[j];
			printf("%d, ", n);
		}
	}

	if (sorted != 0 || counter != (lenFirst + lenSecond)) {
		printf("Merge went wrong. Counter: %d, expected %d", counter,
				lenFirst + lenSecond);

	}
}
