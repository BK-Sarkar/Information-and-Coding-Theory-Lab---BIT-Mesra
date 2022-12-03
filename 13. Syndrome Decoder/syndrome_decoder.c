/**
 * @file syndrome_decoder.c
 * @brief
 * Compile Command:
 *  gcc -lm syndrome_decoder.c -o syndrome_decoder
 * Run Command:
 *  ./syndrome_decoder
 * @details
 * Files Required:
 *  generator_matrix.txt - This file contains the generator matrix of the LBC. The generator matrix must
 *                         be systematic in th form G = [I | P] for the decoding to work.
 *  input.txt - This file contains the encoded message received after channel transmission of the code.
 *              The received message may be errorneous, but it should be lossless.
 * Generated Files:
 *  coset_array.txt - This file contains the generated coset array generated for syndrome decoding
 *  decoded.txt - This file contains the error-corrected channel-decoded message
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int n, k, cosets_row_count, cosets_column_count, *taken;
int **generator_matrix, **parity_check_matrix, **cosets_array, **HT, **syndrome, **uncoded_messages, **encoded_messages, **coset_leader;

void read_generator_matrix()
{
    FILE *fp = fopen("generator_matrix.txt", "r");
    if (fp == NULL)
    {
        printf("Cannot read file \"generator_matrix.txt\".\n");
        exit(1);
    }
    fscanf(fp, "%d %d", &k, &n);
    cosets_row_count = (1 << (n - k));
    cosets_column_count = (1 << k);
    generator_matrix = (int **)malloc(k * sizeof(int *));
    taken = (int *)malloc((1 << n) * sizeof(int));
    memset(taken, 0, sizeof(taken));
    for (int i = 0; i < k; i++)
    {
        generator_matrix[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
        {
            fscanf(fp, "%d", &generator_matrix[i][j]);
        }
    }
    fclose(fp);
    parity_check_matrix = (int **)malloc((n - k) * sizeof(int *));
    for (int i = 0; i < (n - k); i++)
    {
        parity_check_matrix[i] = (int *)malloc(n * sizeof(int));
    }
    for (int i = 0; i < (n - k); i++)
    {
        for (int j = 0; j < k; j++)
        {
            parity_check_matrix[i][j] = generator_matrix[j][k + i];
        }
    }
    for (int i = 0; i < (n - k); i++)
    {
        for (int j = k; j < n; j++)
        {
            if (j - k == i)
            {
                parity_check_matrix[i][j] = 1;
            }
            else
            {
                parity_check_matrix[i][j] = 0;
            }
        }
    }
    HT = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        HT[i] = (int *)malloc((n - k) * sizeof(int));
        for (int j = 0; j < (n - k); j++)
        {
            HT[i][j] = parity_check_matrix[j][i];
        }
    }
}

int **matrix_multiplication(int **a, int **b, int x, int y, int z)
{
    int **multiplied_matrix = (int **)malloc(x * sizeof(int *));
    for (int i = 0; i < x; i++)
    {
        multiplied_matrix[i] = (int *)malloc(z * sizeof(int));
    }
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < z; j++)
        {
            int sum = 0;
            for (int k = 0; k < y; k++)
            {
                sum = (sum + a[i][k] * b[k][j]) % 2;
            }
            multiplied_matrix[i][j] = sum;
        }
    }
    return multiplied_matrix;
}

int find_next_error()
{
    for (int bit = 0; bit <= n; bit++)
    {
        for (int i = (1 << n) - 1; i >= 0; i--)
        {
            if (taken[i] == 1)
            {
                continue;
            }
            int bit_count = 0, temp = i;
            while (temp)
            {
                bit_count = bit_count + temp % 2;
                temp /= 2;
            }
            if (bit_count == bit)
            {
                return i;
            }
        }
    }
    return -1;
}

void create_possible_received_codes()
{
    cosets_array = (int **)malloc(cosets_row_count * sizeof(int *));
    uncoded_messages = (int **)malloc((1 << k) * sizeof(int *));
    coset_leader = (int **)malloc(cosets_row_count * sizeof(int *));
    for (int i = 0; i < (1 << k); i++)
    {
        uncoded_messages[i] = (int *)malloc(k * sizeof(int));
        int temp = i;
        for (int j = k - 1; j >= 0; j--)
        {
            uncoded_messages[i][j] = temp % 2;
            temp /= 2;
        }
    }
    encoded_messages = matrix_multiplication(uncoded_messages, generator_matrix, (1 << k), k, n);
    for (int i = 0; i < cosets_row_count; i++)
    {
        coset_leader[i] = (int *)malloc(n * sizeof(int));
        int err = find_next_error();
        for (int j = n - 1; j >= 0; j--)
        {
            coset_leader[i][j] = err % 2;
            err /= 2;
        }
        cosets_array[i] = (int *)malloc(cosets_column_count * sizeof(int));
        for (int j = 0; j < cosets_column_count; j++)
        {
            int val = 0, mul = 1;
            for (int itr = n - 1; itr >= 0; itr--)
            {
                int x = (encoded_messages[j][itr] + coset_leader[i][itr]) % 2;
                val = val + mul * x;
                mul <<= 1;
            }
            cosets_array[i][j] = val;
            taken[val] = 1;
        }
    }
    syndrome = matrix_multiplication(coset_leader, HT, cosets_row_count, n, n - k);
}

int find_syndrome(int *syn)
{
    for (int i = 0; i < cosets_row_count; i++)
    {
        int flag = 1;
        for (int j = 0; j < (n - k); j++)
        {
            if (syndrome[i][j] != syn[j])
            {
                flag = 0;
                break;
            }
        }
        if (flag == 1)
        {
            return i;
        }
    }
    return -1;
}

char *decode(char *codeword)
{
    int **vector = (int **)malloc(sizeof(int *));
    vector[0] = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        vector[0][i] = codeword[i] - '0';
    }
    int **derived_syndrome = matrix_multiplication(vector, HT, 1, n, n - k);
    int idx = find_syndrome(derived_syndrome[0]);
    for (int j = 0; j < n; j++)
    {
        vector[0][j] = (vector[0][j] + coset_leader[idx][j]) % 2;
    }
    char *decoded_message = (char *)malloc((k + 1) * sizeof(char));
    for (int j = 0; j < k; j++)
    {
        decoded_message[j] = (char)(vector[0][j] + '0');
    }
    decoded_message[k] = '\0';
    free(vector[0]);
    free(derived_syndrome[0]);
    free(vector);
    free(derived_syndrome);
    return decoded_message;
}

void decode_received_codes()
{
    FILE *fpi = fopen("input.txt", "r");
    if (fpi == NULL)
    {
        printf("File \'input.txt\' not found\n");
        exit(2);
    }
    FILE *fpo = fopen("decoded.txt", "w");
    char *buffer = (char *)malloc((n + 1) * sizeof(char));
    int buffer_size = 0;
    while (fscanf(fpi, "%c", &buffer[buffer_size]) == 1)
    {
        buffer_size++;
        if (buffer_size == n)
        {
            buffer[buffer_size] = '\0';
            char *decoded_message = decode(buffer);
            fprintf(fpo, "%s", decoded_message);
            buffer_size = 0;
        }
    }
    fclose(fpi);
    fclose(fpo);
    printf("File successfully decoded to \"decoded.txt\"\n");
}

void print_coset_array()
{
    FILE *fp = fopen("coset_array.txt", "w");
    for (int i = 0; i < cosets_row_count; i++)
    {
        fprintf(fp, "Coset Leader: ");
        for (int j = 0; j < n; j++)
        {
            fprintf(fp, "%d", coset_leader[i][j]);
        }
        fprintf(fp, "    Coset: ");
        for (int j = 0; j < cosets_column_count; j++)
        {
            int *vector = (int *)malloc(n * sizeof(int)), val = cosets_array[i][j];
            for (int itr = n - 1; itr >= 0; itr--)
            {
                vector[itr] = val % 2;
                val /= 2;
            }
            for (int itr = 0; itr < n; itr++)
            {
                fprintf(fp, "%d", vector[itr]);
            }
            free(vector);
            if (j < cosets_column_count - 1)
            {
                fprintf(fp, ", ");
            }
        }
        fprintf(fp, "    Syndrome: ");
        for (int j = 0; j < (n - k); j++)
        {
            fprintf(fp, "%d", syndrome[i][j]);
        }
        fprintf(fp, "\n");
    }
    printf("Coset Array has been successfully saved to file \"coset_array.txt\".\n");
    fclose(fp);
}

void cleanup()
{
    for (int i = 0; i < k; i++)
    {
        free(generator_matrix[i]);
    }
    free(generator_matrix);
    for (int i = 0; i < n; i++)
    {
        free(HT[i]);
    }
    free(HT);
    for (int i = 0; i < (n - k); i++)
    {
        free(parity_check_matrix[i]);
    }
    free(parity_check_matrix);
    for (int i = 0; i < cosets_row_count; i++)
    {
        free(cosets_array[i]);
        free(coset_leader[i]);
        free(syndrome[i]);
    }
    free(coset_leader);
    free(cosets_array);
    free(syndrome);
    free(taken);
    for (int i = 0; i < (1 << k); i++)
    {
        free(uncoded_messages[i]);
        free(encoded_messages[i]);
    }
    free(uncoded_messages);
    free(encoded_messages);
}

int exec()
{
    read_generator_matrix();
    create_possible_received_codes();
    decode_received_codes();
    print_coset_array();
    cleanup();
    return 0;
}

int main()
{
    return exec();
}
