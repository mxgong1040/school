//Michelle Gong
//UTA ID 1001671203
//CSE 3320 Assignment 2 part 1

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <algorithm>
#include "csvparse.h"

void bubbleSort(int s, struct Data e[10810]);
void process2();
void process4();
void process10();
//void mergeAll(int s, std::vector<Data>& e, int l1, int h1, int h2);

int main()
{
    //4 arrays for sorting
    Data* earthquakes = new Data[10810];
    Data* earthquakes2 = new Data[10810];
    Data* earthquakes4 = new Data[10810];
    Data* earthquakes10 = new Data[10810];

    std::string line;
    int i;
    int fileSize = 10811; //lines in file, hardcoded
    int fs=10810; //array length

    //read in file
    std::ifstream datafile("earthquakes.csv");
    if(datafile.is_open())
    {
        //header line, remove
        std::string headerLine;
        getline(datafile, headerLine);

        //parse out fields, fill array
        for(int i=0; i<10810; i++)
        {
            std::string temp;
            getline(datafile, earthquakes[i].time, ',');
            getline(datafile, temp, ',');
            earthquakes[i].latitude = atof(temp.c_str()); //convert to double
            getline(datafile, earthquakes[i].longitude, ',');
            getline(datafile, earthquakes[i].depth, ',');
            getline(datafile, earthquakes[i].mag, ',');
            getline(datafile, earthquakes[i].magType, ',');
            getline(datafile, earthquakes[i].nst, ',');
            getline(datafile, earthquakes[i].dmin, ',');
            getline(datafile, earthquakes[i].rms, ',');
            getline(datafile, earthquakes[i].net, ',');
            getline(datafile, earthquakes[i].id, ',');
            getline(datafile, earthquakes[i].updated, ',');
            getline(datafile, earthquakes[i].place, ',');
            getline(datafile, earthquakes[i].type, ',');
            getline(datafile, earthquakes[i].horizontal, ',');
            getline(datafile, earthquakes[i].depthError, ',');
            getline(datafile, earthquakes[i].magError, ',');
            getline(datafile, earthquakes[i].magNst, ',');
            getline(datafile, earthquakes[i].status, ',');
            getline(datafile, earthquakes[i].locationSource, ',');
            getline(datafile, earthquakes[i].magSource);
        }
    }
    datafile.close();

    //copy arrays for sorting
    std::copy(earthquakes, earthquakes+fs, earthquakes2);
    std::copy(earthquakes, earthquakes+fs, earthquakes4);
    std::copy(earthquakes, earthquakes+fs, earthquakes10);

    bubbleSort(fileSize, earthquakes); //first pass, standard bubblesort

    /*************Shared Memory**************/

    int shmid;
    key_t key = IPC_PRIVATE;
    Data *e;

    //fs=array length

    //mem segment length
    size_t SHM_SIZE = sizeof(Data)*fs;

    //create mem segment
    if((shmid = shmget(key,SHM_SIZE,IPC_CREAT | IPC_EXCL | 0666))==1)
    {
        perror("Error, shmget");
        _exit(1);
    }

    //attach segment to data space
    e = (Data *) shmat(shmid, 0,0);
    if(e == (Data *)(-1))
    {
        perror("shmat");
        _exit(1);
    }

    int j;
    for(j=0;j<fs;j++)
    {
        e[i]=earthquakes2[i];
    }

    /*****************************************/





    //processes
    //split arrays
    //2 proc = 0-5405, 5406-10810
    //4 proc = 0-2702, 2703-5405, 5406-8107, 8108-10810
    //10 proc = 1081, 2162, 3243, 4324, 5405, 6486, 7567, 8648, 9729, 10810

}

void bubbleSort(int s, struct Data e[10810])
{
    int i, j;


    s=s-1; //account for removed header line

    clock_t start, finish;
    std::cout<<"[1 PROCESS]"<<std::endl;
    std::cout<<"Sorting start..."<<std::endl;
    start=clock();
    for(i=0;i<s-1;i++)
    {
        for(j=0;j<s-1-i;j++)
        {
            if(e[j].latitude>e[j+1].latitude)
            {
                std::swap(e[j], e[j+1]);

            }
        }
        //std::cout<<e[j].time<<"\t"<<e[j].latitude<<"\t"<<e[j].longitude<<std::endl;
    }
    finish=clock();
    double time_taken=(double(finish-start))/(CLOCKS_PER_SEC);
    std::cout<<"Sorting complete!"<<std::endl;
    std::cout<<"Elapsed time: "<<time_taken<<" seconds\n"<<std::endl;

    std::cout<<"[Sample Output]"<<std::endl;
    std::cout<<"Time\t\t\t\tLatitude\tLongitude"<<std::endl;
    for(i=0;i<15;i++)
    {
        std::cout<<e[i].time<<"\t"<<e[i].latitude<<"\t"<<e[i].longitude<<std::endl;
    }
    std::cout<<"\n";

    std::cout<<"[2 PROCESSES]"<<std::endl;
    std::cout<<"Sorting start..."<<std::endl;
    process2();

    std::cout<<"[4 PROCESSES]"<<std::endl;
    std::cout<<"Sorting start..."<<std::endl;
    process4();

    std::cout<<"[10 PROCESSES]"<<std::endl;
    std::cout<<"Sorting start..."<<std::endl;
    process10();
}

void process2()
{
    //create pipes for parent-child and child-parent communication

    //track parent
    int pid;

    //create children

    clock_t start, finish;
    start=clock();
    finish=clock();
    double time_taken=(double(finish-start))/(CLOCKS_PER_SEC);
    std::cout<<"Sorting complete!"<<std::endl;
    std::cout<<"Elapsed time: "<<time_taken<<" seconds\n"<<std::endl;
    std::cout<<"[Sample Output]"<<std::endl;
    std::cout<<"Time\t\t\t\tLatitude\tLongitude"<<std::endl;

}

void process4()
{
    clock_t start, finish;
    start=clock();
    finish=clock();
    double time_taken=(double(finish-start))/(CLOCKS_PER_SEC);
    std::cout<<"Sorting complete!"<<std::endl;
    std::cout<<"Elapsed time: "<<time_taken<<" seconds\n"<<std::endl;
    std::cout<<"[Sample Output]"<<std::endl;
    std::cout<<"Time\t\t\t\tLatitude\tLongitude"<<std::endl;

}

void process10()
{
    clock_t start, finish;
    start=clock();
    finish=clock();
    double time_taken=(double(finish-start))/(CLOCKS_PER_SEC);
    std::cout<<"Sorting complete!"<<std::endl;
    std::cout<<"Elapsed time: "<<time_taken<<" seconds\n"<<std::endl;
    std::cout<<"[Sample Output]"<<std::endl;
    std::cout<<"Time\t\t\t\tLatitude\tLongitude"<<std::endl;

}




