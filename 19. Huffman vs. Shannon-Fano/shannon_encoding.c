/**
 * @file shannon_encoding.c
 * @brief
 * Compile Command:
 *  gcc -lm shannon_encoding.c -o shannon_encoding
 *
 * Run Command:
 *  ./shannon_encoding
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Given symbols and their probabilities, Huffman source encoding was
 * done on those symbols and code efficiency was found to be
 * 0.977195454. Now apply Shannon-Fano source encoding and find
 * the code-efficiency of this algorithm on the same given example.
 * Observe the differences between the both source encoding schemes.
 * Given:
 * Efficiency for Huffman Encoding = 0.977195454
 * H(x) = Source Entropy = 2.14983
 */

double *probability;
char *symbol, **code;

int symbol_index = 0, char_count = 0;

struct node
{
    char ch;
    double ctr;
} *node_arr;

void form_tree(int l, int r, char *codeword, int depth)
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
        printf("%c %s\n", symbol[symbol_index], code[symbol_index]);
        symbol_index++;
        return;
    }
    double total_sum = 0;
    for (int i = l; i <= r; i++)
    {
        total_sum += node_arr[i].ctr;
    }
    double right_sum = 0, min_diff = total_sum;
    int idx = -1;
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
    form_tree(l, idx, temp, depth + 1);
    form_tree(idx + 1, r, codeword, depth + 1);
}

void form_map()
{
    for (int i = 0; i < 256; i++)
    {
        if (probability[i] > 0)
        {
            char_count++;
        }
    }
    node_arr = (struct node *)malloc(char_count * sizeof(struct node));
    int j = 0;
    for (int i = 0; i < 256; i++)
    {
        if (probability[i] > 0)
        {
            node_arr[j].ch = (char)i;
            node_arr[j].ctr = probability[i];
            j++;
        }
    }
    for (int i = 1; i < char_count; i++)
    {
        struct node key = node_arr[i];
        j = i - 1;
        while (j >= 0 && node_arr[j].ctr < key.ctr)
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
    form_tree(0, char_count - 1, codeword, depth);
    double expected_length = 0;
    for (int i = 0; i < char_count; i++)
    {
        expected_length = expected_length + probability[symbol[i]] * strlen(code[i]);
    }
    double huffman_efficiency = 0.977195454, source_entropy = 2.14983;
    double shannon_efficeincy = source_entropy / expected_length;
    printf("Huffman Code Efficiency: %lf\nShannon-Fano Code Efficiency: %lf\n", huffman_efficiency, shannon_efficeincy);
}

int main()
{
    probability = (double *)malloc(256 * sizeof(double));
    memset(probability, 0, sizeof(probability));
    probability['A'] = 0.4, probability['B'] = 0.19, probability['C'] = 0.16, probability['D'] = 0.15, probability['E'] = 0.1;
    form_map();
    return 0;
}
