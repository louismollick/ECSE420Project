// Reference: https://www.eecs.umich.edu/courses/eecs570/discussions/w20/pthreads.html
#include <stdio.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

int matrix_size;
long **matrix_a;
long **matrix_b;
long **matrix_c;

typedef struct thread_args
{
    int start;
    int end;
} thread_args;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

uint64_t get_current_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t result = tv.tv_sec*(uint64_t)1000000 + tv.tv_usec;
    return result;
} 

void init()
{
    for (int i = 0; i < matrix_size; i++)
    {
        for (int j = 0; j < matrix_size; j++)
        {
            matrix_a[i][j] = i + j;
            matrix_b[i][j] = i - j;
            matrix_c[i][j] = 0;
        }
    }
}

void *multiply(void *args)
{
    struct thread_args *range = (struct thread_args *)args;
    for (int i = 0; i < matrix_size; i++)
    {
        for (int j = 0; j < matrix_size; j++)
        {
            long thread_private_tmp = 0;
            for (int k = range->start; k < range->end; k++)
            {
                thread_private_tmp += matrix_a[i][k] * matrix_b[k][j];
            }
            pthread_mutex_lock(&lock);
            matrix_c[i][j] += thread_private_tmp;
            pthread_mutex_unlock(&lock);
        }
    }
    return 0;
}

void printMatrix(long **matrix)
{
    for (int i = 0; i < matrix_size; i++)
    {
        for (int j = 0; j < matrix_size; j++)
            printf("%ld ", matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

void printHeadMatrix(long **matrix)
{
    int num_vals = matrix_size;
    if (num_vals >= 10) num_vals = 10;

    for (int i = 0; i < num_vals; i++)
    {
        printf("%ld ", matrix[0][i]);
    }
    printf("...\n");
}

int main(int argc, char *argv[])
{
    // Get inputs
    if (argc != 3)
    {
        printf("usage: ./pthreads <matrix_size> <num_threads>\n");
        exit(1);
    }
    matrix_size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    // Allocate matrix memory
    matrix_a = (long **)malloc(sizeof(long *) * matrix_size);
    matrix_b = (long **)malloc(sizeof(long *) * matrix_size);
    matrix_c = (long **)malloc(sizeof(long *) * matrix_size);
    for (int i = 0; i < matrix_size; i++)
    {
        matrix_a[i] = (long *)malloc(sizeof(long) * matrix_size);
        matrix_b[i] = (long *)malloc(sizeof(long) * matrix_size);
        matrix_c[i] = (long *)malloc(sizeof(long) * matrix_size);
    }

    // Prepare data for threads
    pthread_t *child_threads = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
    thread_args *work_ranges = (thread_args *)malloc(sizeof(*work_ranges) * num_threads);
    int current_start, range;
    current_start = 0;
    range = matrix_size / num_threads;
    for (int i = 0; i < num_threads; i++)
    {
        work_ranges[i].start = current_start;
        work_ranges[i].end = current_start + range;
        current_start += range;
    }
    work_ranges[num_threads - 1].end = matrix_size;

    // Generate matrix
    init();

    // printf("MATRIX A:\n");
    // printMatrix(matrix_a);
    // printf("MATRIX B: \n");
    // printMatrix(matrix_b);

    // Init timer
    uint64_t start = get_current_time();
    // Start computation
    for (int i = 0; i < num_threads; i++)
    {
        pthread_create(&child_threads[i], NULL, multiply, &work_ranges[i]);
    }
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(child_threads[i], NULL);
    }
    uint64_t end = get_current_time();

    printHeadMatrix(matrix_c);

    printf("pthreads.c (%d x %d matrix, %d threads) - Runtime: %llu\n", matrix_size, matrix_size, num_threads, end - start);
    
    free(child_threads);
    free(work_ranges);
    for (int i = 0; i < matrix_size; i++)
    {
        free(matrix_a[i]);
        free(matrix_b[i]);
        free(matrix_c[i]);
    }
    free(matrix_a);
    free(matrix_b);
    free(matrix_c);

    // printf("RESULT:\n");
    // printMatrix(matrix_c);

    return 0;
}