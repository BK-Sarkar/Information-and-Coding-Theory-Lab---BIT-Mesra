/**
 * @file self-information_each_symbol.c
 * @brief
 * Compilation Command:
 *  gcc -lm self-information_each_symbol.c -o self-information_each_symbol
 * Execution Command:
 *  ./self-information_each_symbol
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char *get_source()
{
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL)
    {
        printf("File \'input.txt\' not found\n");
        exit(1);
    }
    char *source = (char *)malloc(100001 * sizeof(char));
    memset(source, 0, sizeof(source));
    int i = 0;
    while (fscanf(fp, "%c", &source[i]) == 1 && i < 100000)
    {
        i++;
    }
    fclose(fp);
    return source;
}

void calc_self_info_entropy(char *source)
{
    int n = strlen(source);
    int frequency[256];
    memset(frequency, 0, sizeof(frequency));
    for (int i = 0; i < n; i++)
    {
        frequency[(int)source[i]]++;
    }
    int number_of_distinct_symbols = 0;
    for (int i = 0; i < 256; i++)
    {
        if (frequency[i] > 0)
        {
            number_of_distinct_symbols++;
        }
    }
    char symbol[number_of_distinct_symbols];
    double self_information[number_of_distinct_symbols];
    int j = 0;
    for (int i = 0; i < 256; i++)
    {
        if (frequency[i] > 0)
        {
            symbol[j] = (char)i;
            double probability = ((double)frequency[i]) / n;
            self_information[j] = -log2(probability);
            j++;
        }
    }
    printf("Self-Information of each symbol:\n");
    for (int i = 0; i < number_of_distinct_symbols; i++)
    {
        printf("%c   %.11f\n", symbol[i], self_information[i]);
    }
}

int main()
{
    char *source = get_source();
    calc_self_info_entropy(source);
    return 0;
}