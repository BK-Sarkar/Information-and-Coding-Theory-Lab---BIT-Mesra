/**
 * @file huffman_encoder.c
 * @brief
 * Compile Command:
 *  gcc -lm huffman_encoder.c -o huffman_encoder
 * Run Command:
 *  ./huffman_encoder
 * @details
 * Two files will be created for each run of the code:
 *  encoded.txt: This file contains the encoded version of the message in input.txt
 *  mapping.txt: This file contains the symbol to codeword mapping used in encoding of input.
 *               This file is also preserved as it is used during decoding of data from encoded message.
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>

int *freq, char_count = 0, node_arr_size, symbol_index = 0;
char *symbol, **code;
double *probability;

struct Node
{
    int sym;
    int count;
    struct Node *left, *right;
};

struct Node **node_arr;

struct Node *get_new_node()
{
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    node->sym = 256;
    node->count = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void push_down(int i)
{
    int left = 2 * i + 1, right = 2 * i + 2, smallest = i;
    if (left < node_arr_size && node_arr[left]->count < node_arr[smallest]->count)
    {
        smallest = left;
    }
    if (right < node_arr_size && node_arr[right]->count < node_arr[smallest]->count)
    {
        smallest = right;
    }
    if (smallest != i)
    {
        struct Node *temp = node_arr[i];
        node_arr[i] = node_arr[smallest];
        node_arr[smallest] = temp;
        push_down(smallest);
    }
    return;
}

void pull_up(int i)
{
    int par;
    if (i % 2 == 0)
    {
        par = (i - 1) / 2;
    }
    else
    {
        par = (i - 2) / 2;
    }
    if (par >= 0 && node_arr[i]->count < node_arr[par]->count)
    {
        struct Node *temp = node_arr[i];
        node_arr[i] = node_arr[par];
        node_arr[par] = temp;
        pull_up(par);
    }
    return;
}

struct Node *pop()
{
    struct Node *small = node_arr[0];
    node_arr[0] = node_arr[node_arr_size - 1];
    node_arr_size--;
    push_down(0);
    return small;
}

void push(struct Node *new_node)
{
    node_arr[node_arr_size] = new_node;
    node_arr_size++;
    pull_up(node_arr_size - 1);
    return;
}

struct Node *form_tree()
{
    for (int i = (node_arr_size - 1) / 2; i >= 0; i--)
    {
        push_down(i);
    }
    while (node_arr_size > 1)
    {
        struct Node *t1 = pop();
        struct Node *t2 = pop();
        struct Node *new_node = get_new_node();
        new_node->count = t1->count + t2->count;
        new_node->left = t1, new_node->right = t2;
        push(new_node);
    }
    return pop();
}

void DFT(struct Node *root, char *codeword, int depth)
{
    if (root == NULL)
    {
        return;
    }
    if (root->sym < 256)
    {
        symbol[symbol_index] = (char)(root->sym);
        code[symbol_index] = (char *)malloc((depth + 1) * sizeof(char));
        strcpy(code[symbol_index], codeword);
        symbol_index++;
    }
    char t1[20], t2[20];
    strcpy(t1, codeword);
    strcpy(t2, codeword);
    strcat(t1, "0");
    strcat(t2, "1");
    DFT(root->left, t1, depth + 1);
    DFT(root->right, t2, depth + 1);
    return;
}

void form_codes()
{
    double total_freq = 0;
    for (int i = 0; i < 256; i++)
    {
        if (freq[i] > 0)
        {
            char_count++;
            total_freq += freq[i];
        }
    }
    symbol = (char *)malloc(char_count * sizeof(char));
    code = (char **)malloc(char_count * sizeof(char *));
    probability = (double *)malloc(char_count * sizeof(double));
    node_arr = (struct Node **)malloc(char_count * sizeof(struct Node *));
    node_arr_size = char_count;
    int j = 0;
    for (int i = 0; i < 256; i++)
    {
        if (freq[i] > 0)
        {
            symbol[j] = (char)i;
            probability[j] = ((double)freq[i]) / total_freq;
            node_arr[j] = get_new_node();
            node_arr[j]->sym = symbol[j];
            node_arr[j]->count = freq[i];
            j++;
        }
    }
    struct Node *root = form_tree();
    free(node_arr);
    char codeword[20] = "";
    DFT(root, codeword, 0);
    FILE *fp = fopen("mapping.txt", "w");
    fprintf(fp, "%d\n", char_count);
    for (int i = 0; i < char_count; i++)
    {
        fprintf(fp, "%c %s\n", symbol[i], code[i]);
    }
    fclose(fp);
    double entropy = 0, expected_length = 0;
    for (int i = 0; i < char_count; i++)
    {
        entropy = entropy - probability[i] * log2(probability[i]);
        expected_length = expected_length + probability[i] * strlen(code[i]);
    }
    double efficiency = entropy / expected_length;
    printf("Entropy: %.11f\nExpected Length: %.11f\nCode Efficiency: %.11f\n", entropy, expected_length, efficiency);
    return;
}

int exec()
{
    freq = (int *)malloc(256 * sizeof(int));
    memset(freq, 0, sizeof(freq));
    FILE *fp1 = fopen("input.txt", "r");
    FILE *fp2 = fopen("encoded.txt", "w");
    if (fp1 == NULL)
    {
        printf("Cannot access file \"input.txt\"\n");
        return 1;
    }
    fpos_t pos;
    fgetpos(fp1, &pos);
    char word;
    while (fscanf(fp1, "%c", &word) == 1)
    {
        freq[(int)word]++;
    }
    form_codes();
    fsetpos(fp1, &pos);
    while (fscanf(fp1, "%c", &word) == 1)
    {
        for (int i = 0; i < char_count; i++)
        {
            if (symbol[i] == word)
            {
                fprintf(fp2, "%s", code[i]);
                break;
            }
        }
    }
    fclose(fp1);
    fclose(fp2);
    return 0;
}

int main()
{
    return exec();
}
