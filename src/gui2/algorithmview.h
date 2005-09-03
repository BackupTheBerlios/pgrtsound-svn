#ifndef ALGORITHMVIEW_H
#define ALGORITHMVIEW_H

#include "../debug.h"
#include "../algorithm.h"
#include "guimodulefactory.h"
#include "guimodules/guimodule.h"
#include "guiconnection.h"

#include <gtkmm/window.h>
#include <gtkmm/layout.h>
#include <gtkmm/menu.h>

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
		//void ConnectModules(string from, int fomNumoutput, string to,
		//	int toNuminput);
		void DeleteConnection( GuiModule* module, int inpuitId );
		void ConnectModules(GuiModule* from, int fomNumoutput, GuiModule* to,
			int toNuminput);
		void SelectGuiModule(GuiModule* guiMod);
		void Clear();
		GuiModule* GetModule(std::string name);
		void RedrawConnections();
		bool IsDraggingModule();
		Algorithm* GetAlgorithm();
		void LoadFromFile(string fileName);
		void SetParentWindow(Gtk::Window *window);
		void DeleteModule( GuiModule* guiModule );
		bool on_expose_event(GdkEventExpose* e);
		void on_realize();
		bool on_motion_notify_event(GdkEventMotion* even);
		bool on_button_press_event(GdkEventButton* event);
		bool on_button_release_event(GdkEventButton* event);
		// No description
		void UpdateConnections();

	private:
		void InitAudioPorts();

		bool isDraggingModule;
		bool isDraggingConnection;
		int width, height;
		// modul pod kursorem
		GuiModule* currentGuiModule;
		// moduly miedzy ktorymi chcemy polaczenie
		GuiModule* connSourceModule, * connDestModule;
		// numer wejscia i wyjscia dla tworzonego polaczenia
		int connSourceNumber, connDestNumber;
		// gdzie byl kursor nad kliknietym widgecie po kliknieciu
		int currentGuiModuleX, currentGuiModuleY;
		std::map<string, GuiModule*> name2GuiModuleMap;
		// zakresy scrollbarow
		Gtk::Adjustment* adjh, * adjv;
		std::list<GuiModule*> guiModules;
		std::list<GuiConnection*> connections;
		Algorithm algorithm;
		GuiModuleFactory guiFactory;
		Glib::RefPtr<Gdk::GC> fgGc, bgGc;
		Gdk::Color fgColor;
		// bgColor;
		Glib::RefPtr<Gdk::Window> window;
		GuiConnection connectionDrag;
		//Rodzic
		Gtk::Window *parent;
		Gtk::Menu menuPopup;
		Gdk::Point lastClick;
};

#endif // ALGORITHMVIEW_H
