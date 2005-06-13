#ifndef MODULEGUI_H
#define MODULEGUI_H

#include <gtkmm.h>
#include "guislider.h"

/**
 * Klasa graficznych interfejsow modulow.
 * Minimalny wrapper Gtk::Frame. Byc moze ulatwi zmiane interfejsow na jakies
 * inne, np. wykorzystujace biblioteke Qt.
 */
class ModuleGui : public Gtk::Frame
{
	public:
		ModuleGui();
		~ModuleGui();
};

#endif // MODULEGUI_H
