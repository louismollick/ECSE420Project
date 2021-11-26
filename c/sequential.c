// Reference: https://www.eecs.umich.edu/courses/eecs570/discussions/w20/pthreads.html
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int matrix_size;
long **matrix_a;
long **matrix_b;
long **matrix_c;

uint64_t get_current_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t result = tv.tv_sec * (uint64_t)1000000 + tv.tv_usec;
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

void multiply()
{
    for (int i = 0; i < matrix_size; i++)
    {
        for (int j = 0; j < matrix_size; j++)
        {
            for (int k = 0; k < matrix_size; k++)
            {
                matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
            }
        }
    }
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
    if (argc != 2)
    {
        printf("usage: ./sequential <matrix_size>\n");
        for (size_t i = 0; i < argc; i++)
        {
            printf("received: %s\n", argv[i]);
        }

        exit(1);
    }
    matrix_size = atoi(argv[1]);

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

    // Generate matrix
    init();

    // printf("MATRIX A:\n");
    // printMatrix(matrix_a);
    // printf("MATRIX B: \n");
    // printMatrix(matrix_b);

    // Init timer
    uint64_t start = get_current_time();
    // Start computation
    multiply();
    uint64_t end = get_current_time();

    printHeadMatrix(matrix_c);

    printf("sequential.c (%d x %d matrix) - Runtime: %llu\n", matrix_size, matrix_size, end - start);

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