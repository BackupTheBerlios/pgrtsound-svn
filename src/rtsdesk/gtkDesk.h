#ifndef GTKMM_EXAMPLE_MYAREA_H
#define GTKMM_EXAMPLE_MYAREA_H

#include <gtkmm/drawingarea.h>
#include <gdkmm/colormap.h>
#include <gdkmm/window.h>

#include "desk.h"

class GtkDesk : public Gtk::DrawingArea
{
public:
  GtkDesk(Desk *d);
  virtual ~GtkDesk();
 virtual bool on_expose_event(GdkEventExpose* e);
 //Biurko
  Desk *desk;
protected:
  
  virtual void on_realize();
 

  Glib::RefPtr<Gdk::GC> gc_;
  Gdk::Color bgColor_, conColor_,gridColor_;
};


#endif
