#include <cstdlib>
#include <iostream>

#include <gtkmm/main.h>

#include "guimainwindow.h"

// logowanie do pliku
//ofstream logfile("RTSGui2log.txt");
//streambuf* out = cout.rdbuf(logfile.rdbuf());

using namespace std;

int main(int argc, char *argv[])
{
	Gtk::Main kit(argc, argv, false); // 'false' aby nie zmieniac locale!!!!

	GuiMainWindow window;

	window.show_all_children();
 	Gtk::Main::run(window); //Shows the window and returns when it is closed.

	//system("pause");
    return EXIT_SUCCESS;
}
