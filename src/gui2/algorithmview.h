#ifndef GUIMODULESVIEW_H
#define AlgorithmView_H

#include <list>

#include <gtkmm/layout.h> 

#include "../debug.h"
#include "../algorithm.h"
#include "guimodulefactory.h"
#include "guimodules/guimodule.h"

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
		bool IsDraggingModule();
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
		
		Gtk::Adjustment* adjh, * adjv;
		std::list<GuiModule*> guiModules;

		Algorithm* algorithm;
		GuiModuleFactory guiFactory;
};

#endif // AlgorithmView_H
