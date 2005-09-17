#include "gfeedbackdelay.h"

GFeedbackDelay::GFeedbackDelay( Module* mod ) :
	Gui( mod )
{
	delayEntry.AttachFloatParameter( (ParameterFloat*)module->GetParameter( 0 ) );

	slFeedback.SetParameter( (ParameterFloat*)module->GetParameter( 1 ),
		0, 0.99, 0.01 );

	delayButton.signal_clicked().connect(
		sigc::mem_fun( this, &GFeedbackDelay::OnDelayChanged ) );

	slFeedback.signal_slider_moved().connect(
		sigc::mem_fun( (MFeedbackDelay*)module, &MFeedbackDelay::SomethingChanged ) );

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

GFeedbackDelay::~GFeedbackDelay() {

}

Gtk::Widget* GFeedbackDelay::GetGui() {
	return &box;
}

void GFeedbackDelay::OnDelayChanged() {
    ( (ParameterFloat*)module->GetParameter( 0 ) )->SetValue(
		delayEntry.GetValue() );
	delayEntry.SetValue( ( (ParameterFloat*)module->GetParameter( 0 ) )->GetValue() );
	( (MFeedbackDelay*)module )->SomethingChanged();
}
