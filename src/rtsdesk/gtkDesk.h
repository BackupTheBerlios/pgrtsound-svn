#ifndef GTKMM_GTKDESK_H
#define GTKMM_GTKDESK_H

#include <gtkmm/drawingarea.h>
#include <gdkmm/colormap.h>
#include <gdkmm/window.h>

#include "desk.h"


/**
 * Klasa reprezentuj¹ca graficznie biurko na którym 
 * projektowany/wyœwietlany jest algorytm
 */
class GtkDesk : public Gtk::DrawingArea
{
    public:
        GtkDesk(Desk *desk_);
        virtual ~GtkDesk();
        virtual bool on_expose_event(GdkEventExpose* e);            
        
    protected:
        Desk *desk;
        Glib::RefPtr<Gdk::GC> gc_;
        Gdk::Color bgColor_, conColor_,gridColor_, activeColor_;    
        
        virtual void on_realize();
};


#endif
