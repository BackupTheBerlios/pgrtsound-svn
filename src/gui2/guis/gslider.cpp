#include "gslider.h"

GSlider::GSlider( Module* mod ) :
	Gui( mod ),
	guiSlider(0, 1, 0.1),
    table( 4, 2, false )
{
	TRACE( "GSlider::GSlider\n" );
	ParameterFloat* param;

	TRACE( "GSlider::GSlider - seting min i max\n" );
	pMin = (ParameterFloat*)module->GetParameter( 0 );
	entryMin.AttachFloatParameter( pMin );

	pMax = (ParameterFloat*)module->GetParameter( 1 );
	entryMax.AttachFloatParameter( pMax );

	//TRACE( "GSlider::GSlider - attaching pVal\n" )	;
	param = (ParameterFloat*)module->GetParameter( 2 ); // parametr value
	cout << "GSlider::GSlider - attaching pVal: " << param->GetValue() << endl;
	
	guiSlider.SetParameter( param, pMin->GetValue(), pMax->GetValue(), 0.1 );

	//	cout << sliderModule->GetName() << "." << param->GetName() << " " << param->GetValue() << endl;
	//guiSlider.set_value( param->GetValue() );

	TRACE( "GSlider::GSlider - creating GUI\n" )	;

	labelMin.set_label( "Minimum" );
	labelMax.set_label( "Maksimum" );

    buttonApply.set_label( Glib::locale_to_utf8("Zmieñ zakres") );
	
	//Gtk::FILL /* Gtk::EXPAND*/
	table.set_row_spacings( 5 );
	table.attach( guiSlider, 0, 2, 0, 1 );
	table.attach( labelMin, 0, 1, 1, 2 );
	table.attach( entryMin, 1, 2, 1, 2 );
	table.attach( labelMax, 0, 1, 2, 3 );
	table.attach( entryMax, 1, 2, 2, 3 );
	table.attach( buttonApply, 0, 2, 3, 4 );
	
	buttonApply.signal_clicked().connect( sigc::mem_fun( this, &GSlider::OnApply ) );
	
	TRACE( "GSlider::GSlider - Done\n" );
}

GSlider::~GSlider() {
	// insert your code here
}

Gtk::Widget* GSlider::GetGui() {
	return &table;
}

void GSlider::OnApply() {
	float min = entryMin.GetValue();
	float max = entryMax.GetValue();

	pMin->SetValue( min );
	pMax->SetValue( max );

	guiSlider.ChangeRange( min, max, 0.01 );
}
