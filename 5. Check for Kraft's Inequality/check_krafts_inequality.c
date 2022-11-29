/**
 * @file check_krafts_inequality.c
 * @brief
 * Compile Command:
 *  gcc -lm check_krafts_inequality.c -o check_krafts_inequality
 * Run Command:
 *  ./check_krafts_inequality
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char **code;
int number_of_codewords = 0, q;

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

int satisfies_krafts_inequality()
{
    long double sum = 0;
    for (int i = 0; i < number_of_codewords; i++)
    {
        int len = strlen(code[i]);
        sum = sum + ((long double)1) / ((long double)pow(q, len));
    }
    return (sum <= 1);
}

int main()
{
    read_input();
    int flag = satisfies_krafts_inequality();
    if (flag == 1)
    {
        printf("The given code satisfies Kraft\'s Inequality\n");
        return 0;
    }
    printf("The given code does not satisfy Kraft\'s Inequality\n");
    return 0;
}
