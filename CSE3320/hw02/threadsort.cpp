//Michelle Gong
//UTA ID 1001671203
//CSE 3320 Assignment 2 part 1

#include <cstdlib>
#include <pthread.h>
#include <ctime>
#include <sys/time.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "csvparse.h"

/***Function Decs***/
void bubbleSort(int s, struct Data e[]);
void setup(int &num_t); //set up mutex, take user input for num_threads
void new_thread(pthread_t *thread, int num_threads); //create threads
void *multiSort(void *arg); //multi-thread sort using local sort loop
void get_gid(int &_id); //global id for threads
void sort_loop(int data_start, int data_end); //sort loop
void sync_threads(pthread_t *thread, int num_threads); //sync and join
void barrier_func(int num_threads); //barrier function to keep threads in sync, ops in right order

/***Variables***/
int _size = 10810, num_threads, *index;
int counter = 0, gid = 0;
Data* e2; //sorted struct array
pthread_mutex_t mtx_id, mtx_barrier, mtx_print, *mtx_pipe; //mutexes
pthread_cond_t cond_complete, *cond_pipe; //conditional variables

int main()
{
    int fileSize = 10811; //lines in file, hardcoded

    //read in data, fill arrays
    Data* earthquakes = new Data[fileSize];
	e2 = new Data[fileSize];

    std::string line;
    int i;
    int fs = fileSize-1; //account for removed header line for array size

    //read in file
    std::ifstream datafile("earthquakes.csv");
    if(datafile.is_open())
    {
        //header line, remove
        std::string headerLine;
        getline(datafile, headerLine);

        //parse out fields, fill array
        for(int i=0; i<fileSize-1; i++)
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

	//copy data for sorting
    std::copy(earthquakes, earthquakes+fs, e2);

    bubbleSort(fs, earthquakes); //first pass, standard bubblesort

    /***Thread Sort***/

    int num_t;
    pthread_t *thread;
    setup(num_t);
    thread = new pthread_t[num_threads];
    new_thread(thread, num_threads);
    sync_threads(thread, num_threads);
    return 0;
}

void bubbleSort(int s, struct Data e[])
{
    int i, j;

    clock_t start, finish; //timers

    std::cout<<"[NO THREADS]"<<std::endl;
    std::cout<<"Sorting start, please wait..."<<std::endl;
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

}

void setup(int &num_t) //set up mutex, take user input for num_threads
{
    int i;
    pthread_mutex_init(&mtx_id, NULL);
    pthread_mutex_init(&mtx_barrier, NULL);
    pthread_mutex_init(&mtx_print, NULL);
    pthread_cond_init(&cond_complete, NULL);

    std::cout << "Enter number of threads:"<<std::endl;
    std::cin >> num_threads;
    num_t = num_threads;
    std::cout<<"[ " << num_threads << " THREADS ]"<<std::endl;
    std::cout<<"Sorting start, please wait..."<<std::endl;

    //multisort making use of pipeline
    mtx_pipe = new pthread_mutex_t[num_threads];
    cond_pipe = new pthread_cond_t[num_threads];
    index = new int[num_threads]; //keeping track of threads separately
    for(i=0; i<num_threads; i++)
    {
        pthread_mutex_init(&mtx_pipe[i], NULL);
        pthread_cond_init(&cond_pipe[i], NULL);
        index[i] = -1;
    }
}
void new_thread(pthread_t *thread, int num_threads) //create threads
{
    int i;
    for(i=0; i<num_threads; i++)
    {
        if(pthread_create(&thread[i], NULL, multiSort, NULL)!=0)
        {
            std::cout<<"Error, thread could not be created."<<std::endl;
        }
    }
}


void *multiSort(void *arg) //multi-thread sort using local sort loop
{
    int _id, i=0, j;
    get_gid(_id);
    clock_t start, finish;

    start = clock();
    int size_left = _size/num_threads;
    if(_size % num_threads!=0)
    {
        size_left++;
    }
    int data_start = _id*size_left;
    int data_end = std::min(_size-1, (_id+1)*size_left);

    //std::cout<<"Starting multiSort for loop..."<<std::endl;

    for(i=-_id; i<_size+num_threads-_id; i++)
    {
        //std::cout<<"Inside"<<std::endl;
        //std::cout<<num_threads;
        if(i>=0 && i<_size)
        {
            for(j=data_start; j<data_end; j++)
            {
                if(e2[j].latitude > e2[j+1].latitude)
                {
                    std::swap(e2[j], e2[j+1]);
                    //std::cout<<"Sort loop end..."<<std::endl;
                }
            }
            barrier_func(num_threads);
        }
    }

    if(_id == num_threads-1) //sort complete
    {
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
    return NULL;
}
void get_gid(int &_id) //global id for threads, protect with mutex
{
    pthread_mutex_lock(&mtx_id);
    _id = gid;
    gid++;
    pthread_mutex_unlock(&mtx_id);
}

void sort_loop(int data_start, int data_end) //sort loop
{
    int i, j;
    //std::cout<<"Sort loop start..."<<std::endl;

    for(i=data_start; i<data_end; i++)
    {
        if(e2[i].latitude > e2[i+1].latitude)
        {
            std::swap(e2[i], e2[i+1]);
            //std::cout<<"Sort loop end..."<<std::endl;
        }
    }
}
void sync_threads(pthread_t *thread, int num_threads) //sync and join
{
    int i;
    for(i=0; i<num_threads; i++)
    {
        if(pthread_join(thread[i], NULL)!=0)
        {
            std::cout<<"Error, thread could not be joined."<<std::endl;
        }
    }
}

void barrier_func(int num_threads) //barrier function to keep threads in sync, ops in right order
{
    static int counter = 0;
    pthread_mutex_lock(&mtx_barrier);
    counter++;
    if(counter == num_threads) //threads all reached same stage
    {
        pthread_cond_broadcast(&cond_complete); //release condition for all
        counter = 0;
    }
    else
    {
        pthread_cond_wait(&cond_complete, &mtx_barrier); //otherwise keep barrier up, threads can't proceed
    }
    pthread_mutex_unlock(&mtx_barrier);
}

