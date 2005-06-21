#include "gtkDesk.h"

/** Konstruktor
 * @param desk_ wska¿nik do obiektu typu desk
 */
GtkDesk::GtkDesk(Desk *desk_) {
    desk = desk_;

    //deklaracja i alokacja kolorów    
    Glib::RefPtr<Gdk::Colormap> colormap = get_default_colormap ();
    bgColor_    = Gdk::Color("white");
    conColor_   = Gdk::Color("blue");
    gridColor_  = Gdk::Color("navajo white");
    activeColor_= Gdk::Color("red");
    colormap->alloc_color(bgColor_);
    colormap->alloc_color(conColor_);
    colormap->alloc_color(gridColor_);
    colormap->alloc_color(activeColor_);
}

GtkDesk::~GtkDesk() {
}

void GtkDesk::on_realize() {
  Gtk::DrawingArea::on_realize();
  Glib::RefPtr<Gdk::Window> window = get_window();
  gc_ = Gdk::GC::create(window);
  window->set_background(bgColor_);
  window->clear();
  gc_->set_foreground(conColor_);
}


//Metoda rysuj¹ca siatkê i wszystkie po³¹czenia
bool GtkDesk::on_expose_event(GdkEventExpose*) { 
    int x1=-1,y1=-1,x2=-1,y2=-1;
    Glib::RefPtr<Gdk::Window> window = get_window();
    window->clear();
    
    //SIATKA
    gc_->set_foreground(gridColor_);
    for (int x=1;x<=800;x+=20) {
        window->draw_line(gc_, x, 0, x, 600);
        window->draw_line(gc_, 0, x, 800, x);
    }
    
    gc_->set_foreground(conColor_);

    //PO£¥CZENIA

    for (int c1=0; c1 < desk->guiModules.size(); c1++) {
        for (int input = 0; input < desk->guiModules[c1]->GetModule()->GetInputCount(); input++) {
            x2 = desk->guiModules[c1]->GetX();
            y2 = desk->guiModules[c1]->GetY()+input*10;
            float *signal = desk->guiModules[c1]->GetModule()->GetInput(input)->GetSignal();              
            for (int c2=0; c2 < desk->guiModules.size(); c2++) {
                for (int output = 0; output < desk->guiModules[c2]->GetModule()->GetOutputCount(); output++) {              
                    if (desk->guiModules[c2]->GetModule()->GetOutput(output)->GetSignal() == signal) {
                        if ((desk->guiModules[c1] == desk->GetDeskModuleActive()) | (desk->guiModules[c2] == desk->GetDeskModuleActive()))
                            gc_->set_foreground(activeColor_);
                        else  gc_->set_foreground(conColor_);
                            
                        
                        x1 = desk->guiModules[c2]->GetX()+65;
                        y1 = desk->guiModules[c2]->GetY()+input*10;                      
                    }
                }
            }
        
            if (!((x1==-1)|(x2==-1)|(y1==-1)|(y2==-1))) {
                cout << "line" << endl;
                window->draw_line(gc_, x1, y1, x2-15, y2);
                window->draw_line(gc_, x2, y2, x2-15, y2);
                window->draw_line(gc_, x2, y2, x2-10, y2+2);
                window->draw_line(gc_, x2, y2, x2-10, y2-2);
                x1=-1;
                x2=-1;
                y1=-1;
                y2=-1;
            }
        }    
    }
    return true;
}
