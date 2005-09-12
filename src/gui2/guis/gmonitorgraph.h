#ifndef GMONITORGRAPH_H
#define GMONITORGRAPH_H

#include "gui.h"
#include "../../modules/mmonitorgraph.h"

#include <gtkmm/label.h>
#include <gtkmm/curve.h>

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
        Gtk::Curve curve;
		sigc::slot<bool> my_slot;
		sigc::connection conn;
		char txtBuffer[50];
		float *buffor;
		unsigned int  n;
		unsigned int  t;
		float         a;
};

#endif // GMONITORGRAPH_H
