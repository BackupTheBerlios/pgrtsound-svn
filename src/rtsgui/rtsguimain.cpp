#include <cstdlib>
#include <iostream>

#include <gtkmm/main.h>

#include "rtmainwindow.h"

// logowanie do pliku
ofstream logfile("RTSGuilog.txt");
streambuf* out = cout.rdbuf(logfile.rdbuf());

using namespace std;

int main(int argc, char *argv[])
{
	Gtk::Main kit(argc, argv, false); // 'false' aby nie zmieniac locale!!!!

	RTMainWindow rtWindow;

	rtWindow.show_all_children();

 	Gtk::Main::run(rtWindow); //Shows the window and returns when it is closed.

    return EXIT_SUCCESS;
}
