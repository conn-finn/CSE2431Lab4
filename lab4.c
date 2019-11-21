/* 
Connor Finneran
compile with: gcc -g -pthread lab4.c -o lab4
run with: lab4
 */

#include <stdio.h>
#include <time.h>
#include <pthread.h>

#define DIM1 1200
#define DIM2 1000
#define DIM3 500
#define THREAD_NUM 6

static int a[DIM1][DIM2];
static int b[DIM2][DIM3];
static int c[DIM1][DIM3]; /* where c is the result of a * b */
static int d[DIM1][DIM3];

struct location {
	int start;
	int end;
};

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

void test() {
	int different = 0;
	int i, j;
	for (i = 0; i < DIM1; i++) {
		for (j = 0; j < DIM3; j++) {
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

void multiplyThreads(int thread_num, pthread_t threads[]) {
	struct location data[thread_num];
	int k;
	clock_t start, end;
	double timeTaken;

	start = clock();

	for (k=0; k < thread_num; k++) {
		data[k].start = ((DIM1 / thread_num) * k);
		data[k].end = (DIM1 / thread_num) * (k+1);
		//printf("%d    start: %d end: %d\n", k+1, data[k].start, data[k].end);
		pthread_create(&threads[k], NULL, multiplyWithThreads, &data[k]);
	}

	for (k=0; k < thread_num; k++) {
		pthread_join(threads[k], NULL);
	}
	
	end = clock();
	timeTaken = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("It took %.3f seconds to multiply the two matrices with %d threads.\n", timeTaken, thread_num);
	
	test();

}

int main(int argc, char*argv[]) {
	int i;
	double timeTaken;
	int different = 0;
	clock_t start, end;
	pthread_t threads[THREAD_NUM];
	
	//data = (struct location **)malloc(thread_num * sizeof(struct location));

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
