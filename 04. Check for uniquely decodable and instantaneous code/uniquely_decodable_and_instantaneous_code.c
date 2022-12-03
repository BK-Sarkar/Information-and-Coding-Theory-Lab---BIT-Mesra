/**
 * @file uniquely_decodable_and_instantaneous_code.c
 * @brief
 *  The necessary and sufficeint condition for a code to be uniquely decodable is that if it satisfies Kraft's Inequality.
 *  For a code to be instantaneous, any complete codeword must not be a prefix of any other codeword.
 *  It is assumed that the input code is non-singular.
 * Compile Command:
 *  gcc -lm uniquely_decodable_and_instantaneous_code.c -o uniquely_decodable_and_instantaneous_code
 * Run Command:
 *  ./uniquely_decodable_and_instantaneous_code
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int number_of_codewords = 0, q;
char **code;

struct trie_node
{
    int is_end, visited;
    struct trie_node **children;
};

struct trie_node *get_new_node()
{
    struct trie_node *node = (struct trie_node *)malloc(sizeof(struct trie_node));
    node->is_end = 0;
    node->visited = 0;
    node->children = (struct trie_node **)malloc(256 * sizeof(struct trie_node *));
    for (int i = 0; i < 256; i++)
    {
        node->children[i] = NULL;
    }
    return node;
}

int insert(struct trie_node *root, char *str)
{
    struct trie_node *temp = root;
    int n = strlen(str);
    for (int i = 0; i < n; i++)
    {
        temp->visited = 1;
        char ch = str[i];
        if (temp->children[(int)ch] == NULL)
        {
            if (temp->is_end == 1)
            {
                return 0;
            }
            temp->children[(int)ch] = get_new_node();
        }
        temp = temp->children[(int)ch];
    }
    temp->is_end = 1;
    if (temp->visited == 1)
    {
        return 0;
    }
    temp->visited = 1;
    return 1;
}

void read_input()
{
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL)
    {
        printf("File \'input.txt\' not found\n");
        exit(1);
    }
    // Input Field Size
    fscanf(fp, "%d\n", &q);
    code = (char **)malloc(1000 * sizeof(char *));
    for (int i = 0; i < 1000; i++)
    {
        code[i] = (char *)malloc(1000 * sizeof(char));
        memset(code[i], 0, sizeof(code[i]));
    }
    int i = 0;
    while (!feof(fp))
    {
        int j = 0;
        char ch = fgetc(fp);
        while (ch != '\n' && !feof(fp))
        {
            code[i][j] = ch;
            j++;
            if (!feof(fp))
            {
                ch = fgetc(fp);
            }
        }
        i++;
    }
    number_of_codewords = i;
}

int is_uniquely_decodable()
{
    long double sum = 0;
    for (int i = 0; i < number_of_codewords; i++)
    {
        int len = strlen(code[i]);
        sum = sum + ((long double)1) / ((long double)pow(q, len));
    }
    return (sum <= 1);
}

int is_instantaneous()
{
    /**
     * @brief
     *  We will use Trie Data Structure
     *  (or any other prefix structure that you may like)
     *  to verify whether the given code is instantaneous or not
     */
    struct trie_node *root = get_new_node();
    for (int i = 0; i < number_of_codewords; i++)
    {
        int x = insert(root, code[i]);
        if (x == 0)
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    read_input();
    int flag = is_uniquely_decodable();
    if (flag == 0)
    {
        printf("The given code is not uniquely decodable\n");
        return 0;
    }
    printf("The given code is uniquely decodable\n");
    flag = is_instantaneous();
    if (flag == 0)
    {
        printf("The given code is not instantaneous\n");
        return 0;
    }
    printf("The given code is instantaneous\n");
    return 0;
}
