/**************************************************************
  File:          sort_pipeline.cc
  Description:   A program that implements a sorting function
                 for an array using a pipeline model. 
                                                         
  Author:        Dana Vrajitoru                          
  Organization:  IUSB, Computer and Information Sciences
  Date:          October 2018
***************************************************************/

#include <cstdlib>
#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <utility>
using namespace std;

// Creates the threads calling the Ship_check_in function.
void Create_threads(pthread_t *threads, int no_threads);

// Joins all of the threads so that the program doesn;t end before
// they have finished the work.
void Synchronise(pthread_t *threads, int no_threads);

// Inputs the number of ships and initializes the global variables.
void Init_data(int &nthreads);

// A barrier function based on the number of threads implemented using
// a conditional variable. It uses a global counter intialized as
// 0. Each thread coming in increments the counter. If the count is
// not equal to the number of threads, it goes into a wait on the
// conditional variable. The thread that finds the counter equal to
// the number of threads broadcasts the condition that releases all of
// them.
void Barrier(int no_threads) ;

// Copies the value of a global id into the reference parameter and
// increments the global id.
void Get_id(int &id);

// Waits for a given delay specified in miliseconds.
void Wait(int miliseconds);

// Swaps two integer variables.
void Swap(int &a, int &b);

// Loops through a local array once and swaps any adjacent elements
// that are not in the correct order.
void Local_loop(int my_start, int my_end);

// Multi-threaded bubble sort implemented as a pipeline. Each thread
// handles an equal portion of the array with an overlap of 1 element
// between each adjacent threads. In the first iteration, only the
// thread 0 loops over its array once. In the second iteration, both
// thread 0 and thread 1 can do their local loop and so on. A barrier
// insures that the operations are done in the corrcet order.
void *Parallel_bubble_sort(void *arg);

// Outputs all the elements in the array to the console.
void  Output_array();

// global variables 
int size, limit=10, no_threads, *index; 
int counter = 0, global_id = 0, threads_done=0, global_sum = 0;; 
int *a = NULL, *result=NULL;
pthread_mutex_t id_mutex, barrier_mutex, cout_mutex, *pipe_mutex;
pthread_cond_t allin_cond, *pipe_cond;

// Creates the threads calling the Ship_check_in function.
void Create_threads(pthread_t *threads, int no_threads)
{
    for (int i=0; i<no_threads; i++)
        if (pthread_create(&threads[i], NULL, Parallel_bubble_sort, NULL) != 0)
            cout << "Pthread_create failed" << endl;
}

// Joins all of the threads so that the program doesn't end before
// they have finished the work.
void Synchronise(pthread_t *threads, int no_threads)
{
    for (int i=0; i<no_threads; i++)
        if (pthread_join(threads[i], NULL) != 0)
            cout << "Pthread_join failed" << endl;
}

// Inputs the number of ships and initializes the global variables.
void Init_data(int &nthreads)
{
    int i;
    pthread_mutex_init(&id_mutex, NULL);
    pthread_mutex_init(&barrier_mutex, NULL);
    pthread_mutex_init(&cout_mutex, NULL);
    pthread_cond_init(&allin_cond, NULL);

	std::string line;
    int i;
    int fileSize = 10811; //lines in file, hardcoded
    int fs=10810; //array length
	
	a = new int[fs];
	arr = new int[fs];

    //read in file
    std::ifstream datafile("test.csv");
    if(datafile.is_open())
    {
        //header line, remove
        std::string headerLine;
        getline(datafile, headerLine);

        //parse out fields, fill array
        for(int i=0; i<10810; i++)
        {
            std::string temp;
            getline(datafile, arr[i]);
        }
    }
    datafile.close();
	
	std::copy(arr, arr+fs, a);
	
    /*cout << "Enter the size of the array" << endl;
    cin >> size;
    cout << "Enter the upper limit for the elements of the array" << endl;
    cin >> limit;
    a = new int[size];
    for (i=0; i<size; i++) {
        a[i] = rand() % limit;
        cout << a[i] << ' ';
    }*/
	
	int s = 10810;
	
	clock_t start, finish;
    std::cout<<"[NO THREADS]"<<std::endl;
    std::cout<<"Sorting start..."<<std::endl;
    start=clock();
    for(i=0;i<s-1;i++)
    {
        for(j=0;j<s-1-i;j++)
        {
            if(arr[j]>arr[j+1])
            {
                std::swap(arr[j], arr[j+1]);

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
        std::cout<<arr[i]<<std::endl;
    }
	
    cout << endl;
    cout << "Enter the number of threads" << endl;
    cin >> no_threads;
    nthreads = no_threads;
    result = new int[no_threads];
    pipe_mutex = new pthread_mutex_t[no_threads];
    pipe_cond = new pthread_cond_t[no_threads];
    index = new int[no_threads];
    for (i=0; i<no_threads; i++) {
        pthread_mutex_init(&pipe_mutex[i], NULL);
        pthread_cond_init(&pipe_cond[i], NULL);
        index[i] = -1;
    }
}

// A barrier function based on the number of threads implemented using
// a conditional variable. It uses a global counter intialized as
// 0. Each thread coming in increments the counter. If the count is
// not equal to the number of threads, it goes into a wait on the
// conditional variable. The thread that finds the counter equal to
// the number of threads broadcasts the condition that releases all of
// them.
void Barrier(int no_threads) 
{
    static int count = 0;
    pthread_mutex_lock(&barrier_mutex);
    count++;
    if (count == no_threads) {
        pthread_cond_broadcast(&allin_cond); // last one in lets everyone go
        count = 0;
    }
    else
        pthread_cond_wait(&allin_cond, &barrier_mutex); 
    pthread_mutex_unlock(&barrier_mutex);
}

// Copies the value of a global id into the reference parameter and
// increments the global id.
void Get_id(int &id)
{
    pthread_mutex_lock(&id_mutex);
    id = global_id;
    global_id++;
    pthread_mutex_unlock(&id_mutex);
}

// Waits for a given delay specified in miliseconds.
void Wait(int delay)
{
    struct timeval before, after;
    int timing = 0;
    gettimeofday(&before, 0);
    while (timing < delay) {
        gettimeofday(&after, 0);
        // Add the difference in seconds * 1000 and difference 
        // in microseconds divided by 1000
        timing = (after.tv_sec - before.tv_sec)*1000 +
            (after.tv_usec - before.tv_usec)/1000;
    }
}

// Swaps two integer variables.
void Swap(int &a, int &b) 
{
    int temp = a;
    a = b;
    b = temp;
}

// Loops through a local array once and swaps any adjacent elements
// that are not in the correct order.
void Local_loop(int my_start, int my_end)
{
    for (int j=my_start; j< my_end; j++) 
        if (a[j] > a[j+1])
            Swap(a[j], a[j+1]);
}

// Multi-threaded bubble sort implemented as a pipeline. Each thread
// handles an equal portion of the array with an overlap of 1 element
// between each adjacent threads. In the first iteration, only the
// thread 0 loops over its array once. In the second iteration, both
// thread 0 and thread 1 can do their local loop and so on. A barrier
// insures that the operations are done in the corrcet order.
void *Parallel_bubble_sort(void *arg)
{
    int id, i;
    Get_id(id);
    
    int lsize = size/no_threads;
    if (size % no_threads != 0)
        lsize++;
    int my_start = id*lsize;
    int my_end = min(size-1, (id+1)*lsize);
    
    // i starts with a negative value to delay when each thread starts
    // their own local loop. The upper bound is set upi such that all
    // threads will keep coming into the barrier until all of them have
    // finished doing the loop, to avoid a deadlock.
    for (i=-id; i<size+no_threads-id; i++) {
        if (i >= 0 && i<size) 
            Local_loop(my_start, my_end);
        Barrier(no_threads);
    } 
    if (id == no_threads-1) {
        cout << "Here is the sorted array:" << endl;
        Output_array();
    }
    return NULL;
}

// Outputs all the elements in the array to the console.
void Output_array()
{
    for (int i=0; i<size; i++) 
        cout << a[i] << ' ';
    cout << endl;
}

int main() 
{
    int nthreads;
    pthread_t *threads;
    //srand(time(NULL));
    Init_data(nthreads);
    threads = new pthread_t[nthreads];
    Create_threads(threads, nthreads);
    Synchronise(threads, nthreads);
    return 0;
}