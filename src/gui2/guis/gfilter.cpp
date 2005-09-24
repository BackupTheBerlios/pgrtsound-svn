#include "gfilter.h"


GFilter::GFilter( Module* mod ) :
	Gui( mod )
{
	rbLP = Gtk::manage( new Gtk::RadioButton(group, Glib::locale_to_utf8("Dolnoprzepustowy")) );
	rbHP = Gtk::manage( new Gtk::RadioButton(group,	Glib::locale_to_utf8("Górnoprzepustowy")) );
	buttonApply.set_label( Glib::locale_to_utf8("ZatwierdŸ ustawienia") );
	
	//freqEntry.AttachFloatParameter( (ParameterFloat*)module->GetParameter( 0 ) );
	
	slFreq.SetParameter( (ParameterFloat*)module->GetParameter( 0 ), 50, 12000, 1 );
	slFreq.signal_slider_moved().connect(
		sigc::mem_fun( (Filter12dB*)module, &Filter12dB::UpdateCoefficients ) );
	
	// stan poczatkowy
	pFilerType = (ParameterString*)module->GetParameter( 1 );
	if( pFilerType->GetText() == "lp" )
	    rbLP->set_active( true );
   	if( pFilerType->GetText() == "hp" )
	    rbHP->set_active( true );
	    
    labelFreq.set_label( Glib::locale_to_utf8("Czêstotliwoœæ") );
	freqBox.add( slFreq );
	
	mainBox.set_spacing( 2 );
	
	mainBox.add( *rbLP );
	mainBox.add( *rbHP );
	mainBox.add( freqBox );
//	mainBox.add( buttonApply );
	
	rbLP->signal_clicked().connect( sigc::mem_fun( this, &GFilter::OnApply ) );
	rbHP->signal_clicked().connect( sigc::mem_fun( this, &GFilter::OnApply ) );

    //group.signal_group_changed().connect( sigc::mem_fun( this, &GFilter::OnApply ) );
	
	//buttonApply.signal_clicked().connect( sigc::mem_fun( this, &GFilter::OnApply ) );
}

GFilter::~GFilter() {
	// insert your code here
}

void GFilter::OnApply() {
	//cout << freqEntry.GetValue() << endl;
	//cout << "LP " << rbLP->get_active() << endl;
	//cout << "HP " << rbHP->get_active() << endl;
	
	if( rbLP->get_active() )
	    pFilerType->SetText( "lp" );

	if( rbHP->get_active() )
	    pFilerType->SetText( "hp" );

	((Filter12dB*)module)->UpdateCoefficients();
}

Gtk::Widget* GFilter::GetGui() {
	return &mainBox;
}
