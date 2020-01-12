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

void bubbleSort(int s, struct Data e[10810]);
void process2(struct Data e2[10810], int z, int length);
void process4();
void process10();
//void mergeAll(int s, std::vector<Data>& e, int l1, int h1, int h2);

int main()
{
    //4 arrays for sorting


    std::string line;
    int i;
    int fileSize = 10811; //lines in file, hardcoded
    int fs=10810; //array length
    double earthquakes[fs];
    double earthquakes2[fs];
    double earthquakes4[fs];
    double earthquakes10[fs];

    //read in file
    std::ifstream datafile("latitude.csv");
    if(datafile.is_open())
    {
        //header line, remove
        std::string headerLine;
        getline(datafile, headerLine);

        //fill array
        for(int i=0; i<fs; i++)
        {
            std::string temp;
            getline(datafile, earthquakes[i].time);

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

    process2(e, 0, fs);

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

    std::cout<<"[4 PROCESSES]"<<std::endl;
    std::cout<<"Sorting start..."<<std::endl;
    process4();

    std::cout<<"[10 PROCESSES]"<<std::endl;
    std::cout<<"Sorting start..."<<std::endl;
    process10();
}

void process2(struct Data e2[10810], int z, int length)
{
    std::cout<<"[2 PROCESSES]"<<std::endl;
    std::cout<<"Sorting start..."<<std::endl;
    clock_t start, finish;
    start = clock();
    //create pipes for parent-child and child-parent communication
    int i, j;
    int s = 10810;
    //track parent
    int pid;

    //create children
    pid_t lchild, rchild;
    lchild = fork();
    if(lchild<0)
    {
        perror("Error, could not create Left Child\n");
        _exit(-1);
    }
    else if(lchild==0)  //2 proc = 0-5405, 5406-10810
    {
        int i, j;
        for(i=0;i<5405;i++)
        {
            for(j=0;j<5405-i;j++)
            {
                if(e2[j].latitude>e2[j+1].latitude)
                {
                    std::swap(e2[j], e2[j+1]);

                }
            }
        }
    }
    else
    {
        rchild = fork();
        if(rchild<0)
        {
            perror("Error, could not create Right Child\n");
            _exit(-1);
        }
        else if(rchild==0)  //2 proc = 0-5405, 5406-10810
        {
            int i, j;
            for(i=5406;i<10810;i++)
            {
                for(j=5406;j<10810-i;j++)
                {
                    if(e2[j].latitude>e2[j+1].latitude)
                    {
                        std::swap(e2[j], e2[j+1]);

                    }
                }
            }
        }
    }

    //merge
    for(i=0;i<s-1;i++)
    {
        for(j=0;j<s-1-i;j++)
        {
            if(e2[j].latitude>e2[j+1].latitude)
            {
                std::swap(e2[j], e2[j+1]);

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
        std::cout<<e2[i].time<<"\t"<<e2[i].latitude<<"\t"<<e2[i].longitude<<std::endl;
    }
    std::cout<<"\n";

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




