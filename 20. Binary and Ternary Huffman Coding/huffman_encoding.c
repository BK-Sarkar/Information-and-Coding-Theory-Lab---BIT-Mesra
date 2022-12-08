/**
 * @file huffman_encoding.c
 * @brief
 * Compile Command:
 *  gcc -lm huffman_encoding.c -o huffman_encoding
 *
 * Run Command:
 *  ./huffman_encoding
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Huffman coding. Consider the random variable
 * X = x1 x2 x3 x4 x5 x6 x7 [0.50 0.26 0.11 0.04 0.04 0.03 0.02 ]
 * (a) Find a binary Huffman code for X.
 * (b) Find the expected code length for this encoding.
 * (c) Extend the Binary Huffman method to Ternarry (Alphabet of 3) and apply it for X.
 */
double *probability;
int char_count = 0, node_arr_size, symbol_index = 0;
char *symbol, **code;

struct Node
{
    int sym;
    double count;
    struct Node **children;
};

struct Node **node_arr;

struct Node *get_new_node(int c)
{
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    node->sym = 256;
    node->count = 0;
    node->children = (struct Node **)malloc(c * sizeof(struct Node *));
    for (int i = 0; i < c; i++)
    {
        node->children[i] = NULL;
    }
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

struct Node *form_tree(int c)
{
    for (int i = (node_arr_size - 1) / 2; i >= 0; i--)
    {
        push_down(i);
    }
    while (node_arr_size > 1)
    {
        int ctr = 0;
        struct Node *new_node = get_new_node(c);
        while (node_arr_size > 0 && ctr < c)
        {
            struct Node *t = pop();
            new_node->count += t->count;
            new_node->children[ctr] = t;
            ctr++;
        }
        push(new_node);
    }
    return pop();
}

void DFT(struct Node *root, char *codeword, int c)
{
    if (root == NULL)
    {
        return;
    }
    if (root->sym < 256)
    {
        symbol[symbol_index] = (char)(root->sym);
        strcat(code[symbol_index], codeword);
        symbol_index++;
        return;
    }
    for (int i = 0; i < c; i++)
    {
        char t[20] = "", dig[2] = {i + 48, '\0'};
        strcpy(t, codeword);
        strcat(t, dig);
        DFT(root->children[i], t, c);
    }
    return;
}

void form_codes(int c)
{
    node_arr_size = char_count;
    int j = 0;
    for (int i = 0; i < 256; i++)
    {
        if (probability[i] > 0)
        {
            symbol[j] = (char)i;
            node_arr[j] = get_new_node(c);
            node_arr[j]->sym = symbol[j];
            node_arr[j]->count = probability[i];
            j++;
        }
    }
    struct Node *root = form_tree(c);
    char codeword[20] = "";
    DFT(root, codeword, c);
    for (int i = 0; i < char_count; i++)
    {
        printf("%c %s\n", symbol[i], code[i]);
    }
    printf("\n");
    return;
}

int exec()
{
    probability = (double *)malloc(256 * sizeof(double));
    memset(probability, 0, sizeof(probability));
    probability['A'] = 0.5, probability['B'] = 0.26, probability['C'] = 0.11, probability['D'] = 0.04;
    probability['E'] = 0.04, probability['F'] = 0.03, probability['G'] = 0.02;
    for (int i = 0; i < 256; i++)
    {
        if (probability[i] > 0)
        {
            char_count++;
        }
    }
    symbol = (char *)malloc(char_count * sizeof(char));
    code = (char **)malloc(char_count * sizeof(char *));
    node_arr = (struct Node **)malloc(char_count * sizeof(struct Node *));
    for (int i = 0; i < char_count; i++)
    {
        code[i] = (char *)malloc(20 * sizeof(char));
        for (int j = 0; j < 20; j++)
        {
            code[i][j] = '\0';
        }
    }
    printf("Binary Huffman Code:\n");
    form_codes(2);
    symbol_index = 0;
    for (int i = 0; i < char_count; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            code[i][j] = '\0';
        }
    }
    printf("Ternary Huffman Code:\n");
    form_codes(3);
    return 0;
}

int main()
{
    return exec();
}
