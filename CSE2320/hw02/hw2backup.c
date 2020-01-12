/*Michelle Gong
UTA ID 1001671203
CSE 2320, Homework 02*/

/*(f) The overall process is
i. call my function to get an array of 100 randomly generated integers; be sure to use
the arguments given above with the seed value changing each time.
ii. print the first 5 values in the array.
iii. sort the array.
iv. print the number of comparisons for this run.
v. return to step 2(f)i and repeat until 5 different arrays of integers have been generated
and sorted.
vi. after the 5 runs, print the average number of comparisons per run and the predicted
average using the closed-form solution.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hw02-lib.h"

void sort(int *nums);

int main()
{
    int n;
    int lower;
    int upper;
    int seed;
    int i;
    int j;

    n = 100;
    lower = 1;
    upper = 2000;
    seed = 1;

    for(i=0;i<5;i++)
    {
        int *nums=randArray(n, lower, upper, seed);
        sort(nums);
        seed++;
    }

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
    printf("%5d\t", arr[4]);
    /*printf("\n");*/

    return arr;
}

void sort(int *nums)
{
    int i, m, key, j;
    int arr[100];
    int count;
    int n;

    n = 100;

    for(m=0;m<n;m++)
    {
        arr[m]=*(nums+m);
    }

    for (j = 1; j < n; j++)
    {
        key = arr[j];
        i = j - 1;
        count = 0;
        while (j >= 0 && arr[i] > key) {
            arr[i + 1] = arr[i];
            i = i - 1;
        }
        arr[i + 1] = key;
    }
    printf("\tcomparisons = %d\n", count);

    /*[Sort Test]
    for(i=0;i<5;i++)
    {
        printf("(%d),\t", arr[i]);
    }
    printf("\n");*/

}
