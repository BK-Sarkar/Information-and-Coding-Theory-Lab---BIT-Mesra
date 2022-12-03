// Run combined_encoder.sh
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int substring_count;
char substring[1000][1000], code[1000][20];

void form_map()
{
    FILE *fp = fopen("mapping.txt", "r");
    if (fp == NULL)
    {
        printf("Cannot read \"mapping.txt\"\n");
        exit(1);
    }
    fscanf(fp, "%d", &substring_count);
    memset(substring, '\0', sizeof(substring));
    int substring_index = 0;
    char codeword[20], g;
    fscanf(fp, "%c", &g);
    int symbol_length;
    while (fscanf(fp, "%d", &symbol_length) == 1)
    {
        fscanf(fp, "%c", &g);
        for (int i = 0; i < symbol_length; i++)
        {
            fscanf(fp, "%c", &g);
            substring[substring_index][i] = g;
        }
        fscanf(fp, "%c", &g);
        int j = 0;
        fscanf(fp, "%c", &g);
        while (g != '\n')
        {
            codeword[j] = g;
            fscanf(fp, "%c", &g);
            j++;
        }
        codeword[j] = '\0';
        for (int k = 0; k < j; k++)
        {
            code[substring_index][k] = codeword[k];
        }
        substring_index++;
    }
    fclose(fp);
    return;
}

int exec()
{
    form_map();
    FILE *fp1 = fopen("encoded.txt", "r");
    if (fp1 == NULL)
    {
        printf("Cannot access \"encoded.txt\"\n");
        return 1;
    }
    FILE *fp2 = fopen("decoded.txt", "w");
    char buffer[20], word;
    memset(buffer, '\0', sizeof(buffer));
    int buffer_index = 0;
    while (fscanf(fp1, "%c", &word) == 1)
    {
        buffer[buffer_index] = word;
        buffer_index++;
        for (int i = 0; i < substring_count; i++)
        {
            if (strcmp(buffer, code[i]) == 0)
            {
                buffer_index = 0;
                memset(buffer, '\0', sizeof(buffer));
                fprintf(fp2, "%s", substring[i]);
                break;
            }
        }
    }
    if (buffer_index > 0)
    {
        printf("Codeword %s not decoded.\n", buffer);
        return 2;
    }
    printf("Successfully decoded file to \"decoded.txt\"\n");
    fclose(fp2);
    fclose(fp1);
    return 0;
}

int main()
{
    return exec();
}
