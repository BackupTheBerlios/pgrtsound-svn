#ifndef MODULEGUIFORM_H
#define MODULEGUIFORM_H

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
//#include <gtkmm/frame.h>
#include <gtkmm/separator.h>
#include <gtkmm/button.h>

#include <iostream>

class GuiModule; // pre-declaration

/**
 * No description
 */
class ModuleGuiWindow : public Gtk::Window
{
	public:
		ModuleGuiWindow( GuiModule* guiModule );
		~ModuleGuiWindow();
		void AddGui( Gtk::Widget* widget );
		void SetName( const Glib::ustring& str );
		const Glib::ustring GetName();
		

	protected:
		GuiModule* parentGuiModule;
        Gtk::VBox mainBox;
		Gtk::HBox nameBox;
		Gtk::Button nameButton;
		Gtk::HSeparator separator;
		Gtk::Label nameLabel;
		Gtk::Entry nameEntry;
};

#endif // MODULEGUIFORM_H
