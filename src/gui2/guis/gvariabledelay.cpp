#include "gvariabledelay.h"

GVariableDelay::GVariableDelay( Module* mod ) :
	Gui( mod )
{
	delayEntry.AttachFloatParameter( (ParameterFloat*)module->GetParameter( 0 ) );

	slFeedback.SetParameter( (ParameterFloat*)module->GetParameter( 1 ),
		0, 0.99, 0.01 );

	delayButton.signal_clicked().connect(
		sigc::mem_fun( this, &GVariableDelay::OnDelayChanged ) );

	slFeedback.signal_slider_moved().connect(
		sigc::mem_fun( (MVariableDelay*)module, &MVariableDelay::SomethingChanged ) );

	delayLabel.set_label( Glib::locale_to_utf8( "OpóŸnienie [ms]" ) );
	delayButton.set_label( Glib::locale_to_utf8( "Zmieñ" ) );

	delayBox.set_spacing( 2 );
	delayBox.add( delayLabel );
	delayBox.add( delayEntry );
	delayBox.add( delayButton );

	box.set_spacing( 2 );
	box.add( delayBox );
	box.add( slFeedback );
}

GVariableDelay::~GVariableDelay() {

}

Gtk::Widget* GVariableDelay::GetGui() {
	return &box;
}

void GVariableDelay::OnDelayChanged() {
    ( (ParameterFloat*)module->GetParameter( 0 ) )->SetValue(
		delayEntry.GetValue() );
	delayEntry.SetValue( ( (ParameterFloat*)module->GetParameter( 0 ) )->GetValue() );
	( (MVariableDelay*)module )->SomethingChanged();
}
