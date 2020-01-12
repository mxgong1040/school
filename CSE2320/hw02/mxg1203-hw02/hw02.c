/*Michelle Gong
CSE 2320, Homework 02*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hw02-lib.h"

int sort(int *nums);

int main()
{
    int n;
    int lower;
    int upper;
    int seed;
    int i;
    int j;
    int total_comps;
    float avg_comps;
    float predicted;

    n = 100;
    lower = 1;
    upper = 2000;
    seed = 1;
    total_comps = 0;

    for(i=0;i<5;i++)
    {
        int *nums=randArray(n, lower, upper, seed);
        total_comps = sort(nums) + total_comps;
        seed++;
    }

    /*printf("Total Comps: %d", total_comps);*/

    avg_comps = (total_comps)/5;
    predicted = ((n*n)+(3*n)-4)/4;

    printf("\n");
    printf("avg number of experiments = %.1f\n", avg_comps);
    printf("        predicted average = %.1f\n", predicted);


    return 0;
}


int* randArray(int n, int lower, int upper, int seed)
{
    int num;
    int i;
    static int arr[100]; /*allocate to heap*/
    srand(seed);
    /* [Value Test]
    printf("seed = %d\n", seed);
    printf("lower = %d\n", lower);
    printf("upper = %d\n", upper);
    printf("n = %d\n", n);*/

    for(i=0;i<n;i++)
    {
        num = (rand() % (upper - lower) + lower);
        arr[i] = num;
    }

    printf("%4d,", arr[0]);

    for(i=1;i<4;i++)
    {
        printf("%5d,", arr[i]);
    }
    printf("%5d", arr[4]);
    /*printf("\n");*/

    return arr;
}

int sort(int *nums)
{
    int i, m, key, j;
    int arr[100];
    int count, comps;
    int n;

    n = 100;

    for(m=0;m<n;m++)
    {
        arr[m]=*(nums+m);
    }

    comps = 0;
    for (j = 1; j < n; j++)
    {
        key = arr[j];
        i = j - 1;
        while (i >= 0 && arr[i] > key)
        {
            arr[i + 1] = arr[i];
            i = i - 1;
            comps++; /*loop true*/
        }
        arr[i+1] = key;
        comps++; /*loop false*/
    }
    printf("    comparisons = %d\n", comps);

    /*[Sort Test]
    for(i=0;i<5;i++)
    {
        printf("(%d),\t", arr[i]);
    }
    printf("\n");*/

    return comps;

}
