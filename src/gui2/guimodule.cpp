#include "guimodule.h"
#include "algorithmview.h"

// rozmiar kwadracikow gniazdek
const int GuiModule::socketSize = 5;
const int GuiModule::socketSizeDoubled = 2 * GuiModule::socketSize;

GuiModule::GuiModule(Module* moduleToAttach) :
	gui(NULL)
{
   	module = moduleToAttach;
	x = 0;
	y = 0;
	
	guiWindow = NULL;
	isGuiWindowCreated = false;
	inputCount = module->GetInputCount();
	outputCount = module->GetOutputCount();
	currentInput = currentOutput = -1;

	// tablica na identyfikatory polaczen zwiazanych z kazdym wejsciem
	//inputConnectionIds = new ConnectionId[ inputCount ];
	inputGuiConnections = new GuiConnection*[ inputCount ];
	
	// ustalenie rozmiarow (zaleznie od ilosci wejsc/wyjsc)
    height = (inputCount < outputCount)? outputCount : inputCount;
    height = height * socketSizeDoubled;
    if (height < 3 * socketSizeDoubled) height = 3 * socketSizeDoubled;
    width = 100;

    set_size_request(width, height);

	// kolory
 	Glib::RefPtr<Gdk::Colormap> colormap = get_default_colormap ();
	bgColor = Gdk::Color("white");
	fgColor = Gdk::Color("blue");
	colormap->alloc_color(bgColor);
	colormap->alloc_color(fgColor);

	// potrzebne wykrywanie ponizszych zdarzen
	set_events(Gdk::LEAVE_NOTIFY_MASK | Gdk::ENTER_NOTIFY_MASK /*| Gdk::BUTTON_PRESS_MASK |
		Gdk::POINTER_MOTION_MASK | Gdk::POINTER_MOTION_HINT_MASK */);
}

GuiModule::~GuiModule() {
	delete guiWindow;
	delete []inputGuiConnections;
	delete gui;
	TRACE("GuiModule::~GuiModule - Zakonczony\n");
}

void GuiModule::SetParentView(AlgorithmView* algoView) {
	algorithmView = algoView;
}

Gtk::Widget* GuiModule::GetGui() {
	cout << "GuiModule::GetGui" << endl;
	if( gui != NULL) return gui->GetGui();
	return NULL;
}

void GuiModule::SetXY(int x_, int y_) {
    x = x_;
    y = y_;
}

Module* GuiModule::GetModule() {
    return module;
}

void GuiModule::on_realize() {
	Gtk::EventBox::on_realize();
	window = get_window();
	gc = Gdk::GC::create(window);

	pixmapBuffer = Gdk::Pixmap::create( window, width, height, -1);
	
	Repaint();
}

bool GuiModule::on_expose_event(GdkEventExpose* event) {
 	get_window()->draw_drawable(
		gc,
		pixmapBuffer,
		// Only copy the area that was exposed:
		event->area.x, event->area.y,
		event->area.x, event->area.y,
		event->area.width, event->area.height);

    return true;
}

/*
 Kursor wchodzi nad modul.
 'Aktywujemy' ten modul w AlgorithmView.
*/
bool GuiModule::on_enter_notify_event(GdkEventCrossing* event) {
	(void) event;
	//cout << "Entering " << guiModule->GetModule()->GetName() << endl;
	//algorithmView->SelectGuiModule(this);

	return true;
}

/*
 Kursor opuszcza modul.
 Dajemy znac AlgorithmView, ze nie jet on juz aktywnym modulem.
*/
bool GuiModule::on_leave_notify_event(GdkEventCrossing* event) {
	(void) event;
    //cout << "Leaving " << guiModule->GetModule()->GetName() << endl;
    if( currentInput > -1 ) PaintInput(currentInput, false);
	if( currentOutput > -1 ) PaintOutput(currentOutput, false);

	// podczas szybkiego przesuwania modulu kursor moze wyjsc poza modul
	// co by go zdeaktywowalo, stad ponizszy warunek
	//if( !algorithmView->IsDraggingModule() ) {
	//	algorithmView->SelectGuiModule(NULL);
	//}



	return true;
}

/*
 Rysowanie gniadka wejscia.
 @param num NUmer wejscia
 @param isSel Gniazdko zapalone czy nie?
*/
void GuiModule::PaintInput(int num, bool isSel) {
	Glib::RefPtr<Gdk::Window> window = get_window();
	(isSel)? gc->set_foreground( bgColor ) : gc->set_foreground( fgColor );
    window->draw_rectangle(gc, true, 0, num*socketSizeDoubled, socketSize, socketSize);
}

/*
 Rysowanie gniadka wyjscia.
 @param num NUmer wejscia
 @param isSel Gniazdko zapalone czy nie?
*/
void GuiModule::PaintOutput(int num, bool isSel) {
	Glib::RefPtr<Gdk::Window> window = get_window();
	(isSel)? gc->set_foreground( bgColor ) : gc->set_foreground( fgColor );
    window->draw_rectangle(gc, true, width - socketSize, num*socketSizeDoubled,
		socketSize , socketSize);
}

/*
 Na podstawie podanyc wspolrzednych (w ramach widgeta modulu) aktywuje
 wewnetrznie wejscie lub wyjscie, nad ktorym jest kursor.
*/
void GuiModule::FindXput(int x, int y) {
	// numery znalezionego w*jscia
	int inFound = -1, outFound = -1;
	int num;
	float temp;

	// biezaca wysokosc przez rozmiar kwadracika gniazdka
	temp = (float)y / (float)socketSize;
	temp = fabs(temp);
	num = (int)temp;

	// tylko parzyste num
	if(num % 2 == 0) {
		// num to potencjalny numer w*jscia
		num = num/2;
		// wesjcie
		if( (x > 0) && (x <= socketSize) && (num < inputCount) ) {
			inFound = num;
		}
		// wyjscie
		if ( (x < width - 1) && (x >= (width - socketSize)) && (num < outputCount) ) {
			outFound = num;
		}
	}

	if(inFound > -1) {
		// kursor jest nad wejsciem
		PaintInput(inFound, true);
		currentInput = inFound;
		//cout << "wejscie nr : " << currentInput << endl;
	} else if ( currentInput > - 1 ) {
		// kursor opuscil poprzednie wejscie
		PaintInput(currentInput, false);
		currentInput = -1;
	}

	if(outFound > -1) {
   		// kursor jest nad wyjsciem
		PaintOutput(outFound, true);
		currentOutput = outFound;
		//cout << "wyjscie nr : " << currentOutput << endl;
	} else if ( currentOutput > - 1 ) {
        // kursor opuscil poprzednie wyjscie
		PaintOutput(currentOutput, false);
		currentOutput = -1;
	}
}

int GuiModule::GetCurrentInputNumber() {
	return currentInput;
}

int GuiModule::GetCurrentOutputNumber() {
	return currentOutput;
}

void GuiModule::GetInputPosition(int inNum, int& xx, int& yy) {
	xx = x;
	yy = y + inNum * GuiModule::socketSizeDoubled + GuiModule::socketSize/2;

}

void GuiModule::GetOutputPosition(int outNum, int& xx, int& yy) {
  	xx = x + width;
	yy = y + outNum * GuiModule::socketSizeDoubled + GuiModule::socketSize/2;
}

void GuiModule::GetPosition(int& xx, int& yy) {
	xx = x;
	yy = y;
}

void GuiModule::OpenGuiWindow() {
	if( !isGuiWindowCreated ) {
   		guiWindow = new ModuleGuiWindow( this );
   		
		Gtk::Widget* gui = GetGui();
		if( gui != NULL ) {
            //guiWindow->add( *manage(gui) );
            guiWindow->AddGui( gui );
		}
		
        guiWindow->SetName( module->GetName() );
		Glib::ustring title =  module->GetName() + " - " + module->GetType();
		guiWindow->set_title( title );
		guiWindow->set_transient_for( *( (Gtk::Window*)get_toplevel() ) );
		guiWindow->set_position( Gtk::WIN_POS_CENTER );
		guiWindow->show_all_children();
        guiWindow->show();
        isGuiWindowCreated = true;
	}
	else if( guiWindow != NULL ) {
		guiWindow->show();
		guiWindow->raise();
	}
}

void GuiModule::SetInputGuiConnection( int inputId, GuiConnection* conn ) {
	inputGuiConnections[ inputId ] = conn;
}
		//ConnectionId GetInputConnectionId( int inputId );
GuiConnection* GuiModule::GetInputGuiConnection( int inputId ) {
	return inputGuiConnections[ inputId ];
}


void GuiModule::SetModuleId( ModuleId modId ) {
	moduleId = modId;
}

ModuleId GuiModule::GetModuleId() {
	return moduleId;
}

void GuiModule::SetGui( Gui* newGui ) {
	gui = newGui;
}

void GuiModule::ChangeName() {
	Glib::ustring newName = guiWindow->GetName();
	TRACE( "GuiModule::ChangeName - Change name: '%s'\n", newName.c_str() );
	if( algorithmView->ChangeModuleName( moduleId, newName ) ) {
        Glib::ustring title = module->GetName() + " - " + module->GetType();
		guiWindow->set_title( title );
		Repaint();
	}
}

void GuiModule::Repaint() {
	window = get_window();
	gc = Gdk::GC::create( window );

	gc->set_foreground(bgColor);
	pixmapBuffer->draw_rectangle(gc, true, 0, 0, width, height);

	gc->set_foreground(fgColor);

	//window->set_background(bgColor);
	//window->clear();

	// glowny prostokat modulu
	pixmapBuffer->draw_rectangle(gc, false, 0, 0, width - 1, height - 1);

	// gniazda wejsc
    for (int i = 1; i <= inputCount; i++) {
        pixmapBuffer->draw_rectangle(gc, true, 0, (i - 1)*socketSizeDoubled,
			socketSize, socketSize);
    }

	// gniazda wyjsc
    for (int i = 1; i <= outputCount; i++) {
        pixmapBuffer->draw_rectangle(gc, true, width - socketSize,
			(i - 1)*socketSizeDoubled, socketSize, socketSize);
    }

   	int xsize, ysize;
	pangolayout = create_pango_layout( module->GetName() );
	pangolayout->set_ellipsize(Pango::ELLIPSIZE_END);
	pangolayout->set_width(90000); // TODO: wtf?
	//cout << pangolayout->get_width() << endl;
	pangolayout->get_pixel_size(xsize, ysize);
	pixmapBuffer->draw_layout(gc, (width - xsize)/2 , (height - ysize)/2, pangolayout);
	
	queue_draw();

}

int GuiModule::GetWidth() const {
	return width;
}

int GuiModule::GetHeight() const {
	return height;
}
