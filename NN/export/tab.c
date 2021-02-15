#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

char* lower(char* str)
{
    char* s = malloc(255);
    for (int i = 0; str[i] != '\0'; i++)
    {
        s[i] = tolower(str[i]);
    }
    return s;
}

int levenshtein(char *s1, char *s2) 
{
    unsigned int x, y, s1len, s2len;

    s1len = strlen(s1);
    s2len = strlen(s2);

    unsigned int matrix[s2len+1][s1len+1];

    matrix[0][0] = 0;
    for (x = 1; x <= s2len; x++)
        matrix[x][0] = matrix[x-1][0] + 1;

    for (y = 1; y <= s1len; y++)
        matrix[0][y] = matrix[0][y-1] + 1;

    for (x = 1; x <= s2len; x++)
        for (y = 1; y <= s1len; y++)
            matrix[x][y] = MIN3(matrix[x-1][y] + 1, matrix[x][y-1] + 1, matrix[x-1][y-1] + (s1[y-1] == s2[x-1] ? 0 : 1));

    return(matrix[s2len][s1len]);
}

int main(int argc, char *argv[])
{
    FILE* fichier = NULL;
    char chaine[100] = "";
    fichier = fopen("dico.txt", "r");
    int list[10000000];
    char* mots[10000000];

    if (fichier != NULL)
    {
        for (int j = 0; words[j] != NULL ; j++)
        {
            int i = 0;
            while(fgets(chaine, 100, fichier))
            {
                mots[i] = lower(chaine);
            }
        }
        fclose(fichier);
    }
    else
    {
        printf("erreur");
    }

    for (int i = 0; mots[i][0] != '\0'; i++)
    {
        list[i] = levenshtein(chaine, lower(words[i]));
    }
    
    int min = list[0];
    int minindex = 0;
    for (int j = 0; list[j] != 0; j++)
    {
        if (list[j] < min) 
        {
            min = list[j];
            minindex = j;
        }
    }

    printf("%s\n", mots[minindex]);
    return 0;
}
