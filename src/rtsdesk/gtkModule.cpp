#include "GtkModule.h"



/** Konstruktor
 * @param iCount ilo�� wej��
 * @param oCount ilo�� wyj��
 */
GtkModule::GtkModule(int iCount, int oCount) {
    inputCount    = iCount;
    outputCount   = oCount;

    //deklaracja i alokacja kolor�w
    Glib::RefPtr<Gdk::Colormap> colormap = get_default_colormap ();
    bgColorSelected_      = Gdk::Color("gray");
    bgColorUnSelected_    = Gdk::Color("white");
    conColor_             = Gdk::Color("red");
    colormap->alloc_color(bgColorUnSelected_);
    colormap->alloc_color(bgColorSelected_);
    colormap->alloc_color(conColor_);
  
    //modu� nie zaznaczony
    selected = false; 
  
    //ustawienie wysoko�ci modu�u na podstawie ilo�ci wej�� i wyj�� ale nie mniej niz 25 pix
  
    size = inputCount;
    if (size <outputCount) size = outputCount;
    size = size * 5;
    if (size < 25) size = 25;
  
    width = 65;
  
    set_size_request(width,size);
}

/** Destruktor
 */
GtkModule::~GtkModule() {
}

/** On expose event
 * Rysowanie modu�u oraz jego wej�� i wyj��
 */
bool GtkModule::on_expose_event(GdkEventExpose*) {
    Glib::RefPtr<Gdk::Window> window = get_window();
    window->clear();
    
    //kolor wype�nienia uzalezniony od tego czy modu� zaznaczony
    if (selected) {
        window->set_background(bgColorSelected_);
    } else {
        window->set_background(bgColorUnSelected_);  
    }

    window->draw_rectangle(gc_,false, 0, 0, width - 1, size - 1);

    for (int i = 1; i<=inputCount; i++) {
        window->draw_rectangle(gc_,true, 0, (i-1)*10, 5 , 5);
    }
  
    for (int i = 1; i<=outputCount; i++) {
        window->draw_rectangle(gc_,true, width - 5, (i-1)*10, 5 , 5);
    }
    
    return true;
}

/** Ustwianie zanaczenia modu�u
 * @param selected_ czy modu� zaznaczony
 */
void GtkModule::SetSelected(bool selected_) {
    selected = selected_;
    on_expose_event(NULL);
}

/** On realize
 * 
 */
void GtkModule::on_realize() {
  Gtk::DrawingArea::on_realize();

  Glib::RefPtr<Gdk::Window> window = get_window();
  gc_ = Gdk::GC::create(window);

  window->clear();
  gc_->set_foreground(conColor_);
}

int GtkModule::GetSize() {
    return size;
}
