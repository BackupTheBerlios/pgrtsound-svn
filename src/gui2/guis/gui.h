#ifndef GUI_H
#define GUI_H

#include "../../modules/module.h"
#include <gtkmm/widget.h>

#define REGISTER_GUI( mod_type, class_name ) \
	static Gui* Create( Module* __mod ) { return new class_name( __mod ); } \
	static string GetTypeStatic() { return mod_type; } \
	string GetType() { return mod_type; } \

/**
 * No description
 */
class Gui {
	public:
		Gui(  Module* module );
		virtual ~Gui();
		virtual Gtk::Widget* GetGui();
		static Gui* Create( Module* mod ) { (void)mod; return NULL; }
		static string GetTypeStatic() { return ""; }
		string GetType() { return ""; }

	protected:
		Module* module;
};

#endif // GUI_H
