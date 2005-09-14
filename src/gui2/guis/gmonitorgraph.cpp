#include    "gmonitorgraph.h"

GMonitorGraph::GMonitorGraph( Module* mod ) :
	Gui( mod ),
    slScale(1,101,1), 
    slTime(1,101,1)
{
    TRACE( "GMonitorGraph::GMonitorGraph( Module* mod )\n" );
  	// timer
	my_slot = sigc::mem_fun( this, &GMonitorGraph::OnUpdateTimer );
	conn = Glib::signal_timeout().connect( my_slot, 200 );
	
	//buffor = new float[Module::framesPerBlock];
    
    for (unsigned int i = 0; i <  Module::framesPerBlock; i++)
        buffor[i] = 0;
     
    n = 0;
    t = slTime.get_value();
    a = slScale.get_value();
    
    curve.set_range(0,Module::framesPerBlock,-1, 1);
    //curve.reset();
    
    hbox.add( curve );
	hbox.add( slScale );
    
	box.add ( hbox );
	box.add ( slTime );
	
	TRACE( "END GMonitorGraph::GMonitorGraph( Module* mod )\n" );
    
}


GMonitorGraph::~GMonitorGraph(){
    delete buffor;
    conn.disconnect();
}

Gtk::Widget* GMonitorGraph::GetGui() {
    TRACE( "Gtk::Widget* GMonitorGraph::GetGui()\n" );
	return &box;
}

bool GMonitorGraph::OnUpdateTimer() {
	TRACE( "GMonitorGraph::OnUpdateTimer\n" );

    if (module->GetInput( 0 ) != NULL) {
        float *in=module->GetInput( 0 )->GetSignal();
        for (unsigned int i=0; i<Module::framesPerBlock; i+=t) {       
            if (n>=Module::framesPerBlock) {
                n=0;
                t = slTime.get_value();
                if (a != slScale.get_value()) {
                    a = slScale.get_value();
                    curve.reset();
                    curve.set_range(0,Module::framesPerBlock,-a, a);

                }
                list<float> pointsList;
                for (unsigned int j=0; j<Module::framesPerBlock;j++) {
                    float value = (buffor[j]);
                    pointsList.push_back(value);
                }   
                Glib::ArrayHandle<float> arrayHandle(pointsList);
	            curve.set_vector(arrayHandle);  
                cout << "a="<<a << "  t="<<  t << endl ;      
            }
            buffor[n] = in[i]; 
            n++; 
        }
    }
    TRACE( "END GMonitorGraph::OnUpdateTimer\n" );
	return true;
}









/*
#include "gmonitorgraph.h"



GMonitorGraph::GMonitorGraph( Module* mod ) :
	Gui( mod )
{
    TRACE( "GMonitorGraph::GMonitorGraph( Module* mod )...\n" );
  	
      // timer
	my_slot = sigc::mem_fun( this, &GMonitorGraph::OnUpdateTimer );
	conn = Glib::signal_timeout().connect( my_slot, 200 );
	
	
	buffor = new float(Module::framesPerBlock);
    t = 2;
    a = 10;
    for (unsigned int i = 0; i <  Module::framesPerBlock; i++)
        buffor[i] = 0;
    n = 0;
    //curve.set_range(0,Module::framesPerBlock,-1 * a, 1 * a);
    
    
    TRACE( "end GMonitorGraph::GMonitorGraph( Module* mod )...\n" );
    
}


GMonitorGraph::~GMonitorGraph(){
    delete buffor;
    conn.disconnect();
}

Gtk::Widget* GMonitorGraph::GetGui() {
    TRACE( "GMonitorGraph::GetGui()...\n" );
	return &curve;
}

bool GMonitorGraph::OnUpdateTimer() {
	TRACE( "GMonitorGraph::OnUpdateTimer...\n" );

    list<float> pointsList;
    
    if (module->GetInput( 0 ) != NULL) {
        float *in=module->GetInput( 0 )->GetSignal();
        
        for (unsigned int i=0; i<Module::framesPerBlock;i+=t) {
            buffor[i] = *in++; 
            n++;           
        }
        if (n>=Module::framesPerBlock) 
            n = 0; 
        
        if (n==0)
            for (unsigned int i=0; i<Module::framesPerBlock;i++) {
                float value = (buffor[i]) * a;
                pointsList.push_back(value);
            }   
    }
    
    curve.reset();
	Glib::ArrayHandle<float> arrayHandle(pointsList);
	curve.set_vector(arrayHandle);    
    
	curve.set_range(0,Module::framesPerBlock,-1 * a, 1 * a);

	
	TRACE( "end GMonitorGraph::OnUpdateTimer...\n" );
	return true;
}
*/
