#ifndef GMONITORGRAPH_H
#define GMONITORGRAPH_H

#include "gui.h"
#include "../../modules/mmonitorgraph.h"

#include <gtkmm/label.h>
#include <gtkmm/curve.h>
#include <gtkmm/scale.h>

/**
 * No description
 */
class GMonitorGraph : public Gui
{
	public:
		GMonitorGraph( Module* mod );
		~GMonitorGraph();
		virtual Gtk::Widget* GetGui();
		REGISTER_GUI( MMonitorGraph::GetTypeStatic(), GMonitorGraph )
		bool OnUpdateTimer();

	protected:
		Gtk::VBox         box;
        Gtk::HBox         hbox;
        Gtk::VScale       slScale;
  		Gtk::HScale       slTime;
        Gtk::Curve        curve;
		sigc::slot<bool>  my_slot;
		sigc::connection  conn;
		char              txtBuffer[50];
		float             buffor[5000];
		unsigned int      n;
		unsigned int      t;
		int               a;  
};

#endif // GMONITORGRAPH_H
