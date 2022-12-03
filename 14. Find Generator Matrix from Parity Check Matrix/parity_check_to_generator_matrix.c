/**
 * @file parity_check_to_generator_matrix.c
 * @brief
 * Compile Command:
 *  gcc -lm parity_check_to_generator_matrix.c -o parity_check_to_generator_matrix
 * Run Command:
 *  ./parity_check_to_generator_matrix
 * @details
 * Required Files:
 *  parity_check.txt - This file contains the parity check matrix.
 *                     It is assumed that the parity check matrix is in systematic form.
 * Generated Files:
 *  generator_matrix.txt - This file contains the generator matrix for the given parity check matrix.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int n_k, n, **parity_check_matrix, **generator_matrix, k;

void read_parity_check_matrix()
{
    FILE *fp = fopen("parity_check.txt", "r");
    if (fp == NULL)
    {
        printf("Cannot read file \"parity_check.txt\"\n");
        exit(1);
    }
    fscanf(fp, "%d %d", &n_k, &n);
    k = n - n_k;
    parity_check_matrix = (int **)malloc(n_k * sizeof(int *));
    for (int i = 0; i < n_k; i++)
    {
        parity_check_matrix[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
        {
            fscanf(fp, "%d", &parity_check_matrix[i][j]);
        }
    }
    fclose(fp);
}

void convert_to_generator()
{
    generator_matrix = (int **)malloc(k * sizeof(int));
    for (int i = 0; i < k; i++)
    {
        generator_matrix[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
        {
            generator_matrix[i][j] = 0;
        }
    }
    for (int i = 0; i < k; i++)
    {
        generator_matrix[i][i] = 1;
    }
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < (n - k); j++)
        {
            generator_matrix[i][k + j] = parity_check_matrix[j][i];
        }
    }
    FILE *fp = fopen("generator_matrix.txt", "w");
    fprintf(fp, "%d %d\n", k, n);
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fprintf(fp, "%d ", generator_matrix[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

void cleanup()
{
    for (int i = 0; i < n_k; i++)
    {
        free(parity_check_matrix[i]);
    }
    free(parity_check_matrix);
    for (int i = 0; i < k; i++)
    {
        free(generator_matrix[i]);
    }
    free(generator_matrix);
}

int exec()
{
    read_parity_check_matrix();
    convert_to_generator();
    cleanup();
    return 0;
}

int main()
{
    return exec();
}
