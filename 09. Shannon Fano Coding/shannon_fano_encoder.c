/**
 * @file shannon_fano_encoder.c
 * @brief
 * Compile Command:
 *  gcc -lm shannon_fano_encoder.c -o shannon_fano_encoder
 * Run Command:
 *  ./shannon_fano_encoder
 * @details
 * Two files will be created for each run of the code:
 *  encoded.txt: This file contains the encoded version of the message in input.txt
 *  mapping.txt: This file contains the symbol to codeword mapping used in encoding of input.
 *               This file is also preserved as it is used during decoding of data from encoded message.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *freq;
char *symbol, **code;

int symbol_index = 0, char_count = 0;

struct node
{
    char ch;
    int ctr;
} *node_arr;

void form_tree(int l, int r, char *codeword, int depth, FILE *fp)
{
    if (l > r)
    {
        return;
    }
    if (l == r)
    {
        symbol[symbol_index] = node_arr[l].ch;
        code[symbol_index] = (char *)malloc((depth + 1) * sizeof(char));
        strcpy(code[symbol_index], codeword);
        fprintf(fp, "%c %s\n", symbol[symbol_index], code[symbol_index]);
        symbol_index++;
        return;
    }
    int total_sum = 0;
    for (int i = l; i <= r; i++)
    {
        total_sum += node_arr[i].ctr;
    }
    int right_sum = 0, min_diff = total_sum, idx = -1;
    for (int i = l; i <= r; i++)
    {
        right_sum += node_arr[i].ctr;
        int s = total_sum - 2 * right_sum;
        if (s < 0)
            s *= -1;
        if (s < min_diff)
        {
            min_diff = s;
            idx = i;
        }
    }
    char temp[20];
    strcpy(temp, codeword);
    strcat(codeword, "1");
    strcat(temp, "0");
    form_tree(l, idx, temp, depth + 1, fp);
    form_tree(idx + 1, r, codeword, depth + 1, fp);
}

void form_map()
{
    FILE *fp = fopen("mapping.txt", "w");
    for (int i = 0; i < 256; i++)
    {
        if (freq[i] > 0)
        {
            char_count++;
        }
    }
    fprintf(fp, "%d\n", char_count);
    node_arr = (struct node *)malloc(char_count * sizeof(struct node));
    int j = 0;
    for (int i = 0; i < 256; i++)
    {
        if (freq[i] > 0)
        {
            node_arr[j].ch = (char)i;
            node_arr[j].ctr = freq[i];
            j++;
        }
    }
    // Sorting using insertion sort
    for (int i = 1; i < char_count; i++)
    {
        struct node key = node_arr[i];
        j = i - 1;
        while (j >= 0 && node_arr[j].ctr > key.ctr)
        {
            node_arr[j + 1] = node_arr[j];
            j--;
        }
        node_arr[j + 1] = key;
    }
    code = (char **)malloc(char_count * sizeof(char *));
    symbol = (char *)malloc(char_count * sizeof(char));
    char codeword[20] = "";
    int depth = 0;
    form_tree(0, char_count - 1, codeword, depth, fp);
    fclose(fp);
}

int exec()
{
    freq = (int *)malloc(256 * sizeof(int));
    for (int i = 0; i < 256; i++)
        freq[i] = 0;
    FILE *fp1 = fopen("input.txt", "r");
    if (fp1 == NULL)
    {
        printf("Cannot read \"input.txt\"\n");
        return 1;
    }
    fpos_t position;
    fgetpos(fp1, &position);
    char word;
    while (fscanf(fp1, "%c", &word) == 1)
    {
        freq[(int)word]++;
    }
    form_map();
    fsetpos(fp1, &position);
    FILE *fp2 = fopen("encoded.txt", "w");
    while (fscanf(fp1, "%c", &word) == 1)
    {
        int encoded = 0;
        for (int i = 0; i < char_count; i++)
        {
            if (symbol[i] == word)
            {
                encoded = 1;
                fprintf(fp2, "%s", code[i]);
                break;
            }
        }
        if (!encoded)
        {
            printf("Unknown symbol \'%c\' found in input.\n");
            return 2;
        }
    }
    printf("Successfully encoded file to \"encoded.txt\"\n");
    fclose(fp2);
    fclose(fp1);
}

int main()
{
    return exec();
}
