/**
 * @file self-information_and_entropy.c
 * @brief
 * Compile Command:
 *  gcc -lm self-information_and_entropy.c -o self-information_and_entropy
 * Run Command:
 *  ./self-information_and_entropy
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char *get_source()
{
    char *source = (char *)malloc(100 * sizeof(char));
    memset(source, 0, sizeof(source));
    printf("Enter the generated symbols from source: ");
    scanf("%[^\n]s", source);
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
    double self_information[number_of_distinct_symbols], entropy = 0;
    int j = 0;
    for (int i = 0; i < 256; i++)
    {
        if (frequency[i] > 0)
        {
            symbol[j] = (char)i;
            double probability = ((double)frequency[i]) / n;
            self_information[j] = -log2(probability);
            entropy = entropy + self_information[j] * probability;
            j++;
        }
    }
    printf("Self-Information of Symbols:\n");
    for (int i = 0; i < number_of_distinct_symbols; i++)
    {
        printf("%c   %.11f\n", symbol[i], self_information[i]);
    }
    printf("\nEntropy of source: %.11f\n", entropy);
}

int main()
{
    char *source = get_source();
    calc_self_info_entropy(source);
    return 0;
}
