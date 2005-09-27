#include    "gmonitorgraph.h"

GMonitorGraph::GMonitorGraph( Module* mod ) :
	Gui( mod ),
    slScale(0,5,1), 
    slTime(-3,5,1)
{
  	// timer
	my_slot = sigc::mem_fun( this, &GMonitorGraph::OnUpdateTimer );
	conn = Glib::signal_timeout().connect( my_slot, 200 );
	
	size   = Module::framesPerBlock;
	
    slScale.signal_value_changed().connect( sigc::mem_fun(this, &GMonitorGraph::ScaleValueChanged ));
	slScale.set_draw_value(false);
    slTime.signal_value_changed().connect( sigc::mem_fun(this, &GMonitorGraph::TimeValueChanged ));
    slTime.set_draw_value(false);
        
    for (unsigned int i = 0; i < size ; i++)
        buffor[i] = 0;
     
    n          = 0;
    timeValue  = 1;
    t          = 1;
    scaleValue = 1;
    x          = 0;
    slTime.set_value(timeValue);
    slScale.set_value(scaleValue);
    
    curve.set_range(1,size,-100, 100);

    labelScale.set_text("1");
    labelTime.set_text("1");
    scaleBox.add( labelScale );
    scaleBox.add( slScale );
    hbox.add( curve );
	hbox.add( scaleBox );
    timeBox.add( labelTime );
    timeBox.add( slTime );
	box.add ( hbox );
	box.add ( timeBox );
}


GMonitorGraph::~GMonitorGraph(){
    delete buffor;
    conn.disconnect();
}

Gtk::Widget* GMonitorGraph::GetGui() {
	return &box;
}

bool GMonitorGraph::OnUpdateTimer() {

    if (module->GetInput( 0 ) != NULL) {
        float *in=module->GetInput( 0 )->GetSignal();
        float i;
        for (i=0; i<size; i+=t) {       
            if (n>=size) {
                n=0;
                t = timeValue;
                list<float> pointsList;
                for (unsigned int j=0; j<size;j++) {
                    float value = (buffor[j]);
                    pointsList.push_back(value*scaleValue);
                }   
                Glib::ArrayHandle<float> arrayHandle(pointsList);
	            curve.set_vector(arrayHandle);  
    
            }
           buffor[n] = in[(int)floor(i)]; 
           buffor[n] = in[0];
            n++; 
            
        }
        
    }

	return true;
}

void GMonitorGraph::ScaleValueChanged() {
  	int value = 1;
    for (int i = 0; i < (int)slScale.get_value(); i++)
        value *=10;
  	
  	scaleValue = value * 100;
    g_snprintf( txtBuffer, 50, "%i", value );    
    labelScale.set_text( txtBuffer );	
}

void GMonitorGraph::TimeValueChanged() {
  	float value = 1;
  	if ((int)slTime.get_value()>0)
        for (int i = 0; i < (int)slTime.get_value(); i++)
            value *=2;
  	if ((int)slTime.get_value()<0)
        for (int i = 0; i < (-(int)slTime.get_value()); i++)
            value /=2;

  	
  	timeValue = value;
    g_snprintf( txtBuffer, 50, "%f", value );    
    labelTime.set_text( txtBuffer );	
}
