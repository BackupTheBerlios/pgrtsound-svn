#include "algorithmview.h"



#include <gtkmm/messagedialog.h>

#define FRAMES_PER_BUFFER 256

using namespace std;

AlgorithmView::AlgorithmView() : algorithm(FRAMES_PER_BUFFER) {
	TRACE("AlgorithmView::AlgorithmView - Konstrukcja...\n");
	//algorithm = algo;
	isDraggingModule = false;
	isDraggingConnection = false;
	currentGuiModule = NULL;
	width = 1500;
	height = 1500;
	set_size(width, height);

	//Ask to receive events the layout doesn't normally subscribe to.
	set_events(/*Gdk::LEAVE_NOTIFY_MASK | */ Gdk::BUTTON_PRESS_MASK | 
		Gdk::POINTER_MOTION_MASK | Gdk::POINTER_MOTION_HINT_MASK |
		Gdk::BUTTON_RELEASE_MASK );

	// kolory
 	Glib::RefPtr<Gdk::Colormap> colormap = get_default_colormap ();
	//bgColor = Gdk::Color("lightgray");
	fgColor = Gdk::Color("black");
	//colormap->alloc_color(bgColor);
	colormap->alloc_color(fgColor);
	
	
	//Fill popup menu:
	TRACE( "    Tworze menu...\n" );
	{
		Gtk::Menu::MenuList& menulist = menuPopup.items();
		std::vector<std::string> types = algorithm.ListModuleTypes();
		for(std::vector<std::string>::iterator it = types.begin();
		    it != types.end(); ++it)
		{
			TRACE( "          %s\n", (*it).c_str() );
			menulist.push_back( Gtk::Menu_Helpers::MenuElem(*it,
				sigc::bind<std::string>
					(sigc::mem_fun(*this, &AlgorithmView::onMenuAddModule), *it)
				) );
		}

	}
//	menuPopup.accelerate(*this);

	InitAudioPorts();
}

//void AlgorithmView::SetParentWindow( Gtk::Window *window ) {
//    parent = window;
//}

AlgorithmView::~AlgorithmView() {
    TRACE("AlgorithmView:~AlgorithmView - Destrukcja...");
    currentGuiModule = NULL;

	Clear();
}

/*
 Obsluga ruchu myszki.
 Przesuwa modul, robi polaczenie itp. zaleznie od ustawionych flag.
*/
bool AlgorithmView::on_motion_notify_event(GdkEventMotion* event) {
	window->freeze_updates(); // ???

	if( event && event->window ) {
       	int x = 0, y = 0;
		//const Glib::RefPtr<Gdk::Window> refWindow = Glib::wrap((GdkWindowObject*) event->window, true); // true == take_copy
		// okno Layout
		const Glib::RefPtr<Gdk::Window> refWindow = this->get_window();
		Gdk::ModifierType state = Gdk::ModifierType(0);

		if( refWindow ) {
			refWindow->get_pointer(x, y, state);
			
			// uwzgledniamy ustawienia scrollbarow
			adjh = get_hadjustment();
			adjv = get_vadjustment();
			
			// mamy bezwgledne wspolrzedne kursora w layout
			x += (int)adjh->get_value();
			y += (int)adjv->get_value();

			// przesuwamy modul
			if( isDraggingModule ) {
				int newX, newY;
				if( (state & Gdk::BUTTON1_MASK) != 0 ) {
					if(currentGuiModule != NULL) {
						newX = x - currentGuiModuleX;
						newY = y - currentGuiModuleY;

						// modul nie moze wyjsc poza layout
						if( newX < 0 ) newX = 0;
						if( ( newX + currentGuiModule->get_width() ) > width )
							newX = width - currentGuiModule->get_width();
						if( newY < 0 ) newY = 0;
						if( ( newY + currentGuiModule->get_height() ) > height )
							newY = height - currentGuiModule->get_height();

						currentGuiModule->SetXY(newX, newY);
						move(*currentGuiModule, newX, newY);
						RedrawConnections();
					}
				}
				window->thaw_updates();
				return true;
			}
			
			FindCurrentModule( x, y );

			if(isDraggingConnection) {
                connectionDrag.destinationX = x;
				connectionDrag.destinationY = y;
				window->invalidate_rect( Gdk::Rectangle(0, 0, width, height), false );
				//return true;
			}

			// kursor nad modulem
			if( currentGuiModule != NULL ) {
				int xpos, ypos;
				currentGuiModule->get_window()->get_position(xpos, ypos);
				// zapalmy w*jscie jesli kusor nad jakims
				currentGuiModule->FindXput( x - xpos, y - ypos );

				int inputId = currentGuiModule->GetCurrentInputNumber();
				int outputId = currentGuiModule->GetCurrentOutputNumber();

				Glib::ustring str;
				if( inputId > -1 ) {
					m_signal_notify_xput.emit( currentGuiModule->GetModule()
						->GetInput( inputId )->GetName() );
				}

				if( outputId > -1 ) {
					Glib::ustring str = currentGuiModule->GetModule()
						->GetOutput( outputId )->GetName();
					m_signal_notify_xput.emit( str );
				}
			}

		}
	}
	window->thaw_updates();
	return true;
}

/*
 Obluga kliku myszka.
 Sprawdza czy kliknieto modul -> mozliwe przesuwanie modulu, czy moze kliknieto
 jakies wyjscie -> mozliwe tworzenie polaczenia.
*/
bool AlgorithmView::on_button_press_event( GdkEventButton* event ) {
	int x, y;
	get_pointer(x, y);
	adjh = get_hadjustment();
	adjv = get_vadjustment();
	// bezwzgledne wspolrzedne kursora w layout
	x += (int)adjh->get_value();
	y += (int)adjv->get_value();

	FindCurrentModule( x, y );

	if( currentGuiModule != NULL) {
	    int posx, posy;
		currentGuiModule->get_window()->get_position( posx, posy );
		currentGuiModuleX = x - posx;
		currentGuiModuleY = y - posy;
		currentGuiModule->get_window()->raise();

		// podwojny klik na module
		if( event->type == Gdk::DOUBLE_BUTTON_PRESS ) {
			//cout << "Double click na module '" << currentGuiModule->GetModule()->GetName() << "'" << endl;
            currentGuiModule->OpenGuiWindow();
            return true;
		}

		// wlaczenie przesuwania modulu
		if( (currentGuiModule->GetCurrentInputNumber() == -1) &&
			(currentGuiModule->GetCurrentOutputNumber() == -1) )
		{
			// usuwanie modulu
			if( (event->type == GDK_BUTTON_PRESS) && ( event->state == GDK_CONTROL_MASK ) ) {
        	    DeleteModule( currentGuiModule );
	            return true;
			}

            // mozna ruszac modulem tylko gdy kursor *nie* jest nad w*jciem
			isDraggingModule = true;
			return true;
		}

		// usuwnaie polaczenia
		if( (event->type == GDK_BUTTON_PRESS) && ( event->state == GDK_CONTROL_MASK )
			&& (currentGuiModule->GetCurrentInputNumber() > -1) ) {
			// prawy przycisk nad wejsciem - usuwamy polaczenie
			if( currentGuiModule->GetModule()->GetInput(
				currentGuiModule->GetCurrentInputNumber() )->IsConnected() ) {
					DeleteConnection( currentGuiModule, currentGuiModule->GetCurrentInputNumber() );
					return true;
			}
		}

		// jesli kliknieto wyjscie - mozliwe tworzenie polaczenia
		if( (currentGuiModule->GetCurrentOutputNumber() > -1) ) {
			ConnectionId temp;
            connSourceModule = currentGuiModule;
            connSourceNumber = currentGuiModule->GetCurrentOutputNumber();
			connectionDrag.Set( temp, currentGuiModule,
			    currentGuiModule->GetCurrentOutputNumber(), NULL, -1);
			connectionDrag.destinationX = x;
			connectionDrag.destinationY = y;
			connections.push_back(&connectionDrag);
			isDraggingConnection = true;
		}
	} else {
        // Popup menu pod prawym klawiszem
       	lastClick.set_x( x );
		lastClick.set_y( y );
       	if( (event->type == GDK_BUTTON_PRESS) && (event->button == 3) )	{
			menuPopup.popup(event->button, event->time);
		}
		return true;
	}

	// nie przesylamy tego zdarzenia do dzieci
	return true;
}

/*
 Puszczono przycisku myszki.
 Sprawdzamy czy aby nie w trakcie tworzenia polaczenia itp.
*/
bool AlgorithmView::on_button_release_event(GdkEventButton* event) {
	(void) event;

	// koniec przesuwnia modulu
	if(isDraggingModule) {
       	isDraggingModule = false;
		//currentModule = NULL;
	}

	// sprawdzic czy opuszczono kursor nad jakims wejsciem
	if(isDraggingConnection) {
		connDestModule = currentGuiModule;
		if(connDestModule && (connDestModule != connSourceModule) ) {
			// opuszczono nad innym modulem niz poczatkowy
			connDestNumber = connDestModule->GetCurrentInputNumber();
			if(connDestNumber > -1) {
				// opuszczono nad wejsciem
				if( !connDestModule->GetModule()->GetInput(connDestNumber)->IsConnected() ) {
					// wejscie jeszcze nie polaczone - mamy polaczenie!
	    			ConnectModules(connSourceModule, connSourceNumber, connDestModule, connDestNumber);
//					cout << "Connection: " <<
//						connSourceModule->GetModule()->GetName() <<
//						" [" << connSourceNumber << "]   ->   " <<
//						connDestModule->GetModule()->GetName() <<
//						" [" << connDestNumber << "]" << endl;
				}
			}
		}
		connections.remove(&connectionDrag);
		window->invalidate_rect( Gdk::Rectangle(0, 0, width, height), false );
		isDraggingConnection = false;
	}
	
	return false;
}

/*
 Dodawanie do widoku modulu.
*/
void AlgorithmView::AddModule(string type, string name, int x, int y) {
	TRACE( "AlgorithmView::AddModule - Dodaje modul typu '%s'\n",
		type.c_str() );
    ModuleId modId = algorithm.AddModule( type, name );
    Module* mod = algorithm.GetModule( modId );

	GuiModule* guiMod = guiFactory.CreateGuiModule(mod);
	guiMod->SetParentView( this ); // konieczne na razie :(
	guiModules.push_back( guiMod );
	guiMod->SetXY(x, y);
	guiMod->SetModuleId( modId );
	name2GuiModuleMap.insert( make_pair( guiMod->GetModule()->GetName(), guiMod) );
	
	// na ekran
	this->put( *guiMod, x, y );

	algorithm.PrintInfo();
    
    show_all_children();
}

/*
 Aktywacja (focus) wybranego modulu.
*/
void AlgorithmView::SelectGuiModule(GuiModule* guiMod) {
	if( (currentGuiModule == NULL) && (guiMod != NULL)  ) {
		currentGuiModule = guiMod;
	}
	else if( (currentGuiModule != NULL) && (guiMod == NULL)  ) {
		currentGuiModule = guiMod;
	}
}

bool AlgorithmView::IsDraggingModule() {
	return isDraggingModule;
}

void AlgorithmView::ConnectModules(GuiModule* sourceGuiModule, int sourceNumOutput,
	GuiModule* destGuiModule, int destNumInput)
{
	ConnectionId connId;
	
	if( algorithm.ConnectModules(
			sourceGuiModule->GetModule()->GetName(),
			sourceNumOutput,
			destGuiModule->GetModule()->GetName(),
			destNumInput, connId ) ) {
       	GuiConnection* guiConn = new GuiConnection;
		guiConn->Set( connId, sourceGuiModule, sourceNumOutput, destGuiModule,
			destNumInput);
		//destGuiModule->SetInputConnectionId( destNumInput, connId );
		destGuiModule->SetInputGuiConnection( destNumInput, guiConn );
		algorithm.CreateQueue();
		algorithm.PrintEdges(); // debug
		connections.push_back(guiConn);
		window->invalidate_rect( Gdk::Rectangle(0, 0, width, height), false );
	}
	else {
		TRACE( "AlgorithmView::ConnectModules - Blad cyklicznosci grafu\n" );
		//Gtk::MessageDialog dialog( *parent, "RTSound", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
		//dialog.set_secondary_text( Glib::locale_to_utf8("Nie mo¿na utworzyæ po³aczenia cyklicznego!") );
		//dialog.run();
	}
}

void AlgorithmView::RedrawConnections() {
	GuiConnection* guiConn;
	
	//cout << connections.size() << endl;

	for(std::list<GuiConnection*>::iterator guiConnIt = connections.begin();
		guiConnIt != connections.end(); guiConnIt++)
	{
  		guiConn = *guiConnIt;
        //gc->set_foreground(bgColor);
		window->draw_line(bgGc, guiConn->sourceX, guiConn->sourceY,
			guiConn->destinationX, guiConn->destinationY);

		guiConn->Update();

		//gc->set_foreground(fgColor);
		window->draw_line(fgGc, guiConn->sourceX, guiConn->sourceY,
			guiConn->destinationX, guiConn->destinationY);
  	}
}

void AlgorithmView::Clear() {
	// pozbywamy sie GuiModulow
	for( list<GuiModule*>::iterator modIt = guiModules.begin();
		modIt != guiModules.end(); modIt++ )
	{
		delete *modIt;
	}
	
	guiModules.clear();
	
	for(list<GuiConnection*>::iterator connIt = connections.begin();
		connIt != connections.end(); connIt++)
	{
		delete *connIt;
	}
	
	connections.clear();
	name2GuiModuleMap.clear();
	algorithm.Clear();

	window->clear();
	window->invalidate_rect( Gdk::Rectangle(0, 0, width, height), false );

}

Algorithm* AlgorithmView::GetAlgorithm() {
	return &algorithm;
}

void AlgorithmView::InitAudioPorts() {
	TRACE( "AlgorithmView::InitAudioPorts\n" );

  	GuiModule* guiMod;
	//ModuleId moduleId;

	guiMod = new GuiModule( algorithm.GetModule("AudioPortIn") );
	guiMod->SetParentView(this);
	guiMod->SetModuleId( algorithm.GetModuleId("AudioPortIn") );
	guiModules.push_back(guiMod);
	name2GuiModuleMap.insert( make_pair( guiMod->GetModule()->GetName(), guiMod) );
	guiMod->SetXY( 0, 0 );
	this->put(*guiMod, 0, 0);

	guiMod = new GuiModule( algorithm.GetModule("AudioPortOut") );
	guiMod->SetParentView(this);
	guiMod->SetModuleId( algorithm.GetModuleId("AudioPortOut") );
	guiModules.push_back(guiMod);
	name2GuiModuleMap.insert( make_pair( guiMod->GetModule()->GetName(), guiMod) );
	guiMod->SetXY( 200, 0 );
	this->put(*guiMod, 200, 0);
	
    show_all_children();
}

void AlgorithmView::on_realize() {
	Gtk::Layout::on_realize();
	window = get_bin_window();
	fgGc = Gdk::GC::create(window);
	//gc->set_foreground(bgColor);
	
	bgGc = get_style()->get_bg_gc( get_state() );
	
	//window->set_background(bgColor);
	window->clear();
	fgGc->set_foreground(fgColor);
}

bool AlgorithmView::on_expose_event(GdkEventExpose* event) {
	(void) event;
	
	// rysjemy polaczenia
	GuiConnection* guiConn;
	for(std::list<GuiConnection*>::iterator guiConnIt = connections.begin();
		guiConnIt != connections.end(); guiConnIt++)
	{
        guiConn = *guiConnIt;
        // This is where we draw on the window
		window->draw_line(fgGc,	guiConn->sourceX, guiConn->sourceY,
			guiConn->destinationX, guiConn->destinationY);
	}

	return false;
}

void AlgorithmView::onMenuAddModule(std::string type) {
	int x, y;
	get_pointer(x, y);
    char txt[11];
    g_snprintf(txt, 10, " %d", guiModules.size());
	AddModule(type, type + txt, lastClick.get_x(), lastClick.get_y());
}

void AlgorithmView::DeleteConnection( GuiModule* module, int inputId ) {
	//cout << "AlgorithmView::DeleteConnection: Modulu '" << module->GetModule()->GetName()
	//	<< "' input " << inputId << endl;

	if( algorithm.DeleteConnection( module->GetInputGuiConnection( inputId )
		->GetConnectionId() ) )
	{
		connections.remove( module->GetInputGuiConnection( inputId ) );
		RedrawConnections();
		window->invalidate_rect( Gdk::Rectangle(0, 0, width, height), false );
	}
}

void AlgorithmView::DeleteModule( GuiModule* guiModule ) {
	if( guiModule != NULL && guiModule->GetModule()->GetName() != "AudioPortIn"
	    && guiModule->GetModule()->GetName() != "AudioPortOut" )
	{
		//cout << "Chce usunac modul '" << guiModule->GetModule()->GetName() << endl;
		if( algorithm.DeleteModule(guiModule->GetModuleId()) ) {
			name2GuiModuleMap.erase( guiModule->GetModule()->GetName() );
			this->remove( *guiModule );
			guiModules.remove( guiModule );
			delete guiModule;
			currentGuiModule = NULL;

			UpdateConnections();
			RedrawConnections();
			window->invalidate_rect( Gdk::Rectangle(0, 0, width, height), false );
		}
	}
}

void AlgorithmView::UpdateConnections() {
	TRACE( "AlgorithmView::UpdateConnections...\n" );
	for( list<GuiConnection*>::iterator connIt = connections.begin();
		connIt != connections.end(); connIt++ )
	{
		delete *connIt;
	}
	connections.clear();

//	cout << "    w mapie sa: " << endl;
//	for( std::map<string, GuiModule*>::iterator it = name2GuiModuleMap.begin();
//		it != name2GuiModuleMap.end(); it++ )
//	{
//		cout << "        " << (*it).first << " " << (*it).second << endl;
//	}

	// utworzenie GUI-polaczen na podstawie polaczen w obiekcie Algorithm
	for( ConnectionIdIterator connIt = algorithm.ConnectionIdIteratorBegin();
	    connIt != algorithm.ConnectionIdIteratorEnd(); connIt++ )
	{
		ConnectionId connId = *connIt;
		GuiConnection* guiConn = new GuiConnection;
		Connection* conn =  algorithm.GetConnection( connId );
		
		GuiModule *guiSrc, *guiDest;
		guiSrc = (*name2GuiModuleMap.find( conn->sourceModule->GetName() )).second;
		guiDest = (*name2GuiModuleMap.find( conn->destinationModule->GetName() )).second;

		cout << "    " << guiSrc->GetModule()->GetName() << " to " <<
			guiDest->GetModule()->GetName() << endl;

		guiConn->Set( connId, guiSrc, conn->sourceOutputId, guiDest, conn->destinationInputId );
		guiDest->SetInputGuiConnection( conn->destinationInputId, guiConn );
  		connections.push_back( guiConn );
	}
	
	algorithm.PrintEdges();
}

bool AlgorithmView::ChangeModuleName( ModuleId modId, string str ) {
	string oldName = algorithm.GetModule( modId )->GetName();
	GuiModule* guiMod = GetGuiModule( oldName );

   	if( !algorithm.ChangeModuleName( modId, str.c_str() ) ) {
        Gtk::MessageDialog dialog( *( (Gtk::Window*)get_toplevel() ),
			"RTSound", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
		dialog.set_secondary_text( "Modul o tej nazwie juz istnieje!" );
		dialog.run();
		return false;
	}
	
	name2GuiModuleMap.erase( oldName );
	name2GuiModuleMap.insert( make_pair( str, guiMod ) );

	return true;
}

// signal accesor
AlgorithmView::type_signal_notify_xput AlgorithmView::signal_notify_xput() {
	return m_signal_notify_xput;
}

void AlgorithmView::FindCurrentModule( int x, int y ) {
	if( !isDraggingModule ) {
		currentGuiModule = NULL;
		int mx, my;

		for( list<GuiModule*>::iterator modIt = guiModules.begin();
			modIt != guiModules.end(); modIt++ )
		{
			(*modIt)->GetPosition( mx, my );
			//cout << "mx = " << mx << "  my = " << my << endl;
			if( ( mx < x && x < mx + (*modIt)->GetWidth() )
				&& ( my < y && y < my + (*modIt)->GetHeight() ) )
			{
				//cout << "    modulu : " << (*modIt)->GetModule()->GetName() << endl;
				currentGuiModule = *modIt;
				break;
			}
		}
	}
}

GuiModule* AlgorithmView::GetGuiModule( string name ) {
    if ( name2GuiModuleMap.count( name ) > 0 ) {
    	return ( *name2GuiModuleMap.find( name ) ).second;
    }
	else {
		TRACE( "!!!! AlgorithmView::GetGuiModule - Nie ma GUI modulu o nazwie '%s'\n",
		    name.c_str() );
		 return NULL;
	}
}
