#include "gwavplayer.h"

#include <gtkmm/filechooserbutton.h>
#include <gtkmm/stock.h>

GWavPlayer::GWavPlayer( Module* mod ) :
	Gui( mod )
{
	openFileButton.set_label( Glib::locale_to_utf8("Wybierz plik") );
	openFileButton.signal_clicked().connect( sigc::mem_fun( this, &GWavPlayer::OnFileOpen ) );
	
	fileLabel.set_text( Glib::locale_to_utf8(
		( (ParameterString*)( module->GetParameter( 0 )) )->GetText()
	) );
	
	fileLabel.set_ellipsize( Pango::ELLIPSIZE_END );

	box.set_spacing( 2 );
	box.add( openFileButton );
	box.add( fileLabel );
}

GWavPlayer::~GWavPlayer() {

}

Gtk::Widget* GWavPlayer::GetGui() {
	return &box;
}

void GWavPlayer::OnFileOpen() {
	Gtk::FileChooserDialog dialog( Glib::locale_to_utf8( "Please choose a file" ),
		Gtk::FILE_CHOOSER_ACTION_OPEN );
	dialog.set_transient_for( *( (Gtk::Window*)openFileButton.get_toplevel() ) );
	dialog.set_position( Gtk::WIN_POS_CENTER_ON_PARENT );
	dialog.add_button( Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL );
	dialog.add_button( Gtk::Stock::OPEN, Gtk::RESPONSE_OK );
	Gtk::FileFilter filter_text;
	filter_text.set_name("WAV files");
	//filter_text.add_mime_type("text/xml");
	filter_text.add_pattern("*.wav");
	dialog.add_filter(filter_text);
	int result = dialog.run();

	if( result == Gtk::RESPONSE_OK ) {
		Glib::ustring fileName = dialog.get_filename();

		( (ParameterString*)( module->GetParameter( 0 )) )
			->SetText( Glib::locale_from_utf8( fileName ) );

		if( !( (MWavPlayer*)module )->Open() ) {
			TRACE( "GWavPlayer::OnFileOpen - Blad otwarcia pliku '%s'\n",
			    fileName.c_str() );
		}
		
		fileLabel.set_text( fileName );
	}
}
