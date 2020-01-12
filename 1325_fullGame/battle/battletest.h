#ifndef BATTLETEST_H
#define BATTLETEST_H

#include <gtkmm.h>

class Battle_window: public Gtk::Window
{
    public:
        Battle_window();
        virtual ~Battle_window();

    protected:
        //set images/values
        double set_level(int current_level);
        std::string set_character(int j);
        std::string set_portrait(int j);
        std::string set_enemy();
        std::string set_bg(int current_level);

        //widgets
        Gtk::Image bg;
        Gtk::Image character;
        Gtk::Image enemy;
        Gtk::Image portrait;
        Gtk::Layout layout;
        Gtk::ProgressBar enemy_hp, character_hp;
        Gtk::ProgressBar level;
};

#endif
