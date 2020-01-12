#include <cstdlib>
#include <pthread.h>
#include <ctime>
#include <sys/time.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "csvparse.h"

using namespace std;

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
void Swap(struct Data &a, struct Data &b);

// Loops through a local array once and swaps any adjacent elements
// that are not in the correct order.
void Local_loop(int my_start, int my_end, struct Data *e2);

// Multi-threaded bubble sort implemented as a pipeline. Each thread
// handles an equal portion of the array with an overlap of 1 element
// between each adjacent threads. In the first iteration, only the
// thread 0 loops over its array once. In the second iteration, both
// thread 0 and thread 1 can do their local loop and so on. A barrier
// insures that the operations are done in the corrcet order.
void *Parallel_bubble_sort(void *arg);

// Outputs all the elements in the array to the console.
void  Output_array();

void bubbleSort(int s, struct Data e2[100]);

// global variables
int size, limit=10, no_threads, *index;
int counter = 0, global_id = 0, threads_done=0, global_sum = 0;;
int *result=NULL;
Data* e2;
pthread_mutex_t id_mutex, barrier_mutex, cout_mutex, *pipe_mutex;
pthread_cond_t allin_cond, *pipe_cond;




int main() 
{
	    //4 arrays for sorting
    Data* earthquakes = new Data[100];
	e2 = new Data[100];


    std::string line;
    int i;
    int fileSize = 101; //lines in file, hardcoded
    int fs=100; //array length

    //read in file
    std::ifstream datafile("earthquakes.csv");
    if(datafile.is_open())
    {
        //header line, remove
        std::string headerLine;
        getline(datafile, headerLine);

        //parse out fields, fill array
        for(int i=0; i<100; i++)
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
	   


    bubbleSort(fileSize, earthquakes); //first pass, standard bubblesort
	
    int nthreads;
    pthread_t *threads;
    srand(time(NULL));
    Init_data(nthreads);
    threads = new pthread_t[nthreads];
    Create_threads(threads, nthreads);
    Synchronise(threads, nthreads);
    return 0;
}

void bubbleSort(int s, struct Data e[100])
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

}

// Inputs the number of ships and initializes the global variables.
void Init_data(int &nthreads)
{
    int i;
    pthread_mutex_init(&id_mutex, NULL);
    pthread_mutex_init(&barrier_mutex, NULL);
    pthread_mutex_init(&cout_mutex, NULL);
    pthread_cond_init(&allin_cond, NULL);
    /*cout << "Enter the size of the array" << endl;
    cin >> size;
    cout << "Enter the upper limit for the elements of the array" << endl;
    cin >> limit;
    a = new int[size];
    for (i=0; i<size; i++) {
        a[i] = rand() % limit;
        cout << a[i] << ' ';
    }*/
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
void Swap(struct Data &a, struct Data &b)
{
    Data temp;
	temp = a;
	a = b;
    b = temp;
}

// Loops through a local array once and swaps any adjacent elements
// that are not in the correct order.
void Local_loop(int my_start, int my_end, struct Data *e2)
{
    int k;
	int j;
	
	for (j=my_start; j< my_end; j++){
		if (e2[j].latitude > e2[j+1].latitude)
		{
			std::swap(e2[j], e2[j+1]);
		}
	

	}
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
            Local_loop(my_start, my_end, e2);
        Barrier(no_threads);
    }
    if (id == no_threads-1) {
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

/*// Outputs all the elements in the array to the console.
void Output_array()
{
    for (int i=0; i<size; i++)
        cout << a[i] << ' ';
    cout << endl;
}*/

