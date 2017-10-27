/*
 ============================================================================
 Name        : ParallelMerge.h
 Author      :
 Version     :
 Copyright   :
 Description :
 ============================================================================
 */

#ifndef PARALLELMERGE_H_
#define PARALLELMERGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <math.h>

void assign_task(int dest, int task_id, int task_size, int tasks);
void serial_mode();
void slave_mode();
void master_mode(int tasks, int task_size);
void setup(int argc, char *argv[]);

int			rank;		/* rank of process */
int			p_num;		/* number of processes */
int			tag;		/* tag for messages */
MPI_Status	status ;		/* return status for receive */
double 		start_time, end_time;

int lenFirst;
int lenSecond;

int* firstArray;
int* secondArray;
int msg[3];

#endif /* PARALLELMERGE_H_ */
