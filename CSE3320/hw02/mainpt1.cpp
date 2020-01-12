//Michelle Gong
//UTA ID 1001671203
//CSE 3320 Assignment 2 part 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <string>
#include <utility>
#include <algorithm>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>


void bubbleSort(double e[], int first, int last);
void merge_arr(double e[], int first, int mid, int last);
void process2(double e[], int first, int last);
void print(double e[]);

int main()
{
    //4 arrays for sorting
    int i;
    int fileSize = 10811; //lines in file, hardcoded
    int fs=10810; //array length
    double earthquakes[fs];
    double earthquakes2[fs];
    double earthquakes4[fs];
    double earthquakes10[fs];

    //read in file
    std::ifstream datafile("latitude.txt");
    if(datafile.is_open())
    {
        //header line, remove
        std::string headerLine;
        getline(datafile, headerLine);

        //fill array
        for(int i=0; i<fs; i++)
        {
            std::string temp;
            getline(datafile, temp);
            earthquakes[i] = atof(temp.c_str());

        }
    }
    datafile.close();

    //copy arrays for sorting
    std::copy(earthquakes, earthquakes+fs, earthquakes2);
    std::copy(earthquakes, earthquakes+fs, earthquakes4);
    std::copy(earthquakes, earthquakes+fs, earthquakes10);

    bubbleSort(earthquakes,0, fileSize); //first pass, standard bubblesort

    /*************Shared Memory**************/

    int shmid;
    key_t key = IPC_PRIVATE;
    int *shm_array;

    int length=128;
    //segment length
    size_t SHM_SIZE = sizeof(int)*length;

    //create segment
    if((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666))<0)
    {
        perror("Error, shmget failed.\n");
        _exit(1);
    }

    //attach segment
    if((shm_array= shmat(shmid, NULL, 0)) == (int *)-1)
    {
        perror("Error, shmat failed.\n");
        _exit(1);
    }

    for(i=0;i<fs;i++)
    {
        earthquakes2[i]=earthquakes[i];
    }

    std::cout<<"[2 PROCESSES]"<<std::endl;
    process2(earthquakes2, 0, fs);
    print(earthquakes2);

    //detach from shared memory
    if(shmdt(earthquakes2)==-1)
    {
        perror("Error, shmdt failed.\n");
        _exit(1);
    }

    //deleted shared segment
    if(shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("Error, shmctl failed.\n");
        _exit(1);
    }

    return 0;


    /*****************************************/





    //processes
    //split arrays
    //2 proc = 0-5405, 5406-10810
    //4 proc = 0-2702, 2703-5405, 5406-8107, 8108-10810
    //10 proc = 1081, 2162, 3243, 4324, 5405, 6486, 7567, 8648, 9729, 10810

}

void bubbleSort(double e[], int first, int last)
{
    int i, j;

    int s = last;

    //standard bubblesort
    clock_t start, finish;
    std::cout<<"[BASE SORT]"<<std::endl;
    std::cout<<"Sorting start..."<<std::endl;
    start=clock();
    for(i=0;i<s-1;i++)
    {
        for(j=0;j<s-1-i;j++)
        {
            if(e[j]>e[j+1])
            {
                std::swap(e[j], e[j+1]);

            }
        }
    }
    finish=clock();
    double time_taken=(double(finish-start))/(CLOCKS_PER_SEC);
    std::cout<<"Sorting complete!"<<std::endl;
    std::cout<<"Elapsed time: "<<time_taken<<" seconds\n"<<std::endl;

    std::cout<<"[Sample Output]"<<std::endl;
    std::cout<<"Latitude"<<std::endl;
    for(i=0;i<15;i++)
    {
        std::cout<<e[i]<<std::endl;
    }

}

void process2(double e[], int first, int last)
{
    int i;
    int len=(last-first+1);

    pid_t child1, child2;
    child1 = fork();
    if(child1<0)
    {
        perror("Error, child1 not created.\n");
        _exit(-1);
    }
    else if(child1==0)
    {
        bubbleSort(e, first, first+len/2-1);
        _exit(0);
    }
    else
    {
        child2=fork();
        if(child2<0)
        {
            perror("Error, child2 not created.\n");
            _exit(-1);
        }
        else if(child2==0)
        {
            bubbleSort(e, first+len/2, last);
            _exit(0);
        }
    }

    int status;

    waitpid(child1, &status, 0);
    waitpid(child2, &status, 0);

    merge_arr(e, first, first+len/2-1, last);

}

void merge_arr(double e[], int first, int mid, int last)
{
    int counter=last-first+1;
    double sorted_arr[counter];
    int i = first;
    int k= mid+1;
    int m= 0;
    while(i<=mid && k<=last)
    {
        if(e[i]<e[k])
        {
            sorted_arr[m++]=e[i++];
        }
        else if(e[k]<e[i])
        {
            sorted_arr[m++]=e[k++];
        }
        else if (e[i]==e[k])
        {
            sorted_arr[m++]=e[i++];
            sorted_arr[m++]=e[k++];
        }
    }

    while(i<=mid)
    {
        sorted_arr[m++]=e[i++];
    }

    while(k<=last)
    {
        sorted_arr[m++]=e[k++];
    }

    int arr_count = first;
    for(i=0;i<counter;i++,first++)
    {
        e[first]=sorted_arr[i];
    }

}

void print(double e[])
{
    std::cout<<"Sample Output:"<<std::endl;
    int i;
    for(i=0;i<15;i++)
    {
        std::cout<<e[i]<<std::endl;
    }
}

