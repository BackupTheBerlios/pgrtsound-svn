#include "moduleguiwindow.h"

using namespace std;

// class constructor
ModuleGuiWindow::ModuleGuiWindow() {
	resize(200, get_height() );
}

// class destructor
ModuleGuiWindow::~ModuleGuiWindow() {
	cout << "Zamykam okno GUI" << endl;
}
