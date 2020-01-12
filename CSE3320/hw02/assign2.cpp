//Michelle Gong
//UTA ID 1001671203
//CSE 3320 Assignment 2 part 1

#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdio>
#include <ctime>
#include <algorithm>
#include "csvparse.h"

pthread_mutex_t id_mutex, barrier_mutex, cout_mutex, *pipe_mutex;
pthread_cond_t allin_cond, *pipe_cond;
int gid = 0, gsum = 0; //global id
int complete_threads = 0;
Data *e2;
int *result=NULL;
int *index;

void bubbleSort(int s, struct Data e[10810]);

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
}

void bubbleSort(int s, struct Data e[10810])
{
    int i, j;


    s=s-1; //account for removed header line

    clock_t start, finish;
    std::cout<<"[NO THREADS]"<<std::endl;
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


    std::cout<<"[4 PROCESSES]"<<std::endl;
    std::cout<<"Sorting start..."<<std::endl;


    std::cout<<"[10 PROCESSES]"<<std::endl;
    std::cout<<"Sorting start..."<<std::endl;

}
