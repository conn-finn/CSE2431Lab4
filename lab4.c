/* 
Connor Finneran
compile with: gcc -g lab4.c -o lab4
run with: lab4
 */

#include <stdio.h>
#include <time.h>
#include <pthread.h>

void multiply(int a[][1000], int b[][500], int c[][500]) {
	int i, j, k;
	int n = 1200;
	int m = 1000;
	int p = 500;	
	for (i=0; i < n; i++) {
 		for (j=0; j < p; j++) {
	 		c[i][j]=0;
	 		for (k=0; k < m; k++) {
	 			c[i][j] += a[i][k]*b[k][j];
			}
	 	}
	}
}

void initializeA(int a[][1000]) {
	int i, j;

	for (i = 0; i < 1200; i++) {
		for ( j = 0; j < 1000; j++) {
			a[i][j] = i + j;
		}
	} 
}

void initializeB(int b[][500]) {
	int i, j;

	for (i = 0; i < 1000; i++) {
		for ( j = 0; j < 500; j++) {
			b[i][j] = j;
		}
	} 
}

int main(int argc, char*argv[]) {
	static int a[1200][1000];
	static int b[1000][500];
	static int c[1200][500]; /* where c is the result of a * b */
	int n = 1200;
	int m = 1000;
	int p = 500;
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
