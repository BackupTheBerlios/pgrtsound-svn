#include "gmonitorgraph.h"



GMonitorGraph::GMonitorGraph( Module* mod ) :
	Gui( mod )
{
  	// timer
	my_slot = sigc::mem_fun( this, &GMonitorGraph::OnUpdateTimer );
	conn = Glib::signal_timeout().connect( my_slot, 200 );
}


GMonitorGraph::~GMonitorGraph(){
    conn.disconnect();
}

Gtk::Widget* GMonitorGraph::GetGui() {
	return &curve;
}

bool GMonitorGraph::OnUpdateTimer() {
	//TRACE( "GMonitor::OnUpdateTimer...\n" );
	
    
    list<float> pointsList;
    float *lista=module->GetInput( 0 )->GetSignal();
    float max=10,min=-10;
    if (module->GetInput( 0 ) != NULL)
        for (unsigned int i=0; i<Module::framesPerBlock;i++) {
            float value = (*lista++) * 10;
            pointsList.push_back(value);
            if (max<value) max = value;
            if (min>value) min = value;
        }   
	curve.set_range(0,Module::framesPerBlock,min-5,max+5);
    curve.reset();
	Glib::ArrayHandle<float> arrayHandle(pointsList);
	curve.set_vector(arrayHandle);
	return true;
}
