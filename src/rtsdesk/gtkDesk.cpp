#include "gtkDesk.h"

GtkDesk::GtkDesk(Desk *d)
{
  desk = d;

  Glib::RefPtr<Gdk::Colormap> colormap = get_default_colormap ();

  bgColor_ = Gdk::Color("white");
  conColor_ = Gdk::Color("blue");
  gridColor_= Gdk::Color("navajo white");
  //gridColor_.set_rgb(20,20,20);

  colormap->alloc_color(bgColor_);
  colormap->alloc_color(conColor_);
  colormap->alloc_color(gridColor_);
}

GtkDesk::~GtkDesk()
{
}

void GtkDesk::on_realize()
{
  // We need to call the base on_realize()
  Gtk::DrawingArea::on_realize();

  // Now we can allocate any additional resources we need
  Glib::RefPtr<Gdk::Window> window = get_window();
  gc_ = Gdk::GC::create(window);
  window->set_background(bgColor_);
  window->clear();
  gc_->set_foreground(conColor_);
}

bool GtkDesk::on_expose_event(GdkEventExpose* /* event */)
{ 
    // This is where we draw on the window
    int x1=-1,y1=-1,x2=-1,y2=-1;
    Glib::RefPtr<Gdk::Window> window = get_window();
    window->clear();
    
    //siatka
    gc_->set_foreground(gridColor_);
    for (int x=1;x<=800;x+=20)
    {
        window->draw_line(gc_, x, 0, x, 600);
        window->draw_line(gc_, 0, x, 800, x);
    }
    
    gc_->set_foreground(conColor_);

    for (int c1=0; c1 < desk->deskModules.size(); c1++)
    {
        for (int input = 0; input < desk->deskModules[c1]->GetRTSModule()->GetInputCount(); input++)
        {
            x2 = desk->deskModules[c1]->x;
            y2 = desk->deskModules[c1]->y+input*10;
            float *signal = desk->deskModules[c1]->GetRTSModule()->GetInput(input)->GetSignal();    
            
            for (int c2=0; c2 < desk->deskModules.size(); c2++)
            {
                for (int output = 0; output < desk->deskModules[c2]->GetRTSModule()->GetOutputCount(); output++)
                {              
                    if (desk->deskModules[c2]->GetRTSModule()->GetOutput(output)->GetSignal() == signal)
                    {
                        x1 = desk->deskModules[c2]->x+65;
                        y1 = desk->deskModules[c2]->y+input*10;                      
                    }
                }
            }
        
            if ((x1==-1)|(x2==-1)|(y1==-1)|(y2==-1))
            {
            }
            else
            {
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
