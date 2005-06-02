#ifndef GTKMM_GtkModule_H
#define GTKMM_GtkModule_H

#include <gtkmm/drawingarea.h>
#include <gdkmm/colormap.h>
#include <gdkmm/window.h>

class GtkModule : public Gtk::DrawingArea
{
public:
  GtkModule(int iCount, int oCount);
  virtual ~GtkModule();
  int   size;
  void SetSelected(bool selected_);
  virtual bool on_expose_event(GdkEventExpose* e);
protected:
  //Overridden default signal handlers:
  virtual void on_realize();
  
  int   width;
  int   inputCount;
  int   outputCount;
  bool  selected;
  

  Glib::RefPtr<Gdk::GC> gc_;
  Gdk::Color bgColorSelected_,bgColorUnSelected_, conColor_;
};


#endif
