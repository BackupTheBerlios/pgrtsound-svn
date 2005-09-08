#include "moduleguiwindow.h"
#include "guimodule.h"

using namespace std;

ModuleGuiWindow::ModuleGuiWindow( GuiModule* guiModule ) :
	parentGuiModule( guiModule )
{
	nameLabel.set_text( Glib::locale_to_utf8("Nazwa: ") );
	nameButton.set_label( Glib::locale_to_utf8("Zmieñ nazwê") );

	nameBox.set_border_width( 2 );
	nameBox.set_spacing( 5 );
	nameBox.add( nameEntry );
	nameBox.add( nameButton );
	
	mainBox.set_spacing( 5 );
	mainBox.add( nameBox );
	add( mainBox );
	resize( 200, get_height() );

	nameButton.signal_clicked().connect( sigc::mem_fun( parentGuiModule, &GuiModule::ChangeName ) );
	
	if( parentGuiModule->GetModule()->GetName() == "AudioPortIn"
		|| parentGuiModule->GetModule()->GetName() == "AudioPortOut" ) {
		nameEntry.set_sensitive( false );
		nameButton.set_sensitive( false );
	}
}

ModuleGuiWindow::~ModuleGuiWindow() {
	//cout << "Zamykam okno GUI" << endl;
}

void ModuleGuiWindow::AddGui( Gtk::Widget *gui ) {
   	mainBox.add( separator );
	mainBox.add( *gui );
}

const Glib::ustring ModuleGuiWindow::GetName() {
	return nameEntry.get_text();
}

void ModuleGuiWindow::SetName( const Glib::ustring& str ) {
	nameEntry.set_text( str );
}
