/*
  Simple Pthread Program to find the sum of a vector.
  Uses mutex locks to update the global sum.
  Author: Purushotham Bangalore
  Date: Jan 25, 2009

  To Compile: gcc -O -Wall pthread_psum.c -lpthread
  To Run: ./a.out 1000 4
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct sumstuff {
    double *a;
    double sum;
    int N;
    int size;
} struct_sum;

// double *a = NULL, sum = 0.0;
// int N, size;

void *compute(void *arg)
{
    int myStart, myEnd, myN, i;
    long tid = (long)arg;

    // determine start and end of computation for the current thread
    myN = struct_sum.N / struct_sum.size;
    myStart = tid * myN;
    myEnd = myStart + myN;
    if (tid == (struct_sum.size - 1)) myEnd = struct_sum.N;

    // compute partial sum
    double mysum = 0.0;
    for (i = myStart; i < myEnd; i++)
        mysum += struct_sum.a[i];

    // grab the lock, update global sum, and release lock
    pthread_mutex_lock(&mutex);
    struct_sum.sum += mysum;
    pthread_mutex_unlock(&mutex);

    return (NULL);
}

int main(int argc, char **argv)
{
    struct_sum.a = NULL;
    struct_sum.sum = 0.0;
    long i;
    pthread_t *tid;

    if (argc != 3) {
        printf("Usage: %s <# of elements> <# of threads>\n", argv[0]);
        exit(-1);
    }

    struct_sum.N = atoi(argv[1]);     // no. of elements
    struct_sum.size = atoi(argv[2]);  // no. of threads

    // allocate vector and initialize
    tid = (pthread_t *)malloc(sizeof(pthread_t) * struct_sum.size);
    struct_sum.a = (double *)malloc(sizeof(double) * struct_sum.N);
    for (i = 0; i < struct_sum.N; i++)
        struct_sum.a[i] = (double)(i + 1);

    // create threads
    for (i = 0; i < struct_sum.size; i++)
        pthread_create(&tid[i], NULL, compute, (void *)i);

    // wait for them to complete
    for (i = 0; i < struct_sum.size; i++)
        pthread_join(tid[i], NULL);

    printf("The total is %g, it should be equal to %g\n",
           struct_sum.sum, ((double)struct_sum.N * (struct_sum.N + 1)) / 2);

    return 0;
}
