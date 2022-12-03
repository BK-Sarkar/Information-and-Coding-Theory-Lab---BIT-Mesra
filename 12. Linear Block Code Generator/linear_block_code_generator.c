/**
 * @file linear_block_code_generator.c
 * @brief
 * Compile Command:
 *  gcc -lm linear_block_code_generator.c -o linear_block_code_generator
 * Run Command:
 *  ./linear_block_code_generator
 *
 * @details
 * Files Required:
 *  generator_matrix.txt - This file contains the generator matrix for the LBC.
 *
 * Files Generated:
 *  output.txt - This file contains the encodings generated.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int **generator_matrix, k, n, m, **uncoded_message, **encoded_message;

void read_generator_matrix()
{
    FILE *fp = fopen("generator_matrix.txt", "r");
    if (fp == NULL)
    {
        printf("Unable to read file \"generator_matrix.txt\"\n");
        exit(1);
    }
    fscanf(fp, "%d %d", &k, &n);
    generator_matrix = (int **)malloc(k * sizeof(int *));
    for (int i = 0; i < k; i++)
    {
        generator_matrix[i] = (int *)malloc(n * sizeof(int));
    }
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fscanf(fp, "%d", &generator_matrix[i][j]);
        }
    }
    fclose(fp);
}

void generate_possible_uncoded_messages()
{
    m = (1 << k);
    uncoded_message = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++)
    {
        uncoded_message[i] = (int *)malloc(k * sizeof(int));
    }
    for (int i = 0; i < m; i++)
    {
        int temp = i;
        for (int j = k - 1; j >= 0; j--)
        {
            uncoded_message[i][j] = temp % 2;
            temp /= 2;
        }
    }
}

void encode_message()
{
    encoded_message = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++)
    {
        encoded_message[i] = (int *)malloc(n * sizeof(int));
    }
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int sum = 0;
            for (int itr = 0; itr < k; itr++)
            {
                sum = (sum + uncoded_message[i][itr] * generator_matrix[itr][j]) % 2;
            }
            encoded_message[i][j] = sum;
        }
    }
}

void write_encoded_message()
{
    FILE *fp = fopen("output.txt", "w");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < k; j++)
        {
            fprintf(fp, "%d", uncoded_message[i][j]);
        }
        fprintf(fp, " -> ");
        for (int j = 0; j < n; j++)
        {
            fprintf(fp, "%d", encoded_message[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("Output Successfully Saved\n");
}

void free_memory()
{
    for (int i = 0; i < k; i++)
    {
        free(generator_matrix[i]);
    }
    free(generator_matrix);
    for (int i = 0; i < m; i++)
    {
        free(encoded_message[i]);
        free(uncoded_message[i]);
    }
    free(encoded_message);
    free(uncoded_message);
}

int exec()
{
    read_generator_matrix();
    generate_possible_uncoded_messages();
    encode_message();
    write_encoded_message();
    free_memory();
    return 0;
}

int main()
{
    return exec();
}
