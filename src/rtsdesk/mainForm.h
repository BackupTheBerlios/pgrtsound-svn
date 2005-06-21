#ifndef GTKMM_EXAMPLE_HELLOWORLD_H
#define GTKMM_EXAMPLE_HELLOWORLD_H

#include <gtkmm.h>
#include "desk.h"
#include "gtkDesk.h"
#include "gtkModule.h"


class MainForm : public Gtk::Window
{

public:
    MainForm(Desk *d);
    virtual ~MainForm();
protected:
  //Signal handlers:

  virtual void OnNewModule();
  virtual void OnNewConnection();
  virtual void OnMenuFileQuit();
  virtual void OnSaveFile();
  virtual void OnLoadFile();
  virtual void OnNew();
  virtual void OnDelete();
  virtual void OnModule();
  
  virtual void AddNewModuleToDesk();
  virtual void ShowModuleParameters(GuiModule*  deskModule);

  //DnD
  virtual void OnModuleDragDataGet(const Glib::RefPtr<Gdk::DragContext>& context, Gtk::SelectionData& selection_data, guint info, guint time);
  virtual void OnModuleDropDragDataReceived(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);
  std::list<Gtk::TargetEntry> listTargets;


  
  //Biurko
  Desk *desk;
  GtkDesk *area;
  
  //Kontenery:
  Gtk::VBox         firstBox;
  Gtk::HBox         secondBox;
  Gtk::Fixed        paramPanel;
  Gtk::Fixed        dnd;

  //Parametry
  Gtk::Button       btDelete;
  Gtk::Button       btModule;
  Gtk::Label        lbParams;
  Gtk::Label        lbName;
  Gtk::Label        lbType;
  Gtk::Entry        edName;
  Gtk::Entry        edType;
  
  Gtk::Widget       *widgetList[50];
  
  
  //Menu
  Glib::RefPtr<Gtk::UIManager>      m_refUIManager;
  Glib::RefPtr<Gtk::ActionGroup>    m_refActionGroup;
  Glib::RefPtr<Gtk::RadioAction>    m_refChoiceOne, m_refChoiceTwo;
};

#endif 
