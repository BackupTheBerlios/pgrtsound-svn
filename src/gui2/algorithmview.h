#ifndef ALGORITHMVIEW_H
#define ALGORITHMVIEW_H

//#include "guis/gui.h"

#include "guimodulefactory.h"
#include "guimodule.h"
#include "guiconnection.h"

#include "../debug.h"
#include "../algorithm.h"

#include <gtkmm/window.h>
#include <gtkmm/layout.h>
#include <gtkmm/menu.h>
#include <gtkmm/messagedialog.h>

#include <list>
#include <string>
#include <map>

/**
 * Widok graficznych reprezentacji modulow.
 */
class AlgorithmView : public Gtk::Layout {
	public:
		AlgorithmView();
		~AlgorithmView();
		void AddModule(string type, string name, int x, int y);
		void onMenuAddModule(string type);
		void DeleteConnection( GuiModule* module, int inpuitId );
		void ConnectModules(GuiModule* from, int fomNumoutput, GuiModule* to,
			int toNuminput);
		void SelectGuiModule( GuiModule* guiMod );
		void Clear();
		GuiModule* GetModule( std::string name );
		void RedrawConnections();
		bool IsDraggingModule();
		Algorithm* GetAlgorithm();
		void LoadFromFile(string fileName);
		void SaveToFile(string fileName);
		//void SetParentWindow( Gtk::Window *window );
		void DeleteModule( GuiModule* guiModule );
		bool on_expose_event(GdkEventExpose* e);
		void on_realize();
		bool on_motion_notify_event(GdkEventMotion* even);
		bool on_button_press_event(GdkEventButton* event);
		bool on_button_release_event(GdkEventButton* event);
		bool ChangeModuleName( ModuleId moduleId, string str );
		// nowy sygnal
   		typedef sigc::signal<void, Glib::ustring> type_signal_notify_xput;
		type_signal_notify_xput signal_notify_xput();
		void InitAudioPorts();
		void FindCurrentModule( int x, int y );

	private:
		type_signal_notify_xput m_signal_notify_xput;
		bool isDraggingModule;
		bool isDraggingConnection;
		int width, height;
		GuiModule* currentGuiModule; // modul pod kursorem
		GuiModule* connSourceModule, * connDestModule; // moduly miedzy ktorymi chcemy polaczenie
		int connSourceNumber, connDestNumber; // numer wejscia i wyjscia dla tworzonego polaczenia
		int currentGuiModuleX, currentGuiModuleY; // gdzie byl kursor nad kliknietym widgecie po kliknieciu
		std::map<string, GuiModule*> name2GuiModuleMap;
		Gtk::Adjustment* adjh, * adjv; // zakresy scrollbarow
		std::list<GuiModule*> guiModules;
		std::list<GuiConnection*> connections;
		Algorithm algorithm;
		GuiModuleFactory guiFactory;
		Glib::RefPtr<Gdk::GC> fgGc, bgGc;
		Gdk::Color fgColor;
		Glib::RefPtr<Gdk::Window> window;
		GuiConnection connectionDrag;
		Gtk::Menu menuPopup;
		Gdk::Point lastClick;

		void UpdateConnections();
};

#endif // ALGORITHMVIEW_H
