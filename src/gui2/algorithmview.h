#ifndef ALGORITHMVIEW_H
#define ALGORITHMVIEW_H

#include <list>

#include <gtkmm/layout.h> 

#include "../debug.h"
#include "../algorithm.h"
#include "guimodulefactory.h"
#include "guimodules/guimodule.h"

struct GuiConnection {
	ConnectionId connectionId;
	int sourceX, sourceY;
	int destinationX, destinationY;
	// TODO: Gtk::EventBox strzalka;
};

/**
 * Widok graficznych reprezentacji modulow.
 */
class AlgorithmView : public Gtk::Layout {
	public:
		AlgorithmView(Algorithm* algo);
		~AlgorithmView();
		void AddModule(string type, string name, int x, int y);
		void ConnectModules(GuiModule* from, int fomNumoutput, GuiModule* to,
			int toNuminput);
		void SelectGuiModule(GuiModule* guiMod);
		void DrawAlgorithm();
		void CreateModuleWindow(GuiModule* gui);
		void Clear();
		//void DrawConnectionDrag();
		bool IsDraggingModule();
		Algorithm* GetAlgorithm();
		bool on_motion_notify_event(GdkEventMotion* even);
		bool on_button_press_event(GdkEventButton* event);
		bool on_button_release_event(GdkEventButton* event);

	private:
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
		std::list<GuiConnection> connections;

		Algorithm* algorithm;
		GuiModuleFactory guiFactory;
};

#endif // ALGORITHMVIEW_H
