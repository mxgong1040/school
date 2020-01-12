#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include "csvparse.h"

int main()
{
    std::vector<Data> earthquakes;
    std::string line;
    Data d;

    //read in file
    std::ifstream datafile("earthquakes.csv");
    if(datafile.is_open())
    {
        //header line
        std::string headerLine;
        getline(datafile, headerLine);
        int fileSize = 0;
        while(datafile.good())
        {
            std::string temp;
            getline(datafile, d.time, ',');
            getline(datafile, temp, ',');
            d.latitude = atof(temp.c_str());
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

            earthquakes.push_back(d);
        }
    }
    datafile.close();

    int filesize = 10811;
    int i, j, key;
    std::vector<Data> sorted;

    //insertion sort
    for(i=1;i<12;i++)
    {
        key=earthquakes[i].latitude;
        j=i-1;
        while(j>=0 && earthquakes[j].latitude > key)
        {
            earthquakes[j+1] = earthquakes[j];
            j=j-1;
        }
        earthquakes[j+1].latitude = key;
    }

    /*for(i=1;i<12;i++)
    {
        std::cout<<earthquakes[i].time<<"\t"<<earthquakes[i].latitude<<"\t";
        std::cout<<earthquakes[i].longitude<<"\t"<<earthquakes[i].depth<<std::endl;
    }*/


    /*for(i=0;i<filesize-1;i++)
    {
        std::cout<<earthquakes[i].latitude<<"\n";
    }*/
}
