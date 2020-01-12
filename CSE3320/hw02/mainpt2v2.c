//Michelle Gong
//UTA ID 1001671203
//CSE 3320 Assignment 2 part 1

//#include <sys/types.h>
//#include <unistd.h>
//#include <sys/wait.h>
#include <iostream>
#include <ctime>
#include <functional>
#include <utility>
#include <vector>
#include <cstdlib>
#include <pthread.h>
#include "csvparse.h"


void bubbleSort(int s, std::vector<Data>& e);


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

    bubbleSort(fileSize, earthquakes); //first pass, standard bubblesort

    //2 thread
    int n = fileSize; //fileSize - 1;
    int i;

    clock_t start, finish;
    std::cout<<"[2 THREADS]"<<std::endl;
    std::cout<<"Sorting start..."<<std::endl;
    /*start=clock();
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
    std::cout<<"\n";*/

    //threads
    //split vectors
    //2 proc = 0-5405, 5406-10810
    //4 proc = 0-2702, 2703-5405, 5406-8107, 8108-10810
    //10 proc = 1081, 2162, 3243, 4324, 5405, 6486, 7567, 8648, 9729, 10810

    return 0;

}

void bubbleSort(int s, std::vector<Data>& e)
{
    int i, j;

    int s = s-1; //account for removed header line

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





