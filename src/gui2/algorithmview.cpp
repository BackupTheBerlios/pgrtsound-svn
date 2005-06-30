#include "algorithmview.h"

#define FRAMES_PER_BUFFER 256

using namespace std;

AlgorithmView::AlgorithmView() : algorithm(FRAMES_PER_BUFFER) {
	TRACE("AlgorithmView::AlgorithmView()", "Konstrukcja...");
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
		cout << "Tworze mneu............" << endl;
	{
		Gtk::Menu::MenuList& menulist = menuPopup.items();
		std::vector<std::string> types = algorithm.ListModuleTypes();
		for(std::vector<std::string>::iterator it = types.begin();
		    it != types.end(); ++it)
		{
			cout << "   dodaje do menu: " << *it << endl;
			menulist.push_back( Gtk::Menu_Helpers::MenuElem(*it,
				sigc::bind<std::string>
					(sigc::mem_fun(*this, &AlgorithmView::onMenuAddModule), *it)
				) );
		}
			
	}
//	menuPopup.accelerate(*this);

	InitAudioPorts();
	
 	TRACE("AlgorithmView::AlgorithmView()", "Done!");
}

void AlgorithmView::SetParentWindow(Gtk::Window *window) {
    parent = window;    
}

AlgorithmView::~AlgorithmView() {
    TRACE("AlgorithmView:~AlgorithmView()", "Destrukcja...");
    currentGuiModule = NULL;

	Clear();

    TRACE("AlgorithmView:~AlgorithmView()", "Destrukcja pomyslna");
}

/*
 Obsluga ruchu myszki.
 Przesuwa modul, robi polaczenie itp. zaleznie od ustawionych flag.
*/
bool AlgorithmView::on_motion_notify_event(GdkEventMotion* event) {

	//window->freeze_updates();

	if(event && event->window) {
       	int x = 0, y = 0;
		//const Glib::RefPtr<Gdk::Window> refWindow = Glib::wrap((GdkWindowObject*) event->window, true); // true == take_copy
		// okno Layout
		const Glib::RefPtr<Gdk::Window> refWindow = this->get_window();
		Gdk::ModifierType state = Gdk::ModifierType(0);

		if(refWindow) {
			refWindow->get_pointer(x, y, state);
			
			// uwzgledniamy ustawienia scrollbarow
			adjh = get_hadjustment();
			adjv = get_vadjustment();
			
			// mamy bezwgledne wspolrzedne kursora w layout
			x += (int)adjh->get_value();
			y += (int)adjv->get_value();

			// kursor nad modulem
			if(currentGuiModule != NULL) {
				int xpos, ypos;
				currentGuiModule->get_window()->get_position(xpos, ypos);
				// zapalmy w*jscie jesli kusor nad jakims
				currentGuiModule->FindXput(x - xpos, y - ypos);
			}

			// przesuwamy modul
			if(isDraggingModule) {
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
			}
			
			if(isDraggingConnection) {
				connectionDrag.destinationX = x;
				connectionDrag.destinationY = y;
				window->invalidate_rect( Gdk::Rectangle(0, 0, width, height), false );
			}
		}
	}

	//window->thaw_updates();

	return true;
}

/*
 Obluga kliku myszka.
 Sprawdza czy kliknieto modul -> mozliwe przesuwanie modulu, czy moze kliknieto
 jakies wyjscie -> mozliwe tworzenie polaczenia.
*/
bool AlgorithmView::on_button_press_event(GdkEventButton* event) {
	int x, y;
	get_pointer(x, y);
	adjh = get_hadjustment();
	adjv = get_vadjustment();
	// bezwzgledne wspolrzedne kursora w layout
	x += (int)adjh->get_value();
	y += (int)adjv->get_value();

	if( currentGuiModule != NULL) {
        int posx, posy;
		currentGuiModule->get_window()->get_position(posx, posy);
		currentGuiModuleX = x - posx;
		currentGuiModuleY = y - posy;
		currentGuiModule->get_window()->raise();

		// podwojny klik na module
		if(event->type == Gdk::DOUBLE_BUTTON_PRESS) {
			//cout << "Double click na module '" << currentGuiModule->GetModule()->GetName() << "'" << endl;			
            currentGuiModule->OpenGuiWindow(*parent);
		}

		// mozna ruszac modulem tylko gdy kursor *nie* jest nad w*jciem
		if( (currentGuiModule->GetCurrentInputNumber() == -1) &&
			(currentGuiModule->GetCurrentOutputNumber() == -1) )
		{
			isDraggingModule = true;
		}

		// jesli kliknieto wyjscie - mozliwe tworzenie polaczenia
		if( (currentGuiModule->GetCurrentOutputNumber() > -1) ) {
            connSourceModule = currentGuiModule;
            connSourceNumber = currentGuiModule->GetCurrentOutputNumber();
			connectionDrag.Set(NULL, currentGuiModule,
			    currentGuiModule->GetCurrentOutputNumber(), NULL, -1);
			connectionDrag.destinationX = x;
			connectionDrag.destinationY = y;
			connections.push_back(&connectionDrag);
			isDraggingConnection = true;
		}
	} else {
       	lastClick.set_x(x);
		lastClick.set_y(y);
       	// Popup menu
		if( (event->type == GDK_BUTTON_PRESS) && (event->button == 3) )	{
			menuPopup.popup(event->button, event->time);
		}
	}

	// nie przesylamy tego zdarzenia do dzieci
	return true;
}

/*
 Puszczono przycisku myszki.
 Sprawdzamy czy aby nie w trakcie tworzenia polaczenia itp.
*/
bool AlgorithmView::on_button_release_event(GdkEventButton* event) {
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
	TRACE3("AlgorithmView::AddModule()", "Dodaje modul typu '", type, "'");
    ModuleId modId = algorithm.AddModule(type, name);
    Module* mod = algorithm.GetModule(modId);

	GuiModule* guiMod = guiFactory.CreateGuiModule(mod);
	guiMod->SetParentView(this); // konieczne na razie :(
	guiModules.push_back(guiMod);
	guiMod->SetXY(x, y);
    this->put(*guiMod, x, y);
    
	algorithm.PrintInfo();
    
    show_all_children();
    
    TRACE("AlgorithmView::AddModule()", "Gotowe");
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
	//int x, y;
	ConnectionId connId = algorithm.ConnectModules(sourceGuiModule->GetModule()
		->GetName(), sourceNumOutput, destGuiModule->GetModule()->GetName(),
		destNumInput);

	GuiConnection* guiConn = new GuiConnection;
	guiConn->Set( &connId, sourceGuiModule, sourceNumOutput, destGuiModule,
		destNumInput);

	algorithm.CreateQueue();

	algorithm.PrintEdges();
	
	connections.push_back(guiConn);
	window->invalidate_rect( Gdk::Rectangle(0, 0, width, height), false );
}


//void AlgorithmView::ConnectModules(string sourceName, int sourceNumOutput,
//	string destName, int destNumInput)
//{
//	ConnectModules(GetModule(sourceName), sourceNumOutput, GetModule(destName),
//		destNumInput);
//}

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
	for(list<GuiModule*>::iterator modIt = guiModules.begin();
		modIt != guiModules.end(); modIt++)
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
	
	//moduleName2IdMap.clear();
	algorithm.Clear();
	window->clear();
	window->invalidate_rect( Gdk::Rectangle(0, 0, width, height), false );
	//InitAudioPorts();
}

Algorithm* AlgorithmView::GetAlgorithm() {
	return &algorithm;
}

void AlgorithmView::InitAudioPorts() {
  	GuiModule* guiMod;
	ModuleId moduleId;

	guiMod = new GuiModule( algorithm.GetModule("AudioPortIn") );
	guiMod->SetParentView(this);
	guiModules.push_back(guiMod);
	this->put(*guiMod, 0, 0);

	guiMod = new GuiModule( algorithm.GetModule("AudioPortOut") );
	guiMod->SetParentView(this);
	guiModules.push_back(guiMod);
	this->put(*guiMod, 200, 0);
}

void AlgorithmView::on_realize() {
	cout << "realize" << endl;
	Gtk::Layout::on_realize();
	window = get_bin_window();
	fgGc = Gdk::GC::create(window);
	//gc->set_foreground(bgColor);
	
	bgGc = get_style()->get_bg_gc( get_state() );
	
	//window->set_background(bgColor);
	window->clear();
	fgGc->set_foreground(fgColor);
}

//
bool AlgorithmView::on_expose_event(GdkEventExpose* event) {
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
