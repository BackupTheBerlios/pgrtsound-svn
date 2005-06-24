#include "algorithmview.h"

using namespace std;

AlgorithmView::AlgorithmView(Algorithm* algo) {
	TRACE("AlgorithmView::AlgorithmView()", "Konstrukcja...");
	algorithm = algo;
	isDraggingModule = false;
	currentWidget = NULL;
	width = 1000;
	height = 1000;
	set_size(width, height);

	//Ask to receive events the layout doesn't normally subscribe to.
	set_events(/*Gdk::LEAVE_NOTIFY_MASK | Gdk::BUTTON_PRESS_MASK | */
		Gdk::POINTER_MOTION_MASK | Gdk::POINTER_MOTION_HINT_MASK |
		Gdk::BUTTON_RELEASE_MASK );

	// TODO: wywolanie metody 'parsuj algorytm i go rysuj'
	
	TRACE("AlgorithmView::AlgorithmView()", "Done!");
}

AlgorithmView::~AlgorithmView() {
    TRACE("AlgorithmView:~AlgorithmView()", "Destrukcja...");
    currentWidget = NULL;

	// usuwamy widgety
	typedef list<GuiModuleWidget*>::iterator GMLIt;
	for(GMLIt it = widgets.begin(); it != widgets.end(); it++) {
		delete *it;
	}
	widgets.clear();
	
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
			if(currentWidget != NULL) {
				int xpos, ypos;
				currentWidget->get_window()->get_position(xpos, ypos);
				// zapalmy w*jscie jesli kusor nad jakims
				currentWidget->FindXput(x - xpos, y - ypos);
			}

			// przesuwamy modul
			if(isDraggingModule) {
				int newX, newY;
				if( (state & Gdk::BUTTON1_MASK) != 0 ) {
					if(currentWidget != NULL) {
						x = x - currentWidgetX;
						y = y - currentWidgetY;

						// modul nie moze wyjsc poza layout
						if( x < 0 ) x = 0;
						if( ( x + currentWidget->get_width() ) > width )
							x = width - currentWidget->get_width();
						if( y < 0 ) y = 0;
						if( ( y + currentWidget->get_height() ) > height )
							y = height - currentWidget->get_height();

						move(*currentWidget, x, y);
					}
				}
			}
			
			if(isDraggingConnection) {
				//TODO: rysowanie drucika podczas ruchu myszka
			}
			
		}
	}

	return true;
}

/*
 Obluga kliku myszka.
 Sprawdza czy kliknieto modul -> molziwe przsuwanie modulu, czy moze kliknieto
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

	if( currentWidget != NULL) {
        int posx, posy;
		currentWidget->get_window()->get_position(posx, posy);
		currentWidgetX = x - posx;
		currentWidgetY = y - posy;
		currentWidget->get_window()->raise();

		// mozna ruszac modulem tylko gdy kursor *nie* jest nad w*jciem
		if( (currentWidget->GetCurrentInputNumber() == -1) &&
			(currentWidget->GetCurrentOutputNumber() == -1) )
		{
			isDraggingModule = true;
		}

		// jesli kliknieto wyjscie - mozliwe tworzenie polaczenia
		if( (currentWidget->GetCurrentOutputNumber() > -1) ) {
		 	connSourceWidget = currentWidget;
	   		connSourceNumber = connSourceWidget->GetCurrentOutputNumber();
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
		//currentWidget = NULL;
	}

	// sprawdzic czy opuszczono kursor nad jakims wejsciem
	if(isDraggingConnection) {
		connDestWidget = currentWidget;
		if(connDestWidget && (connDestWidget != connSourceWidget) ) {
			// opuszczono nad innym modulem niz poczatkowy
			connDestNumber = connDestWidget->GetCurrentInputNumber();
			if(connDestNumber > -1) {
				// opuszczono nad wejsciem - mamy polaczenie!
				cout << "Connection: " <<
					connSourceWidget->GetGuiModule()->GetModule()->GetName() <<
					" [" << connSourceNumber << "]   ->   " <<
					connDestWidget->GetGuiModule()->GetModule()->GetName() <<
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
	guiModules.push_back(guiMod);
	GuiModuleWidget* widget = new GuiModuleWidget( guiMod, this,
		mod->GetInputCount(), mod->GetOutputCount() );
	widgets.push_back(widget);
    this->put(*widget, x, y);
    
    TRACE("AlgorithmView::AddModule()", "Gotowe");
}

/*
 Aktywacja (focus) wybranego modulu.
*/
void AlgorithmView::SelectWidget(GuiModuleWidget* wdg) {
	if( (currentWidget == NULL) && (wdg != NULL)  ) {
		currentWidget = wdg;
	}
	else if( (currentWidget != NULL) && (wdg == NULL)  ) {
		currentWidget = wdg;
	}
}

bool AlgorithmView::IsDraggingModule() {
	return isDraggingModule;
}
