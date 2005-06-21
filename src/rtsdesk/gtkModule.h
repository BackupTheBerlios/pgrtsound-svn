#ifndef GTKMM_GTKMODULE_H
#define GTKMM_GTKMODULE_H

#include <gdkmm/colormap.h>
#include <gtkmm/drawingarea.h>
#include <gdkmm/window.h>

/**
 * Klasa reprezentuj¹ca graficznie modu³ w trakcie projektowani w desku
 */
class GtkModule : public Gtk::DrawingArea
{
    public:
        GtkModule(int iCount, int oCount);
        virtual ~GtkModule();      
        virtual bool on_expose_event(GdkEventExpose* e);
        int GetSize();
        void SetSelected(bool selected_);    
          
        
    protected:        
        int   size;
        int   width;
        int   inputCount;
        int   outputCount;
        bool  selected;
        Glib::RefPtr<Gdk::GC> gc_;
        Gdk::Color bgColorSelected_,bgColorUnSelected_, conColor_;
      
        virtual void on_realize();
};


#endif
