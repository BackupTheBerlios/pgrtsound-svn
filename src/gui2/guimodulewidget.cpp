#include "guimodulewidget.h"
#include "algorithmview.h"

// rozmiar kwadracikow gniazdek
const int GuiModuleWidget::xputSize = 7;
const int GuiModuleWidget::xputSizeDoubled = 2 * GuiModuleWidget::xputSize;

GuiModuleWidget::GuiModuleWidget(GuiModule* guiMod, AlgorithmView* algoView,
	 int inCount, int outCount)
{
	algorithmView = algoView;
	guiModule = guiMod;
	inputCount = inCount;
	outputCount = outCount;
	currentInput = currentOutput = -1;

	// ustalenie rozmiarow (zaleznie od ilosci wejsc/wyjsc)
    height = (inputCount < outputCount)? outputCount : inputCount;
    height = height * xputSizeDoubled;
    if (height < 3 * xputSizeDoubled) height = 3 * xputSizeDoubled;
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

GuiModuleWidget::~GuiModuleWidget() {
     TRACE("GuiModuleWidget::~GuiModuleWidget()", "Destrukcja");
}

void GuiModuleWidget::on_realize() {
	// We need to call the base on_realize()
	Gtk::EventBox::on_realize();

	Glib::RefPtr<Gdk::Window> window = get_window();
	gc = Gdk::GC::create(window);
	window->set_background(bgColor);
	window->clear();
	gc->set_foreground(fgColor);
}

bool GuiModuleWidget::on_expose_event(GdkEventExpose* e) {
	Glib::RefPtr<Gdk::Window> window = get_window();

	window->set_background(bgColor);
	window->clear();

	window->draw_rectangle(gc, false, 0, 0, width - 1, height - 1);

    for (int i = 1; i <= inputCount; i++) {
        window->draw_rectangle(gc, true, 0, (i - 1)*xputSizeDoubled, xputSize,
			xputSize);
    }

    for (int i = 1; i <= outputCount; i++) {
        window->draw_rectangle(gc, true, width - xputSize, (i - 1)*xputSizeDoubled,
			xputSize, xputSize);
    }

    return true;
}

void GuiModuleWidget::SetText(string newText) {
	text = newText;
}

bool GuiModuleWidget::on_leave_notify_event(GdkEventCrossing* event) {
    //cout << "Leaving " << guiModule->GetModule()->GetName() << endl;
    if( currentInput > -1 ) PaintInput(currentInput, false);
	if( currentOutput > -1 ) PaintOutput(currentOutput, false);
	
	// podczas szybkiego przesuwania modulu kursor moze wyjsc poza modul :(
	// ponizej obrona
	if( !algorithmView->IsDraggingModule() ) {
		algorithmView->SelectWidget(NULL);
	}
	
	return true;
}

bool GuiModuleWidget::on_enter_notify_event(GdkEventCrossing* event) {
	//cout << "Entering " << guiModule->GetModule()->GetName() << endl;
	//algorithmView->StartModuleDrag(this, x, y);
	algorithmView->SelectWidget(this);
	return true;
}

void GuiModuleWidget::PaintInput(int num, bool isSel) {
	Glib::RefPtr<Gdk::Window> window = get_window();
	(isSel)? gc->set_foreground( bgColor ) : gc->set_foreground( fgColor );
    window->draw_rectangle(gc, true, 0, num*xputSizeDoubled, xputSize, xputSize);
}

void GuiModuleWidget::PaintOutput(int num, bool isSel) {
	Glib::RefPtr<Gdk::Window> window = get_window();
	(isSel)? gc->set_foreground( bgColor ) : gc->set_foreground( fgColor );
    window->draw_rectangle(gc, true, width - xputSize, num*xputSizeDoubled,
		xputSize , xputSize);
}

void GuiModuleWidget::FindXput(int x, int y) {
	// numery znalezionego w*jscia
	int inFound = -1, outFound = -1;
	int num;
	float temp;

	// biezaca wysokosc przez rozmiar kwadracika gniazdka
	temp = (float)y / (float)xputSize;
	temp = fabs(temp);
	num = (int)temp;

	// tylko parzyste num
	if(num % 2 == 0) {
		// num ptencjalny numer w*jscia
		num = num/2;
		// wesjcie
		if( (x > 0) && (x <= xputSize) && (num < inputCount) ) {
			inFound = num;
		}
		// wyjscie
		if ( (x < width - 1) && (x >= (width - xputSize)) && (num < outputCount) ) {
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

int GuiModuleWidget::GetCurrentInputNumber() {
	return currentInput;
}


int GuiModuleWidget::GetCurrentOutputNumber() {
	return currentOutput;
}

GuiModule* GuiModuleWidget::GetGuiModule() {
	return guiModule;
}

//bool GuiModuleWidget::on_button_press_event(GdkEventButton* event) {
//	int x, y;
//	get_pointer(x, y);
//
//	if( currentOutput > -1 ) {
//		// sygnalizujemy mozliwosc rysowaina polaczenia
//		algorithmView->StartConnectionDrag();
//	} else if (currentInput > -1 ) {
//		// nic
//	} else {
//		// sygnalizujemy mozliwosc przesuwania modulu
//		algorithmView->StartModuleDrag(this, x, y);
//	}
//
////	if( (x > xputSize) && (x < width - xputSize) )
//
//	// propagujemy dalej event
//	return false;
//}

//bool GuiModuleWidget::on_motion_notify_event(GdkEventMotion* event) {
//	//cout << "move" << endl;
//
//	if(event && event->window) {
//		const Glib::RefPtr<Gdk::Window> refWindow =
//			Glib::wrap((GdkWindowObject*) event->window, true); // true == take_copy
//
//		if(refWindow) {
//			int x = 0, y = 0;
//			Gdk::ModifierType state = Gdk::ModifierType(0);
//			refWindow->get_pointer(x, y, state);
//
//			//get_pointer(x, y);
//			algorithmView->StartModuleDrag(this, x, y);
////			char txt[40];
////			g_snprintf(txt, 40, "x: %d    y: %d", x, y);
////			cout << txt << endl;
//
//			//FindXput(x, y);
//		}
//	}
//
//	// proppagujemy event dalej
//	return false;
//}


//bool GuiModuleWidget::on_button_release_event(GdkEventButton* event) {
//	cout << "Relesed on " << guiModule->GetModule()->GetName() << endl;
//	return false;
//}
