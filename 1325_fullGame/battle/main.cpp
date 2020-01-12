#include "battle.h"
#include <gtkmm.h>

int main(int argc, char* argv[])
{
    Gtk::Main app(argc, argv);

    Battle_window w;
    Gtk::Main::run(w);
}
