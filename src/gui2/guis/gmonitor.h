#ifndef GMONITOR_H
#define GMONITOR_H

#include "gui.h"
#include "../../modules/mmonitor.h"

#include <gtkmm/label.h>

/**
 * No description
 */
class GMonitor : public Gui
{
	public:
		GMonitor( Module* mod );
		~GMonitor();
		virtual Gtk::Widget* GetGui();
		REGISTER_GUI( MMonitor::GetTypeStatic(), GMonitor )
		bool OnUpdateTimer();
		
	protected:
		Gtk::Label monitorLabel;
		sigc::slot<bool> my_slot;
		sigc::connection conn;
		char txtBuffer[50];
};

#endif // GMONITOR_H
