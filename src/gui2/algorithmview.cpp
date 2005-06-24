#include "algorithmview.h"

using namespace std;

AlgorithmView::AlgorithmView(Algorithm* algo) {
	TRACE("AlgorithmView::AlgorithmView()", "Konstrukcja...");
	algorithm = algo;
	isDraggingModule = false;
	isDraggingConnection = false;
	currentGuiModule = NULL;
	width = 1000;
	height = 1000;
	set_size(width, height);

	//Ask to receive events the layout doesn't normally subscribe to.
	set_events(/*Gdk::LEAVE_NOTIFY_MASK | Gdk::BUTTON_PRESS_MASK | */
		Gdk::POINTER_MOTION_MASK | Gdk::POINTER_MOTION_HINT_MASK |
		Gdk::BUTTON_RELEASE_MASK );

	TRACE("AlgorithmView::AlgorithmView()", "Done!");
}

AlgorithmView::~AlgorithmView() {
    TRACE("AlgorithmView:~AlgorithmView()", "Destrukcja...");
    currentGuiModule = NULL;

	// pozbywamy sie GuiModulow
	typedef list<GuiModule*>::iterator GMIt;
	for(GMIt it = guiModules.begin(); it != guiModules.end(); it++) {
		delete *it;
	}
	guiModules.clear();

    TRACE("AlgorithmView:~AlgorithmView()", "Destrukcja pomyslna");
}

/*
 Obsluga ruchu myszki.
 Przesuwa modul, robi polaczenie itp. zaleznie od ustawionych flag.
*/
bool AlgorithmView::on_motion_notify_event(GdkEventMotion* event) {
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
						
						// TODO: odswiezanie polaczen do biezacego modulu
					}
				}
			}
			
			if(isDraggingConnection) {
				// TODO: rysowanie drucika podczas ruchu myszka
			}
			
		}
	}

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
			cout << "Double click" << endl;
			// funkcja pusta na razie
			CreateModuleWindow(currentGuiModule);
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
	   		connSourceNumber = connSourceModule->GetCurrentOutputNumber();
			isDraggingConnection = true;
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
				// opuszczono nad wejsciem - mamy polaczenie!
				// ponizej pusta funckja
				ConnectModules(connSourceModule, connSourceNumber, connDestModule, connDestNumber);
				cout << "Connection: " <<
					connSourceModule->GetModule()->GetName() <<
					" [" << connSourceNumber << "]   ->   " <<
					connDestModule->GetModule()->GetName() <<
					" [" << connDestNumber << "]" << endl;
			}
		}
		
		isDraggingConnection = false;
	}
	
	return false;
}

/*
 Dodawanie do widoku modulu.
*/
void AlgorithmView::AddModule(string type, string name, int x, int y) {
	TRACE3("AlgorithmView::AddModule()", "Dodaje modul typu '", type, "'");
    ModuleId modId = algorithm->AddModule(type, name);
    Module* mod = algorithm->GetModule(modId);

	GuiModule* guiMod = guiFactory.CreateGuiModule(mod);
	guiMod->SetParentView(this); // konieczne na razie :(
	guiModules.push_back(guiMod);
    this->put(*guiMod, x, y);
    
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

void AlgorithmView::ConnectModules(GuiModule* from, int fomNumoutput, GuiModule* to,
	int toNuminput)
{
	// TODO: tworzenie polaczenia
	// algorithm->ConnectModules(from->GetModule(), ...)
	// rysowanie druta
}

void AlgorithmView::DrawAlgorithm() {
	// TODO: rysowanie calego algorytmu (po wczytaniu z pliku)
}

void AlgorithmView::CreateModuleWindow(GuiModule* gui) {
	// TODO: wywolanie okna z interfejsem modulu
}
