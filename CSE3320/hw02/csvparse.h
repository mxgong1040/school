//Michelle Gong
//UTA ID 1001671203
//CSE 3320 Assignment 2 part 1

#ifndef CSVPARSE_H
#define CSVPARSE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>

struct Data
{
    //don't need to bother with atof() for anything
    //that's not directly being used for sort
    std::string time;
    double latitude;
    std::string longitude;
    std::string depth;
    std::string mag;
    std::string magType;
    std::string nst;
    std::string gap;
    std::string dmin;
    std::string rms;
    std::string net;
    std::string id;
    std::string updated;
    std::string place;
    std::string type;
    std::string horizontal;
    std::string depthError;
    std::string magError;
    std::string magNst;
    std::string status;
    std::string locationSource;
    std::string magSource;
};


#endif // CSVPARSE_H
