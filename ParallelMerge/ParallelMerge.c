/*
 ============================================================================
 Name        : ParallelMerge.c
 Author      : 
 Version     :
 Copyright   :
 Description :
 ============================================================================
 */

#include "ParallelMerge.h"
#include "ParallelMerger.h"
#include "ParallelMergeUtils.h"

const int MASTER_RANK 	= 0;
const int DEFAULT_LENGTH = 67108864; //2^26

int** c;
int* c_len;

int main(int argc, char *argv[]) {
	int tasks = 1;

	/* start up MPI */
	MPI_Init(&argc, &argv);

	/* find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	/* find out number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &p_num);

	setup(argc, argv);

	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == MASTER_RANK) {
		start_time = MPI_Wtime();
		p_fprintf("Starting");
		generate_arrays(argc, argv);
	}

	if (p_num == 1) {
		serial_mode();
	} else {
		MPI_Bcast(firstArray, lenFirst, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);
		MPI_Bcast(secondArray, lenSecond, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);

		if (rank == MASTER_RANK) { // master
			int task_size = log2(lenFirst);
			tasks = ceil((double) lenFirst / (double) task_size);
			p_fprintf("Running %d tasks, task size: %d", tasks, task_size);
			master_mode(tasks, task_size);
		} else { // slaves
			slave_mode();
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == MASTER_RANK) {
		end_time = MPI_Wtime();
		p_fprintf("End");
		p_fprintf("Took %lf seconds.", end_time - start_time);
	}

	/* shut down MPI */
	MPI_Finalize();

	if (rank == MASTER_RANK) {
		print_merged_array(c, c_len, tasks);
	}
	return 0;
}

// allocate arrays
void setup(int argc, char *argv[]) {
	if (argc == 3) {
		lenFirst = atoi(argv[1]);
		lenSecond = atoi(argv[2]);
	} else if (argc == 2) {
		lenFirst = atoi(argv[1]);
		lenSecond = atoi(argv[1]);
	} else {
		lenFirst = DEFAULT_LENGTH;
		lenSecond = DEFAULT_LENGTH;
	}

	firstArray = malloc(sizeof(int*) * lenFirst);
	secondArray = malloc(sizeof(int*) * lenSecond);
}

void serial_mode() {
	c = malloc(sizeof(int*) * 1);
	c_len = malloc(sizeof(int) * 1);
	c_len[0] = lenFirst + lenSecond;
	c[0] = merge(firstArray, secondArray, 0, lenFirst - 1, 0, lenSecond - 1);
}

void slave_mode() {
	// ask for task
	MPI_Send(&msg, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	MPI_Recv(&msg, 3, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

	// while there are tasks
	while (msg[0] != -1) {
		int merged_size;
		int* merged = merge_secion(firstArray, secondArray, msg[1], msg[2],
				&merged_size);
		msg[1] = merged_size;
		MPI_Send(&msg, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Send(merged, merged_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Recv(&msg, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		free(merged);
	}
}

void master_mode(int tasks, int task_size) {
	int task_merged;
	int *merged;
	int finished_tasks = 0;
	int task_id = 0;

	c = malloc(sizeof(int*) * tasks);
	c_len = malloc(sizeof(int) * tasks);

	while (finished_tasks < tasks) {
		MPI_Recv(&msg, 3, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
				&status);
		//receive array
		if (status.MPI_TAG == 0) {
			task_merged = msg[0];
			merged = malloc(sizeof(int) * msg[1]);
			MPI_Recv(merged, msg[1], MPI_INT, status.MPI_SOURCE, tag,
					MPI_COMM_WORLD, &status);
			c[task_merged] = merged;
			c_len[task_merged] = msg[1];
			p_fprintf("Got task %d from %d. Merged section (len %d) %s\n",
					task_merged, status.MPI_SOURCE,  msg[1],  int_array_to_string(merged, msg[1]));
			finished_tasks++;

			assign_task(status.MPI_SOURCE, task_id, task_size, tasks);

		}
		// ask for new job
		else if (status.MPI_TAG == 1) {
			p_fprintf("Asked for a new job by %d\n", status.MPI_SOURCE);
			assign_task(status.MPI_SOURCE, task_id, task_size, tasks);
		}
		task_id++;
	}
}

void assign_task(int dest, int task_id, int task_size, int tasks) {
	int msg[3];
	if (task_id >= tasks) {
		task_id = -1;
	}
	msg[0] = task_id;
	int start = task_id * task_size; //start
	msg[1] = start;
	int end = msg[1] + task_size - 1;
	if (end >= lenFirst) {
		end = lenFirst - 1;
	}
	msg[2] = end;

	MPI_Send(&msg, 3, MPI_INT, dest, 0, MPI_COMM_WORLD);
}
