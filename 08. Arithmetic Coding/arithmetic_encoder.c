/**
 * @file arithmetic_encoder.c
 * @brief
 * Compile Command:
 *  gcc -lm arithmetic_encoder.c -o arithmetic_encoder
 * Run Command:
 *  ./arithmetic_encoder
 * @details
 * Two files will be created for each run of the code:
 *  encoded.txt: This file contains the encoded version of the message in input.txt
 *  mapping.txt: This file contains how many levels of magnification we used in encoding of input and mapping of characters used to their respective probabilities in input.
 *               This file is also preserved as it is used during decoding of data from encoded message.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int freq[256], symbol_count = 0, depth = 0;
long double *upper_limit, *lower_limit, *probability, global_lower = 0, global_upper = 1;
char *symbol, codeword[100];

void convert_binary(long double code)
{
    memset(codeword, '\0', sizeof(codeword));
    while (code > 0)
    {
        code = code * 2;
        if (code >= 1)
        {
            strcat(codeword, "1");
            code -= 1;
        }
        else
        {
            strcat(codeword, "0");
        }
    }
    strcat(codeword, "0");
    return;
}

void adjust_according_to(int idx)
{
    long double lo = lower_limit[idx], up = upper_limit[idx];
    for (int i = 0; i < symbol_count; i++)
    {
        lower_limit[i] = lo + (lower_limit[i] - global_lower) * (up - lo) / (global_upper - global_lower);
        upper_limit[i] = lo + (upper_limit[i] - global_lower) * (up - lo) / (global_upper - global_lower);
    }
    global_lower = lo;
    global_upper = up;
    return;
}

int find(char c)
{
    for (int i = 0; i < symbol_count; i++)
    {
        if (symbol[i] == c)
        {
            return i;
        }
    }
    return -1;
}

void form_probability_map()
{
    FILE *fp = fopen("mapping.txt", "w");
    int total_freq = 0;
    for (int i = 0; i < 256; i++)
    {
        if (freq[i] > 0)
        {
            symbol_count++;
            total_freq += freq[i];
        }
    }
    symbol = (char *)malloc(symbol_count * sizeof(char));
    upper_limit = (long double *)malloc(symbol_count * sizeof(long double));
    lower_limit = (long double *)malloc(symbol_count * sizeof(long double));
    probability = (long double *)malloc(symbol_count * sizeof(long double));
    int j = 0;
    fprintf(fp, "%d %d\n", symbol_count, depth);
    for (int i = 0; i < 256; i++)
    {
        if (freq[i] > 0)
        {
            symbol[j] = (char)i;
            probability[j] = ((long double)freq[i]) / ((long double)total_freq);
            fprintf(fp, "%c %Lf\n", symbol[j], probability[j]);
            lower_limit[j] = 0;
            if (j - 1 >= 0)
            {
                lower_limit[j] = upper_limit[j - 1];
            }
            upper_limit[j] = lower_limit[j] + probability[j];
            j++;
        }
    }
    fclose(fp);
    return;
}

int exec()
{
    memset(freq, 0, sizeof(freq));
    FILE *fp1 = fopen("input.txt", "r");
    if (fp1 == NULL)
    {
        printf("Cannot read file \"input.txt\"\n");
        return 1;
    }
    FILE *fp2 = fopen("encoded.txt", "w");
    fpos_t pos;
    fgetpos(fp1, &pos);
    char word;
    while (fscanf(fp1, "%c", &word) == 1)
    {
        depth++;
        freq[(int)word]++;
    }
    form_probability_map();
    fsetpos(fp1, &pos);
    while (fscanf(fp1, "%c", &word) == 1)
    {
        int idx = find(word);
        if (idx == -1)
        {
            printf("Unencodable symbol \'%c\' found in input!\n", word);
            return 2;
        }
        adjust_according_to(idx);
    }
    long double final_code = (global_lower + global_upper) / 2;
    convert_binary(final_code);
    fprintf(fp2, "%s", codeword);
    printf("File successfully encoded to \"encoded.txt\"\n");
    fclose(fp2);
    fclose(fp1);
    return 0;
}

int main()
{
    return exec();
}
