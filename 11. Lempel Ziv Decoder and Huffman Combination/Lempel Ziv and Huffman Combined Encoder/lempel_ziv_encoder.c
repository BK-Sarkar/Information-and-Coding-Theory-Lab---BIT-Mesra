// Run combined_encoder.sh

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char substring[1000][1000], code[1000][20];
int substring_size = 1;

int check(char *subst)
{
    for (int i = 0; i < 1000; i++)
    {
        if (strcmp(subst, substring[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

void phrase(int idx, char w)
{
    strcpy(substring[substring_size], substring[idx]);
    char temp[2] = {w, '\0'};
    strcat(substring[substring_size], temp);
    substring_size++;
    return;
}

void form_codes()
{
    FILE *fp = fopen("mapping.txt", "w");
    fprintf(fp, "%d\n", substring_size);
    int number_of_bits = log2(substring_size);
    if (pow(2, number_of_bits) < substring_size)
    {
        number_of_bits++;
    }
    for (int i = 0; i < substring_size; i++)
    {
        int temp = i, j = number_of_bits - 1;
        while (j >= 0)
        {
            code[i][j] = (temp % 2 + '0');
            temp /= 2;
            j--;
        }
        fprintf(fp, "%d %s %s\n", strlen(substring[i]), substring[i], code[i]);
    }
    fclose(fp);
    return;
}

int exec()
{
    FILE *fp1 = fopen("input.txt", "r");
    FILE *fp2 = fopen("encoded.txt", "w");
    if (fp1 == NULL)
    {
        printf("Cannot access file \"input.txt\"\n");
        return 1;
    }
    memset(substring, '\0', sizeof(substring));
    memset(code, '\0', sizeof(code));
    char word, subst[1000];
    memset(subst, '\0', sizeof(subst));
    int idx = 0, last_substring_index = 0;
    while (fscanf(fp1, "%c", &word) == 1)
    {
        subst[idx] = word;
        idx++;
        int chk = check(subst);
        if (chk == -1)
        {
            phrase(last_substring_index, word);
            last_substring_index = 0;
            memset(subst, '\0', sizeof(subst));
            idx = 0;
            continue;
        }
        last_substring_index = chk;
    }
    fclose(fp1);
    form_codes();
    for (int i = 1; i < substring_size; i++)
    {
        fprintf(fp2, "%s", code[i]);
    }
    fprintf(fp2, "%s", code[last_substring_index]);
    fclose(fp2);
    return 0;
}

int main()
{
    return exec();
}
