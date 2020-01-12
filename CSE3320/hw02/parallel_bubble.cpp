#include <cstdlib>
#include <pthread.h>
#include <ctime>
//#include <sys/time.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void Create_threads(pthread_t *threads, int no_threads);

void Synchronise(pthread_t *threads, int no_threads);

void Init_data(int &nthreads);

void Barrier(int no_threads) ;

void Get_id(int &id);

void Local_loop(int my_start, int my_end);

void *Parallel_bubble_sort(void *arg);

// global variables
int size=12000, limit=10, no_threads, *index;
int counter = 0, global_id = 0;// threads_done=0, global_sum = 0;;
int *a = NULL;// *result=NULL;
pthread_mutex_t id_mutex, barrier_mutex, cout_mutex, *pipe_mutex;
pthread_cond_t allin_cond, *pipe_cond;


void Create_threads(pthread_t *threads, int no_threads)
{
    for (int i=0; i<no_threads; i++)
        if (pthread_create(&threads[i], NULL, Parallel_bubble_sort, NULL) != 0)
            cout << "Pthread_create failed" << endl;
}

void Synchronise(pthread_t *threads, int no_threads)
{
    for (int i=0; i<no_threads; i++)
        if (pthread_join(threads[i], NULL) != 0)
            cout << "Pthread_join failed" << endl;
}


void Init_data(int &nthreads)
{
    int i;
    pthread_mutex_init(&id_mutex, NULL);
    pthread_mutex_init(&barrier_mutex, NULL);
    pthread_mutex_init(&cout_mutex, NULL);
    pthread_cond_init(&allin_cond, NULL);
    cout << "Enter the number of threads" << endl;
    cin >> no_threads;
    nthreads = no_threads;
    //result = new int[no_threads];
    pipe_mutex = new pthread_mutex_t[no_threads];
    pipe_cond = new pthread_cond_t[no_threads];
    index = new int[no_threads];
    for (i=0; i<no_threads; i++) {
        pthread_mutex_init(&pipe_mutex[i], NULL);
        pthread_cond_init(&pipe_cond[i], NULL);
        index[i] = -1;
    }
}


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

void Get_id(int &id)
{
    pthread_mutex_lock(&id_mutex);
    id = global_id;
    global_id++;
    pthread_mutex_unlock(&id_mutex);
}


void Local_loop(int my_start, int my_end)
{
    int k;
    for (int j=my_start; j< my_end; j++)
        if (a[j] > a[j+1])
            std::swap(a[j], a[j+1]);
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
    for (int i=0; i<15; i++)
        cout << a[i] << endl;
    cout << endl;
    }
    return NULL;
}

// Outputs all the elements in the array to the console.

int main()
{
    int nthreads;
    pthread_t *threads;
    srand(time(NULL));
    int i;
    //cout << "Enter the size of the array" << endl;
    //cin >> size;
    cout << "Enter the upper limit for the elements of the array" << endl;
    cin >> limit;
    a = new int[size];
    for (i=0; i<size; i++) {
        a[i] = rand() % limit;
    }

    for (int i=0; i<15; i++){
        cout << a[i] << endl;
    }
    cout << endl;
    Init_data(nthreads);
    threads = new pthread_t[nthreads];
    Create_threads(threads, nthreads);
    Synchronise(threads, nthreads);
    return 0;
}
