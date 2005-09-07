#include "ggate.h"

GGate::GGate( Module* mod ) :
	Gui( mod ),
	buttonGate( Glib::locale_to_utf8( "Bramkuj" ) )
{
    buttonGate.signal_pressed().connect( sigc::mem_fun( this, &GGate::Switch ) );
	buttonGate.signal_released().connect( sigc::mem_fun( this, &GGate::Switch ) );
}


GGate::~GGate() {
}

Gtk::Widget* GGate::GetGui() {
	return &buttonGate;
}

void GGate::Switch() {
	MGate* gateMod = (MGate*)module;
	gateMod->Switch();
}
