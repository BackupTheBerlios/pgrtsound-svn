#ifndef GUIMODULE_H
#define GUIMODULE_H

#include <string>

#include "../algorithm.h"
#include "guiparameterslider.h"

using namespace std;

/**
 * No description
 */
class GuiModule : public Gtk::Frame {
	public:
		GuiModule();
		~GuiModule();
		void AddParameter(Parameter* param);
		void Clear();

	protected:
   		std::vector<Gtk::Widget*>	guiParameters;
		Gtk::VBox box;
};

#endif // GUIMODULE_H
