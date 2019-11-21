/* 
Connor Finneran (finneran.61@osu.edu)
Ian Blake (blake.404@osu.edu)


compile with: gcc -g -lpthread lab4.c -o lab4
run with: lab4
 */

#include <stdio.h>
#include <time.h>
#include <pthread.h>

/* Define constants and global variables */
#define DIM1 1200
#define DIM2 1000
#define DIM3 500
#define THREAD_NUM 6

static int a[DIM1][DIM2];
static int b[DIM2][DIM3];
static int c[DIM1][DIM3]; /* where c and d will be the result of a * b */
static int d[DIM1][DIM3];

/* struct keeps track of how much of the matrix an individual thread should multiply */
struct location {
	int start;
	int end;
};

/* basic multiply function based on code given in lab4_fall19.pdf done on a single thread */
void multiply(int a[][DIM2], int b[][DIM3], int c[][DIM3]) {
	int i, j, k;
	int n = DIM1;
	int m = DIM2;
	int p = DIM3;
	for (i=0; i < n; i++) {
 		for (j=0; j < p; j++) {
	 		c[i][j]=0;
	 		for (k=0; k < m; k++) {
	 			c[i][j] += a[i][k]*b[k][j];
			}
	 	}
	}
}

/* multiplies a specific section of the matrices based on the args passed in, which contains a start and endpoint */
void* multiplyWithThreads(void *args) {
	struct location *data = (struct location *)args;
	int i, j, k;
	int n = data->end;
	int m = DIM2;
	int p = DIM3;
	//printf("start: %d, end: %d\n", data->start, data->end);
	
	for (i=data->start; i < n; i++) {
 		for (j=0; j < p; j++) {
	 		c[i][j]=0;
	 		for (k=0; k < m; k++) {
	 			c[i][j] += a[i][k]*b[k][j];
			}
	 	}
	}
}
/* functions below initialize matrix a and b based on code given in lab4_fall19.pdf */
void initializeA(int a[][DIM2]) {
	int i, j;

	for (i = 0; i < DIM1; i++) {
		for ( j = 0; j < DIM2; j++) {
			a[i][j] = i + j;
		}
	} 
}

void initializeB(int b[][DIM3]) {
	int i, j;

	for (i = 0; i < DIM2; i++) {
		for ( j = 0; j < DIM3; j++) {
			b[i][j] = j;
		}
	} 
}
/* test function compares matrix calculated with a single thread, in the multiply function with the matrix calculated using multiple threads in the multiplyWithThreads function, checking that all elements are the same */
void test() {
	int different = 0;
	int i, j;
	for (i = 0; i < DIM1; i++) {
		for (j = 0; j < DIM3; j++) {
			/* if statement prints out 30 elements from both matrices so we can see if they are equal in both *//*
			if (i % 200 == 0 && j % 100 == 0) {
				printf("C: %d D: %d\n", c[i][j], d[i][j]); 
			}
			*/		
			if (c[i][j] != d[i][j]) {
				different = different + 1;	
			}
		}
	}
	if (different == 0) {
		printf("No error\n");
	} else {
		printf("There were %d errors\n", different);
	}
}
/* given a number of threads to use, and a matrix of usable threads, the multiplyThreads function measures and prints out the amount of time it takes to multiply the a and b matrices using the number of threads given in thread_num, and calls test() to ensure the result matrix c is correct */
void multiplyThreads(int thread_num, pthread_t threads[]) {
	struct location data[thread_num];
	int k;
	double timeTaken;
	struct timespec start, end;

	clock_gettime(CLOCK_MONOTONIC, &start);

	for (k=0; k < thread_num; k++) {
		data[k].start = ((DIM1 / thread_num) * k);
		data[k].end = (DIM1 / thread_num) * (k+1);
		//printf("%d    start: %d end: %d\n", k+1, data[k].start, data[k].end);
		pthread_create(&threads[k], NULL, multiplyWithThreads, &data[k]);
	}

	for (k=0; k < thread_num; k++) {
		pthread_join(threads[k], NULL);
	}
	
	clock_gettime(CLOCK_MONOTONIC, &end);
	timeTaken = (end.tv_sec - start.tv_sec);
	timeTaken += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("\nIt took %.3f seconds to multiply the two matrices with %d threads.\n", timeTaken, thread_num);
	
	test();

}
/* our main function calls initialize functions on our matrices and then measures the time it takes to multiply the two with a single thread, before repeatedly calling multiplyThreads with an additional thread each time, up until the max number of threads tested THREAD_NUM, before terminating */
int main(int argc, char*argv[]) {
	int i;
	double timeTaken;
	int different = 0;
	clock_t start, end;
	pthread_t threads[THREAD_NUM];

	initializeA(a);
	initializeB(b);

	start = clock();
	multiply(a,b,d); //for testing matrix equality
	end = clock();
	timeTaken = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("It took %.3f seconds to multiply the two matrices with 1 thread.\n", timeTaken);
	
	for (i = 2; i <= THREAD_NUM; i++) {
		multiplyThreads(i, threads);
	}
	
	return 0;
}
