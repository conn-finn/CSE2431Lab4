/* 
Connor Finneran
compile with: gcc -g lab4.c -o lab4
run with: lab4
 */

#include <stdio.h>
#include <time.h>
#include <pthread.h>

#define DIM1 1200
#define DIM2 1000
#define DIM3 500

static int a[DIM1][DIM2];
static int b[DIM2][DIM3];
static int c[DIM1][DIM3]; /* where c is the result of a * b */

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

int main(int argc, char*argv[]) {

	int i, j, k;
	clock_t start, end;
	double timeTaken;

	initializeA(a);
	initializeB(b);
	start = clock();
	multiply(a, b, c);
	end = clock();
	timeTaken = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("It took %.3f seconds to multiply the two matrices.\n", timeTaken);

}
