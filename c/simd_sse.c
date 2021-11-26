// References: https://stackoverflow.com/a/40326720
//             https://robaboukhalil.medium.com/webassembly-and-simd-7a7daa4f2ecd
//             https://emscripten.org/docs/porting/simd.html#compiling-simd-code-targeting-x86-sse-instruction-set
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <immintrin.h>

int matrix_size;
int **matrix_a;
int **matrix_b;
int **matrix_c;

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
    for (int i = 0; i < matrix_size; ++i)
    {
        for (int j = 0; j < matrix_size; j += 4)
        { // vectorize over this loop
            __m128i vR = _mm_setzero_si128();
            for (int k = 0; k < matrix_size; k++)
            { // not this loop
                //result[i][j] += mat1[i][k] * mat2[k][j];
                __m128i vA = _mm_set1_epi32(matrix_a[i][k]);              // load+broadcast is much cheaper than MOVD + 3 inserts (or especially 4x insert, which your new code is doing)
                __m128i vB = _mm_loadu_si128((__m128i *)&matrix_b[k][j]); // mat2[k][j+0..3]
                vR = _mm_add_epi32(vR, _mm_mullo_epi32(vA, vB));
            }
        _mm_storeu_si128((__m128i*)&matrix_c[i][j], vR);
        }
    }
}

void printMatrix(int **matrix)
{
    for (int i = 0; i < matrix_size; i++)
    {
        for (int j = 0; j < matrix_size; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

void printHeadMatrix(int **matrix)
{
    int num_vals = matrix_size;
    if (num_vals >= 10)
        num_vals = 10;

    for (int i = 0; i < num_vals; i++)
    {
        printf("%d ", matrix[0][i]);
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
    matrix_a = (int **)malloc(sizeof(int *) * matrix_size);
    matrix_b = (int **)malloc(sizeof(int *) * matrix_size);
    matrix_c = (int **)malloc(sizeof(int *) * matrix_size);
    for (int i = 0; i < matrix_size; i++)
    {
        matrix_a[i] = (int *)malloc(sizeof(int) * matrix_size);
        matrix_b[i] = (int *)malloc(sizeof(int) * matrix_size);
        matrix_c[i] = (int *)malloc(sizeof(int) * matrix_size);
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

    // printHeadMatrix(matrix_c);

    printf("simd_sse.c (%d x %d matrix) - Runtime: %llu\n", matrix_size, matrix_size, end - start);

    // printf("RESULT:\n");
    // printMatrix(matrix_c);

    for (int i = 0; i < matrix_size; i++)
    {
        free(matrix_a[i]);
        free(matrix_b[i]);
        free(matrix_c[i]);
    }
    free(matrix_a);
    free(matrix_b);
    free(matrix_c);

    return 0;
}