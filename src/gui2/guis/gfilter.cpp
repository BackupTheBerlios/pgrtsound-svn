#include "gfilter.h"


GFilter::GFilter( Module* mod ) :
	Gui( mod )
{
	rbLP = Gtk::manage( new Gtk::RadioButton(group, Glib::locale_to_utf8("Dolnoprzepustowy")) );
	rbHP = Gtk::manage( new Gtk::RadioButton(group,	Glib::locale_to_utf8("G�rnoprzepustowy")) );
	buttonApply.set_label( Glib::locale_to_utf8("Zatwierd� ustawienia") );
	
	freqEntry.AttachFloatParameter( (ParameterFloat*)module->GetParameter( 0 ) );
	
	// stan poczatkowy
	pFilerType = (ParameterString*)module->GetParameter( 1 );
	if( pFilerType->GetText() == "LP" )
	    rbLP->set_active( true );
   	if( pFilerType->GetText() == "HP" )
	    rbHP->set_active( true );
	    
    labelFreq.set_label( Glib::locale_to_utf8("Cz�stotliwo��") );
    freqBox.add( labelFreq );
	freqBox.add( freqEntry );
	
	mainBox.set_spacing( 2 );
	
	mainBox.add( *rbLP );
	mainBox.add( *rbHP );
	mainBox.add( freqBox );
	mainBox.add( buttonApply );
	
	buttonApply.signal_clicked().connect( sigc::mem_fun( this, &GFilter::OnApply ) );
}

GFilter::~GFilter() {
	// insert your code here
}

void GFilter::OnApply() {
	//cout << freqEntry.GetValue() << endl;
	//cout << "LP " << rbLP->get_active() << endl;
	//cout << "HP " << rbHP->get_active() << endl;
	
	float freq =  freqEntry.GetValue();
	if( freq < 50.0) freq = 50.0;
	if( freq > 0.375 * Module::sampleRate ) freq = 0.375 * Module::sampleRate;
	
	freqEntry.SetValue( freq );
	
	if( rbLP->get_active() )
	    pFilerType->SetText( "lp" );

	if( rbHP->get_active() )
	    pFilerType->SetText( "hp" );

	((Filter12dB*)module)->UpdateCoefficients();
}

Gtk::Widget* GFilter::GetGui() {
	return &mainBox;
}