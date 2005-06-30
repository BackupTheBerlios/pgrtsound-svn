#ifndef ALGORITHMVIEW_H
#define ALGORITHMVIEW_H

#include <list>
#include <string>

#include <gtkmm/window.h>
#include <gtkmm/layout.h> 

#include "../debug.h"
#include "../algorithm.h"
#include "guimodulefactory.h"
#include "guimodules/guimodule.h"
#include "guiconnection.h"

/**
 * Widok graficznych reprezentacji modulow.
 */
class AlgorithmView : public Gtk::Layout {
	public:
		AlgorithmView();
		~AlgorithmView();
		void AddModule(string type, string name, int x, int y);
		//void ConnectModules(string from, int fomNumoutput, string to,
		//	int toNuminput);
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
		bool on_expose_event(GdkEventExpose* e);
		void on_realize();
		bool on_motion_notify_event(GdkEventMotion* even);
		bool on_button_press_event(GdkEventButton* event);
		bool on_button_release_event(GdkEventButton* event);

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
		// zakresy scrollbarow
		Gtk::Adjustment* adjh, * adjv;
		std::list<GuiModule*> guiModules;
		std::list<GuiConnection*> connections;
		//std::map<string, GuiModule*> moduleName2IdMap;
		Algorithm algorithm;
		GuiModuleFactory guiFactory;
		Glib::RefPtr<Gdk::GC> fgGc, bgGc;
		Gdk::Color fgColor;
		// bgColor;
		Glib::RefPtr<Gdk::Window> window;
		GuiConnection connectionDrag;
		//Rodzic
		Gtk::Window *parent;
};

#endif // ALGORITHMVIEW_H
