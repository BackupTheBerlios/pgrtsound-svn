#include "gmonitor.h"

GMonitor::GMonitor( Module* mod ) :
	Gui( mod )
{
  	// timer
	my_slot = sigc::mem_fun( this, &GMonitor::OnUpdateTimer );
	conn = Glib::signal_timeout().connect( my_slot, 200 );
}


GMonitor::~GMonitor(){
    conn.disconnect();
}

Gtk::Widget* GMonitor::GetGui() {
	return &monitorLabel;
}

bool GMonitor::OnUpdateTimer() {
	//TRACE( "GMonitor::OnUpdateTimer...\n" );
	float* monitoredValue = module->GetInput( 0 )->GetSignal();
	g_snprintf( txtBuffer, 50, "%#.2f", *monitoredValue );
	monitorLabel.set_text( txtBuffer );
	return true;
}
