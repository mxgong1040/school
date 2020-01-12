/*Michelle Gong
CSE 2320, Homework 03*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*given hash function*/

int hash(char s[], int size, int m)
{
    int value = s[0] * m*m*m + s[1] * m*m + s[2] * m;

    return value%size;
}

void generateArr(int size, int mult)
{

    char str[] = "abcdefghijklmnopqrstuvwxyz";
    int all = 17576; /* 26x26x26 = all possible permutations */

    int i, j, k, index, h;
    int len;


    int tempLen;
    len = strlen(str);
    tempLen = 3;

    /*temp for the current 3-character string in use*/
    char *temp = (char*)malloc(sizeof(char)*(tempLen + 1));

    /*hash array, fill with 0*/
    int *bin=(int*)calloc(size, sizeof(int));

    temp[tempLen] = '\0';

    /*holding all the strings*/
    char **data = malloc(all*sizeof(char*));
    for(i=0;i<all;i++)
    {
        data[i]=(char*)malloc(tempLen+1);
    }

    index = i*(len-1)+j*(len-1)+k;

    for(i=0;i<len;i++)
    {
        for(j=0;j<len;j++)
        {
            for(k=0;k<len;k++)
            {
                /*temp array with first, second, third characters*/
                temp[0] = str[i];
                temp[1] = str[j];
                temp[2] = str[k];

                strcpy(data[index], temp);
              /*Testing
              printf("%s\n", data[index]); */

            }
        }
    }


    /*get value from hash function and increment bins accordingly*/
    for(i=0;i<all-1;i++)
    {
        h = hash(data[i], size, mult);
        bin[h]++;
    }

    for(i=0;i<size;i++)
    {
        printf("%d, ", bin[i]);
    }

    free(temp);
    for(i=0;i<all;i++)
    {
        free(data[i]);
    }


}



int main()
{
    printf("size = 123, mult = 10: ");
    generateArr(123, 10);
    printf("\n");

    printf("size = 123, mult = 31: ");
    generateArr(123, 31);
    printf("\n");

    printf("size = 124, mult = 10: ");
    generateArr(124, 10);
    printf("\n");

    printf("size = 124, mult = 31: ");
    generateArr(124, 31);
    printf("\n");
}
