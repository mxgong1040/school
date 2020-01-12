/*Michelle Gong
CSE 2320, Homework 05*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glob.h>

/*find min edit distance*/
int min(int x, int y, int z)
{
    int minNum;
	if(x < y && x < z)
	{
		minNum = x;
	}
	else if(y < z)
	{
		minNum = y;
	}
	else
	{
		minNum = z;
	}
	return minNum;
}

int editDistDP(char **str1, char **str2, int m, int n)
{
    int i;
    int j;
    int value;
    int **dp = (int**)malloc(m * sizeof(int *));
    *dp = (int*)malloc(sizeof(int)*m*n);

    for(i = 0; i < m; i++)
    {
        dp[i] = (*dp + n*i);
    }

    if((m!=0)&&(n!=0)) /*neither are empty strings*/
    {
        for(i=0;i<=m;i++)
        {
            for(j=0;j<=n;j++)
            {
                /*char *comp1;
                char *comp2;
                int same;

                comp1 =

                strcpy(comp1, *str1);
                strcpy(comp2, *str2);

                same = strcmp(comp1,comp2);*/

                if(str1[i-1]==str2[j-1])
                {
                    dp[i][j] = dp[i-1][j-1];
                }
                else
                {
                    dp[i][j] = 1 + min(dp[i][j-1], /*ins*/
                                       dp[i-1][j], /*rem*/
                                       dp[i-1][j-1]); /*rep*/

                }

            }
        }
    }

    /*if either first or second string empty, editDistance = size of non-empty string*/
    if(m==0)
    {
        free(*dp);
        free(dp);
        return n;
    }
    else if(n==0)
    {
        free(*dp);
        free(dp);
        return m;
    }

    value = dp[m][n];
    free(*dp);
    free(dp);
    return value;

}

int main(int argc, char* argv[])
{
    glob_t paths;
    int csource;
    int maxDistance;
    int retDistance;
    int counter; /*track matches for realloc*/
    size_t len1;
    size_t len2;
    /*char *str1;*/
    /*char *str2;*/

    char **current; /*current file being compared to rest*/
    char **p;
    char **matches; /*store filenames that match parameters*/
    char ext[100]; /*user-input extension*/
    char search[100];

    matches = NULL;
    maxDistance = atoi(argv[1]);
    counter = 0;

    /*  Find all ".mp3" files in given directory  */
    /*strcat for file extension from command line*/
    strcpy(ext, argv[2]);
    strcpy(search, "*.");
    strcat(search, ext);

    csource = glob(search, 0, NULL, &paths);
    if (csource == 0)
    {
        for (current=paths.gl_pathv; *current!= NULL; ++current)
        {
            len1 = strlen(*current);
            printf("%s\n", *current);
            for (p=current; *p!= NULL; ++p)
            {
                len2 = strlen(*p);
                retDistance = editDistDP(current, p, len1, len2);
                /*printf("%s\n", *p);*/
                if(retDistance<=maxDistance)
                {
                    counter++;
                    /*matches =(char**)realloc(matches, counter* sizeof(char*));
                    strcpy(matches[counter-1], *p);*/
                    printf("    %d, %s\n", retDistance, *p);
                }
            }
        }
        globfree(&paths); /* function that frees the memory of the matching strings */
        free(matches);
    }

    return 0;
}
