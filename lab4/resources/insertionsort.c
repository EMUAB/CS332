#include <stdio.h>
#include <stdlib.h>
#include "gettime.h"

void insertionSort(float *array, int n) {	
  int i, j;
  float key;

  for(j=1; j<n; j++) {
    key = array[j];
    i = j-1;
    while(i>=0 && array[i]>key) {
      array[i+1] = array[i];
      i--;
    }
    array[i+1] = key;
  }
}

int main() {
  int n, i; 
  float *array;
  double starttime, endtime;

  printf("Enter number of elements: ");
  scanf("%d", &n);

  array = (float *) malloc(n * sizeof(float));
  if(array == NULL) {
    printf("Error! memory allocation failed!");
    exit(0);
  }

  printf("Enter the elements\n"); 
  for(i = 0; i < n; i++) {
      scanf("%f", &array[i]);
  }

  starttime = gettime();
  insertionSort(array, n);
  endtime = gettime();

  printf("Time taken to sort %d elements = %f seconds\n", n, endtime-starttime);
  printf("Sorted list in ascending order:\n");

  for(i=0; i<n; i++) 
    printf("%f\n",array[i]);

  free(array);

  return 0;
}

