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
        void ScaleValueChanged();
        void TimeValueChanged();
		Gtk::VBox         box, scaleBox;
        Gtk::HBox         hbox, timeBox;
        Gtk::VScale       slScale;
  		Gtk::HScale       slTime;
        Gtk::Curve        curve;
        Gtk::Label        labelScale, labelTime;
		sigc::slot<bool>  my_slot;
		sigc::connection  conn;
		char              txtBuffer[50];
		float             buffor[5000];
		float             size;
		unsigned int      n, scaleValue,a;
		float             t, timeValue;
                            
};

#endif // GMONITORGRAPH_H
