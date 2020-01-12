//Michelle Gong

#include "battletest.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

Battle_window::Battle_window()
{
    set_title("Legend of Mathema");
    this->set_border_width(0);

    //default size + position
    set_default_size(800, 600);

    /*----------------------------------
     Layout widget to allow for overlay
     Hardcoded: current_level
    ----------------------------------*/

    //set up layout
    add(layout);

    //current level
    //used to determine bg, character level bar
    int current_level = 4; //temp hardcode

    //background image
    std::string bg_img = set_bg(current_level);
    bg.set(bg_img);
    layout.put(bg,0,0);


    /*--------------------
           IMAGES
    Hardcoded: c_job
    --------------------*/

    //character
    int c_job = 2; //could be enum, 1=knight, 2=mage, 3=rogue
    std::string c_img = set_character(c_job);
    std::string p_img = set_portrait(c_job);

    character.set(c_img);
    layout.put(character, 590,270);
    portrait.set(p_img);
    layout.put(portrait, 600,10);

    //enemy
    std::string e_img = set_enemy();
    enemy.set("e3.png");
    layout.put(enemy, 100,350);

    /*----------------------------------------------
                     INFO BARS
    Hardcoded:
    chp_part
    ehp_part

    Full HPs should be hardcoded, enemy HP may vary?
    ------------------------------------------------*/

    //character hp
    double chp_full = 10;
    double chp_part = 3; //temp hardcode
    double chp = chp_part/chp_full;
    character_hp.set_text("HP");
    character_hp.set_show_text();
    character_hp.set_fraction(chp);
    layout.put(character_hp, 600, 200);

    //level (aka player's current game level)
    level.set_text("Level");
    level.set_show_text();
    double level_prog = set_level(current_level);
    level.set_fraction(level_prog);
    layout.put(level, 600, 225);

    //enemy hp
    double ehp_full = 10;
    double ehp_part = 5;
    double ehp = ehp_part/ehp_full;
    enemy_hp.set_text("HP");
    enemy_hp.set_show_text();
    enemy_hp.set_fraction(ehp);
    layout.put(enemy_hp, 100, 200);

    show_all();
}

//create virtual window
Battle_window::~Battle_window()
{}


/*-------------------------
 Various setting functions
-------------------------*/

//rename later, maybe not needed actually just put in main function
//set level bar progress
double Battle_window::set_level(int current_level)
{
    double total_level = 4; //should be hardcoded somewhere
    double prog = current_level/total_level;
    return prog;
}

//set bg based on game level
std::string Battle_window::set_bg(int current_level)
{

    switch(current_level)
    {
    case 1: //level 1
        return "battle.png";
        break;
    case 2: //level 2
        return "battle2.png";
        break;
    case 3: //level 3
        return "battle3.png";
        break;
    case 4: //level 4
        return "battle4.png";
        break;
    default:
        return "battle.png";
        break;
    }
}

//set character body image
std::string Battle_window::set_character(int j)
{
    std::string c_img;
    switch(j)
    {
    case 1: //KNIGHT
        c_img = "c1.png";
        return c_img;
        break;
    case 2: //MAGE
        c_img = "c2.png";
        return c_img;
        break;
    case 3: //ROGUE
        c_img = "c3.png";
        return c_img;
        break;
    default:
        c_img = "c1.png";
        return c_img;
        break;
    }
}

//set character portrait
std::string Battle_window::set_portrait(int j)
{
    std::string p_img;
    switch(j)
    {
    case 1: //KNIGHT
        p_img = "p1.png";
        return p_img;
        break;
    case 2: //MAGE
        p_img = "p2.png";
        return p_img;
        break;
    case 3: //ROGUE
        p_img = "p3.png";
        return p_img;
        break;
    default:
        p_img = "p1.png";
        return p_img;
        break;
    }
}

//randomise enemy
std::string Battle_window::set_enemy()
{

    int j = rand() % 4;

    std::string e_img;
    switch(j)
    {
    case 1:
        e_img = "e1.png";
        return e_img;
        break;
    case 2:
        e_img = "e2.png";
        return e_img;
        break;
    case 3:
        e_img = "e3.png";
        return e_img;
        break;
    case 4:
        e_img = "e4.png";
        return e_img;
        break;
    default:
        e_img = "e1.png";
        return e_img;
        break;
    }
}

