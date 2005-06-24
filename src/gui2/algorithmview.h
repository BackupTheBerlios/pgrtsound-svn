#ifndef GUIMODULESVIEW_H
#define AlgorithmView_H

#include <list>

#include <gtkmm/layout.h> 

#include "../debug.h"
#include "../algorithm.h"
#include "guimodulefactory.h"
#include "guimodulewidget.h"

/**
 * Widok graficznych reprezentacji modulow.
 */
class AlgorithmView : public Gtk::Layout {
	public:
		AlgorithmView(Algorithm* algo);
		~AlgorithmView();
		void AddModule(string type, string name, int x, int y);
		void SelectWidget(GuiModuleWidget* wdg);
		void StartModuleDrag(GuiModuleWidget* wdg, int wdgX, int wdgY);
		void StartConnectionDrag();
		bool IsDraggingModule();
		bool on_motion_notify_event(GdkEventMotion* even);
		bool on_button_press_event(GdkEventButton* event);
		bool on_button_release_event(GdkEventButton* event);

	private:
		bool isDraggingModule;
		bool isDraggingConnection;
		int width, height;
		
		// modul pod kursorem
		GuiModuleWidget* currentWidget;

		// moduly miedzy ktorymi chcemy polaczenie
		GuiModuleWidget* connSourceWidget, * connDestWidget;

		// numer wejscia i wyjscia dla tworzonego polaczenia
		int connSourceNumber, connDestNumber;

		// gdzie byl kursor nad kliknietym widgecie po kliknieciu
		int currentWidgetX, currentWidgetY;
		
		Gtk::Adjustment* adjh, * adjv;
		std::list<GuiModule*> guiModules;
		std::list<GuiModuleWidget*> widgets;

		Algorithm* algorithm;
		GuiModuleFactory guiFactory;
};

#endif // AlgorithmView_H
