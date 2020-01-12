//Michelle Gong
//UTA ID 1001671203
//CSE 3320 Assignment 2 part 1

//#include <sys/types.h>
//#include <unistd.h>
//#include <sys/wait.h>
#include <iostream>
#include <ctime>
#include <thread>
#include <functional>
#include <utility>
#include <vector>
#include "csvparse.h"

typedef void (*mergeSort_ptr_t)(std::vector<Data> &v, int l, int r); //passing arguments
void bubbleSort(int first, int last, std::vector<Data>& e);
void mergeData(std::vector<Data> &v, int l, int m, int r);
void mergeSort(std::vector<Data> &v, int l, int r);

/*void thread2();
void thread4();
void thread10();*/

//void mergeAll(int s, std::vector<Data>& e, int l1, int h1, int h2);

int main()
{
    //4 vectors for sorting
    std::vector<Data> earthquakes;
    std::vector<Data> earthquakes2;
    std::vector<Data> earthquakes4;
    std::vector<Data> earthquakes10;

    std::string line;
    Data d;
    int fileSize = 10811; //lines in file, hardcoded

    //read in file
    std::ifstream datafile("earthquakes.csv");
    if(datafile.is_open())
    {
        //header line, remove
        std::string headerLine;
        getline(datafile, headerLine);

        //parse out fields
        while(datafile.good())
        {
            std::string temp;
            getline(datafile, d.time, ',');
            getline(datafile, temp, ',');
            d.latitude = atof(temp.c_str()); //convert to double
            getline(datafile, d.longitude, ',');
            getline(datafile, d.depth, ',');
            getline(datafile, d.mag, ',');
            getline(datafile, d.magType, ',');
            getline(datafile, d.nst, ',');
            getline(datafile, d.dmin, ',');
            getline(datafile, d.rms, ',');
            getline(datafile, d.net, ',');
            getline(datafile, d.id, ',');
            getline(datafile, d.updated, ',');
            getline(datafile, d.place, ',');
            getline(datafile, d.type, ',');
            getline(datafile, d.horizontal, ',');
            getline(datafile, d.depthError, ',');
            getline(datafile, d.magError, ',');
            getline(datafile, d.magNst, ',');
            getline(datafile, d.status, ',');
            getline(datafile, d.locationSource, ',');
            getline(datafile, d.magSource);

            //fill vectors
            earthquakes.push_back(d);
            earthquakes2.push_back(d);
            earthquakes4.push_back(d);
            earthquakes10.push_back(d);
        }
    }
    datafile.close();

    bubbleSort(0, fileSize-1, earthquakes); //first pass, standard bubblesort

    //2 thread
    int n = fileSize; //fileSize - 1;
    int i;

    clock_t start, finish;
    std::cout<<"[2 THREADS]"<<std::endl;
    std::cout<<"Sorting start..."<<std::endl;
    start=clock();

    mergeSort(earthquakes2, 0, 700);
    //std::thread e2()
    finish=clock();
    double time_taken=(double(finish-start))/(CLOCKS_PER_SEC);
    std::cout<<"Sorting complete!"<<std::endl;
    std::cout<<"Elapsed time: "<<time_taken<<" seconds\n"<<std::endl;

    std::cout<<"[Sample Output]"<<std::endl;
    std::cout<<"Time\t\t\t\tLatitude\tLongitude"<<std::endl;
    for(i=0;i<15;i++)
    {
        std::cout<<earthquakes2[i].time<<"\t"<<earthquakes2[i].latitude<<"\t"<<earthquakes2[i].longitude<<std::endl;
    }
    std::cout<<"\n";

    //threads
    //split vectors
    //2 proc = 0-5405, 5406-10810
    //4 proc = 0-2702, 2703-5405, 5406-8107, 8108-10810
    //10 proc = 1081, 2162, 3243, 4324, 5405, 6486, 7567, 8648, 9729, 10810

    return 0;

}

void bubbleSort(int first, int last, std::vector<Data>& e)
{
    int i, j;

    int s = last; //account for removed header line

    clock_t start, finish;
    std::cout<<"[NO THREADS]"<<std::endl;
    std::cout<<"Sorting start..."<<std::endl;
    start=clock();
    for(i=first;i<s-1;i++)
    {
        for(j=first;j<s-1-i;j++)
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

    /*std::cout<<"[2 THREADS]"<<std::endl;
    std::cout<<"Sorting start..."<<std::endl;
    thread2();

    std::cout<<"[4 THREADS]"<<std::endl;
    std::cout<<"Sorting start..."<<std::endl;
    thread4();

    std::cout<<"[10 THREADS]"<<std::endl;
    std::cout<<"Sorting start..."<<std::endl;
    thread10();*/
}

void mergeData(std::vector<Data> &v, int l, int m, int r)
{
    //std::cout<<"Start of mergeData"<<std::endl;
    int i = l;
    int j = m + 1;
    std::vector<Data> temp;

    //std::cout<<"while (i <= m && j <= r)"<<std::endl;
    while (i <= m && j <= r)
     {
        if (v[i].latitude <= v[j].latitude)
        {
            temp.push_back(v[i++]);
        }
        else {
            temp.push_back(v[j++]);
        }
    }

    //std::cout<<"while (i <= m)"<<std::endl;
    while (i <= m) {
        temp.push_back(v[i++]);
    }
    while (j <= r) {
        temp.push_back(v[j++]);
    }
    std::copy(temp.begin(), temp.end(), v.begin() + l);
}

void mergeSort(std::vector<Data> &v, int l, int r)
{
    //std::cout<<"Start of mergeSort"<<std::endl;
    if (l < r)
    {
        int m = (l + r) / 2;
        std::thread sort_thread1((mergeSort_ptr_t)(mergeSort), std::ref(v), l, m);
        std::thread sort_thread2((mergeSort_ptr_t)(mergeSort), std::ref(v), m + 1, r);
        sort_thread1.join();
        sort_thread2.join();
        mergeData(v, l, m, r);
    }
}

/*void thread2()
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

void thread4()
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

void thread10()
{
    clock_t start, finish;
    start=clock();
    finish=clock();
    double time_taken=(double(finish-start))/(CLOCKS_PER_SEC);
    std::cout<<"Sorting complete!"<<std::endl;
    std::cout<<"Elapsed time: "<<time_taken<<" seconds\n"<<std::endl;
    std::cout<<"[Sample Output]"<<std::endl;
    std::cout<<"Time\t\t\t\tLatitude\tLongitude"<<std::endl;

}*/




