#include "guimodulewidget.h"
#include "algorithmview.h"

// rozmiar kwadracikow gniazdek
const int GuiModuleWidget::socketSize = 7;
const int GuiModuleWidget::socketSizeDoubled = 2 * GuiModuleWidget::socketSize;

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

	// glowny prostokat modulu
	window->draw_rectangle(gc, false, 0, 0, width - 1, height - 1);

	// gniazda wejsc
    for (int i = 1; i <= inputCount; i++) {
        window->draw_rectangle(gc, true, 0, (i - 1)*socketSizeDoubled, socketSize,
			socketSize);
    }

	// gniazda wyjsc
    for (int i = 1; i <= outputCount; i++) {
        window->draw_rectangle(gc, true, width - socketSize, (i - 1)*socketSizeDoubled,
			socketSize, socketSize);
    }

    return true;
}

/*
 Docelowo modul mia miec tekst na srodku z jego nazwa.
 Ponizsza funckja bedzie pozwalala zmieniac ten tekst.
*/
void GuiModuleWidget::SetText(string newText) {
	text = newText;
}

/*
 Kursor wchodzi nad modul.
 'Aktywujemy' ten modul w AlgorithmView.
*/
bool GuiModuleWidget::on_enter_notify_event(GdkEventCrossing* event) {
	//cout << "Entering " << guiModule->GetModule()->GetName() << endl;
	algorithmView->SelectWidget(this);
	return true;
}

/*
 Kursor opuszcza modul.
 Dajemy znac AlgorithmView, ze nie jet on juz aktywnym modulem.
*/
bool GuiModuleWidget::on_leave_notify_event(GdkEventCrossing* event) {
    //cout << "Leaving " << guiModule->GetModule()->GetName() << endl;
    if( currentInput > -1 ) PaintInput(currentInput, false);
	if( currentOutput > -1 ) PaintOutput(currentOutput, false);
	
	// podczas szybkiego przesuwania modulu kursor moze wyjsc poza modul
	// co by go zdeaktywowalo, stad ponizszy warunek zanik deaktywacja
	if( !algorithmView->IsDraggingModule() ) {
		algorithmView->SelectWidget(NULL);
	}
	
	return true;
}

/*
 Rysowanie gniadka wejscia.
 @param num NUmer wejscia
 @param isSel Gniazdko zapalone czy nie?
*/
void GuiModuleWidget::PaintInput(int num, bool isSel) {
	Glib::RefPtr<Gdk::Window> window = get_window();
	(isSel)? gc->set_foreground( bgColor ) : gc->set_foreground( fgColor );
    window->draw_rectangle(gc, true, 0, num*socketSizeDoubled, socketSize, socketSize);
}

/*
 Rysowanie gniadka wyjscia.
 @param num NUmer wejscia
 @param isSel Gniazdko zapalone czy nie?
*/
void GuiModuleWidget::PaintOutput(int num, bool isSel) {
	Glib::RefPtr<Gdk::Window> window = get_window();
	(isSel)? gc->set_foreground( bgColor ) : gc->set_foreground( fgColor );
    window->draw_rectangle(gc, true, width - socketSize, num*socketSizeDoubled,
		socketSize , socketSize);
}

/*
 Na podstawie podanyc wspolrzednych (w ramach widgeta modulu) aktywuje
 wewnetrznie wejscie lub wyjscie, nad ktorym jest kursor.
*/
void GuiModuleWidget::FindXput(int x, int y) {
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
		// num ptencjalny numer w*jscia
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

int GuiModuleWidget::GetCurrentInputNumber() {
	return currentInput;
}

int GuiModuleWidget::GetCurrentOutputNumber() {
	return currentOutput;
}

GuiModule* GuiModuleWidget::GetGuiModule() {
	return guiModule;
}

